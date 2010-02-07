#include "Process_Upload.h"

Process_Upload::Process_Upload(const QString& _jobName, const QString& _file, const QDir& _imageDir, const QDir& _tempDir, const QString& _userKey)
{
	// Process properties
	setJobName(_jobName);

	// File properties
	type = tr("Upload");
	file = _file;
	imageDir = _imageDir;
	tempDir = _tempDir;
	setUserKey(_userKey);

	// State properties
	state = QUEUED;
	setStateString(tr("Queued"));

	metaFileName = "";
}




void Process_Upload::run()
{
	// Initialize
	init();


	// Hide file
	pipeline(file);
	// Write MetaFile
	metaFileName = tempDir.absoluteFilePath(getJobName() + ".xml");
	metaHandler.save(metaFileName);
	// Add new metaFile to list of tempObjects (for cleanup)
	tempFiles.push_back(metaFileName);


	// Hide metaFiles
	do	// Loop until MetaFile is hidden in a single image
	{
		// Hide old MetaFile
		pipeline(metaFileName);

		// Write new MetaFile
		metaFileName = tempDir.absoluteFilePath(getJobName() + "_final");
		metaFileName.append(".xml");
		metaHandler.save(metaFileName);
		// Add new metaFile to list of tempObjects (for cleanup)
		tempFiles.push_back(metaFileName);
	}
	while(metaHandler.exists(1));


	// Process has finished successfully
	if(state == RUNNING)
	{
		setProgress(100);
		setStateString(tr("Completed"));
		state = COMPLETED;

		if(metaHandler.exists(0) && metaHandler.get(0)->remote.link.size() > 0)
		{
			// Display link of uploaded MetaImage
			result = metaHandler.get(0)->remote.link.at(0).url;
			emit displayResult(result, this);
		}
		else
		{
			result = tr("ERROR: No Result given.");
			emit displayResult(result, this);
		}
	}
}




void Process_Upload::pipeline(const QString& _file)
{
	// Clear metaHandler (to allow a clean start)
	metaHandler.clear();
	// Strip filepath information from filename
	metaHandler.setName(_file.section('/', -1));
	metaHandler.setKey(getUserKey());

	// Create Components
	Crypto crypto;
	Chunker chunker(_file, crypto.getBlockSize());
	Stegano stegano(imageDir, tempDir);
	Transmission transmission(tempDir);

	qint32 id = 0;
	qint64 hiddenBytes = 0;

	// Shuffle images in imageDir
 	stegano.shuffleImages();

	// Chunk, encrypt and hide File
	setStateString(tr("Hiding Data"));
	do
	{
		// Create new Item
		Item newItem;

		//Set the number of the Item
		newItem.id = id;

		// Create new Chunk
		QByteArray chunk;
		hiddenBytes = stegano.getCapacity();
		chunk = chunker.getNext(hiddenBytes);

		// Encrypt Chunk if password set
		if (userKey != "")
			crypto.encrypt(chunk, userKey);

		// Write Chunk to Image and store info in tempItem
		newItem.image = stegano.write(chunk);

		//Set the number of the Item
		newItem.id = id;

		// Add new Item to list of Items
		newItem.image.padding = chunker.getPaddedBytes();
		metaHandler.add(newItem);

		// Add new image file to list of tempObjects (for cleanup)
		tempFiles.push_back(newItem.image.file);

		//Increment the number (id) of the next item
		id++;
	}
	while(chunker.hasNext() || state == CANCELLED);

	// Pipline Check
	bool pipeline_done = false;

	// Pipeline Loop: While pipeline is *not* finished
	setStateString(tr("Uploading"));
	while(state == RUNNING && !pipeline_done)
	{
		// Upload Images
		for(qint32 i = 0; metaHandler.exists(i); i++)
		{
			// Upload File
			metaHandler.get(i)->remote = transmission.upload(metaHandler.get(i)->image.file);

			// Calculate Progress
			if(metaHandler.getDataSize() > 0)
				setProgress(static_cast<qint32>((static_cast<double>(metaHandler.getDataSize(i))/(metaHandler.getDataSize()))*100));

			// Check for Cancel call
			if(state == CANCELLED)
				break;
		}

		// Pipeline has finished
		if(state == RUNNING)
			pipeline_done = true;
	}

	// Process is cancelled
	if(state == CANCELLED)
		setStateString(tr("Cancelled"));
}

#include "Process_Upload.moc"

