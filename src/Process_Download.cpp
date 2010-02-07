#include "Process_Download.h"

Process_Download::Process_Download(const QString& _jobName, const QString& _file, const QDir& _outputDir, const QDir& _tempDir, const QString& _userKey)
{
	// Process properties
	setJobName(_jobName);

	// File properties
	type = tr("Download");
	file = _file;
	outputDir = _outputDir;
	tempDir = _tempDir;
	setUserKey(_userKey);

	// State properties
	state = QUEUED;
	setStateString(tr("Queued"));

	metaFileIsValid = false;
}




void Process_Download::run()
{
	// Initialize
	init();


	// Create components
	Crypto crypto;
	Stegano stegano(tempDir, tempDir);
	Transmission transmission(tempDir);


	// If URL -> Download MetaFile first
	if(file.startsWith("http://")) {
		Item remoteMetaFile;
		Item_Link remoteLink;
		remoteLink.url = file;
		remoteMetaFile.remote.hash = "";
		remoteMetaFile.remote.link.push_back(remoteLink);
		if(transmission.download(&remoteMetaFile))
		{
			file = remoteMetaFile.image.file;
			tempFiles.push_back(file);
		}
		else
		{
			setStateString(tr("ERROR: Could not download MetaFile!"));
			state = ERROR;
			return;
		}
	}


	// Validate MetaFile
	setStateString(tr("Loading MetaFile"));
	do
	{
		// If there is a new password (this makes sure that only one password request is sent each try)
		if(userKeyChanged)
		{
			// Validate MetaFile
			if(!metaFileIsValid)
			{
				// Open MetaFile
				QFile f(file);
				f.open(QIODevice::Unbuffered | QIODevice::ReadOnly);
				QByteArray metaFileData;
				while(!f.atEnd())
					metaFileData.append(f.readLine());
				f.close();

				// Try interpreting metaFileData as plain data
				if(!metaHandler.load(metaFileData))
				{
					// If that fails...
					// Try decrypting metaFileData
					QByteArray decryptedMetaFileData(metaFileData);
					crypto.decrypt(decryptedMetaFileData, userKey);
					if(!metaHandler.load(decryptedMetaFileData))
					{
						// If that fails...
						// Try interpreting metaFileData as image
						for(qint32 i = stegano.getNumberMethods(); i > 0; i--)
						{
							QByteArray hiddenMetaFileData;
							hiddenMetaFileData = stegano.getMetaFileFromImage(file, i);

							// Cut raw MetaFileData (to remove trailing waste data)
							QByteArray cutHiddenMetaFileData = hiddenMetaFileData;
							cutHiddenMetaFileData.resize(cutHiddenMetaFileData.lastIndexOf("</meta>") + 7);

							if(!metaHandler.load(cutHiddenMetaFileData))
							{
								qDebug() << "Method " << i << " isnt working without decryption. Trying to decrypt";
								// If that fails...
								// Try decrypting data extracted from image
								crypto.decrypt(hiddenMetaFileData, userKey);
								hiddenMetaFileData.resize(hiddenMetaFileData.lastIndexOf("</meta>") + 7);
								if(!metaHandler.load(hiddenMetaFileData))
								{
									// If all fails...
									// Request a new password and start over
									if(i <= 1) {
										setStateString(tr("ERROR: Could not read MetaFile!"));
										emit invalidUserKey(this);
									}
								}
								else
								{
									qDebug() << "Metafile is valid and loaded. Exiting loop with break";
									metaFileIsValid = true;
									break;
								}
							}
							else
							{
								qDebug() << "Metafile is valid and loaded. Exiting loop with break";
								metaFileIsValid = true;
								break;
							}
						}
					}
				}
				else
				{
					// MetaFile was loaded successfully
					metaFileIsValid = true;
				}
			}
			userKeyChanged = false;
		}
	}
	while(!metaFileIsValid && state != CANCELLED && state != ERROR);




	// Open Output
	QFile output(outputDir.absoluteFilePath(metaHandler.getName()));
	output.open(QFile::WriteOnly);

	setStateString(tr("Downloading"));
	while(state == RUNNING)
	{
		// Handle all Items in metaFile
		for(qint32 i = 0; metaHandler.exists(i); i++)
		{
			// Download
			if(transmission.download(metaHandler.get(i)))
			{
				// Add downloaded image to list of tempObjects (for cleanup)
				tempFiles.push_back(metaHandler.get(i)->image.file);

				// Read data from downloaded image
				QByteArray hiddenData = stegano.read(metaHandler.get(i)->image);
				if (userKey != "")
					crypto.decrypt(hiddenData, userKey);

				//output.seek(metaHandler.getDataSize(i));
				hiddenData.chop(metaHandler.get(i)->image.padding);
				output.write(hiddenData);
			}
			else
			{
				// ERROR: Download failed!
				setStateString(tr("ERROR: Missing file.") + metaHandler.get(i)->image.file);
				state = ERROR;
				return;
			}

			// Calculate Progress
			if(metaHandler.getDataSize() > 0)
				setProgress(static_cast<qint32>((static_cast<double>(metaHandler.getDataSize(i))/(metaHandler.getDataSize()))*100));

			// Check for cancel-call or errors
			if(state != RUNNING)
				break;
		}

		// Process has finished successfully
		if(state == RUNNING)
		{
			setProgress(100);
			setStateString(tr("Completed"));
			state = COMPLETED;
		}
	}

	// Process is cancelled
	if(state == CANCELLED)
		setStateString(tr("Cancelled"));

	// Close Output
	output.close();

	return;
}




void Process_Download::pipeline(const QString& _metaFile) {
	// TODO iterating download
}

#include "Process_Download.moc"

