#include <QtGui>
#include <QFileDialog>
#include <QUrl>
#include "DialogNewUpload.h"
#include <QDebug>

DialogNewUpload::DialogNewUpload(QMainWindow *parent)
		: QDialog(parent)
{
	setupUi(this);
	
	buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));
	buttonBox->button(QDialogButtonBox::Ok)->setText(tr("&Ok"));
	fileOpen->button(QDialogButtonBox::Open)->setText(tr("Open"));
	tempDirOpen->button(QDialogButtonBox::Open)->setText(tr("Open"));
	imageDirOpen->button(QDialogButtonBox::Open)->setText(tr("Open"));

	//creates an upload
	connect(buttonBox, SIGNAL(accepted()),
		  this, SLOT(createUpload()));

	//closes dialog
	connect(buttonBox, SIGNAL(rejected()),
		  this, SLOT(close()));

	//passes upload information to main window
	connect(this, SIGNAL(createNewUpload(QString, QDir, QDir, QString, QString)),
		  parent, SLOT(createUploadProcess(QString, QDir, QDir, QString, QString)));

	//toggles password echo
	connect(showPWcheck, SIGNAL(toggled(bool)),
		  this, SLOT(togglePasswordEcho(bool)));

	//selects file for upload
	connect(fileOpen, SIGNAL(accepted()),
		  this, SLOT(selectFile()));

	//selects image folder
	connect(imageDirOpen, SIGNAL(accepted()),
		  this, SLOT(selectImageFolder()));

	//selects temporary folder
	connect(tempDirOpen, SIGNAL(accepted()),
		  this, SLOT(selectTempDirFolder()));

	encryptCheck->toggle();
	collectionCheck->toggle();
}

void DialogNewUpload::setSettings(QSettings* _settings)
{
	settings = _settings;

	tempDirLine->setText(_settings->value("Default/TempDir", "").value<QString>());
		
	collectionCheck->setChecked(_settings->value("Default/use_CollectionDir", false).value<bool>());

	imageDirLabel->setHidden(_settings->value("Default/use_CollectionDir", false).value<bool>());
	imageDirLine->setHidden(_settings->value("Default/use_CollectionDir", false).value<bool>());
	imageDirOpen->setHidden(_settings->value("Default/use_CollectionDir", false).value<bool>());
	
}

void DialogNewUpload::setFile(QString _file)
{
	fileLine->setText(_file);
}

void DialogNewUpload::reset()
{
	jobLine->setText("");
	fileLine->setText("");
	tempDirLine->setText("");
	collectionCheck->setChecked(false);
	imageDirLine->setText("");
	passwordLine->setText("");
	encryptCheck->setChecked(false);
	showPWcheck->setChecked(false);
	collectionCheck->setHidden(false);
	imageDirLabel->setHidden(false);
	imageDirLine->setHidden(false);
	imageDirOpen->setHidden(false);
}

void DialogNewUpload::createUpload()
{
	//if no jobname is set
	if(jobLine->text().isEmpty())
		jobLine->setText(fileLine->text().section('/', -1));
	
	//if path names are correct
	if (uploadCondition())
	{
		qDebug() << "DialogNewUpload: create upload";
		//if collection is not used

		if (collectionCheck->isChecked() == false)
		{
			//if encryption is disabled
			if (encryptCheck->isChecked() == false)
			{
				qDebug() << "DialogNewUpload: no collection, no password";
				emit createNewUpload((QString) fileLine->text(),
							   (QDir) imageDirLine->text(),
							   (QDir) tempDirLine->text(),
							   (QString) "", //empty password
							   jobLine->text());
				reset();
				close();
			}
			else
			{
				qDebug() << "DialogNewUpload: no collection, password";
				emit createNewUpload((QString) fileLine->text(),
							   (QDir) imageDirLine->text(),
							   (QDir) tempDirLine->text(),
							   (QString) passwordLine->text(),
							   jobLine->text());
				reset();
				close();
			}
		}
		else
		{
			//if encryption is disabled
			if (encryptCheck->isChecked() == false)
			{
				qDebug() << "DialogNewUpload: collection, no password";
				emit createNewUpload((QString) fileLine->text(),
							   (QDir) settings->value("Default/CollectionDir", "").value<QString>(),
							   (QDir) tempDirLine->text(),
							   (QString) "", //empty password
							   jobLine->text());
				reset();
				close();
			}
			else
			{
				qDebug() << "DialogNewUpload: collection, password";
				emit createNewUpload((QString) fileLine->text(),
							   (QDir) settings->value("Default/CollectionDir", "").value<QString>(),
							   (QDir) tempDirLine->text(),
							   (QString) passwordLine->text(),
							   jobLine->text());
				reset();
				close();
			}
		}
	}
	else
	{
		qDebug() << "DialogNewUpload: no upload";
	}
}

void DialogNewUpload::togglePasswordEcho(bool _toggle)
{
	switch (_toggle)
	{
		case true:
			passwordLine->setEchoMode(QLineEdit::Normal);
			break;

		case false:
			passwordLine->setEchoMode(QLineEdit::Password);
			break;
	}
}

void DialogNewUpload::selectFile()
{
	QString oldJobName = jobLine->text();
	QString oldFileName = fileLine->text();

	QString temp(QFileDialog::getOpenFileName(this,
			 tr("Select File"),
			 "",
			 tr("File (*)")));

	if (!temp.isEmpty())
		fileLine->setText(temp);

	if (jobLine->text().isEmpty())
		jobLine->setText(fileLine->text().section('/', -1));

	if (oldJobName == oldFileName.section('/', -1))
		jobLine->setText(fileLine->text().section('/', -1));
}

void DialogNewUpload::selectImageFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		imageDirLine->setText(temp);
}

void DialogNewUpload::selectTempDirFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		tempDirLine->setText(temp);
}

bool DialogNewUpload::uploadCondition()
{
	QString errorMessage;
	bool noError = true;
	
	//File
	if (fileLine->text().isEmpty())
	{
		noError = false;
		errorMessage.append(tr("* File is not set\n"));
		
	}
	else if(!fileLine->text().isEmpty())
	{
		if (!((QFile)fileLine->text()).exists())
		{
			//TODO better regexp for urls
// 			bool isUrl = fileLine->text().contains(QRegExp("^http://*", Qt::CaseSensitive, QRegExp::RegExp2));
// 			qDebug() << "DialogNewUpload: isUrl? " << isUrl;
// 			if(!isUrl)
// 			{
				noError = false;
				errorMessage.append(tr("* File does not exist\n"));
				
// 			}
		}
	}

	//temporary Directory
	if (tempDirLine->text().isEmpty())
	{
		noError = false;
		errorMessage.append(tr("* Temporary Directory is not set\n"));
	}
	else if (!((QDir)tempDirLine->text()).exists())
	{
		noError = false;
		errorMessage.append(tr("* Temporary Directory does not exist\n"));
	}

	//image Folder or Collection
	if (!collectionCheck->isChecked())
	{
		if (imageDirLine->text().isEmpty())
		{
			noError = false;
			errorMessage.append(tr("* Image Directory is not set\n"));
		}
		else if (!((QDir)imageDirLine->text()).exists())
		{
			noError = false;
			errorMessage.append(tr("* Image Directory does not exist\n"));
		}
	}
	if(collectionCheck->isChecked())
	{
	
		if(settings->value("Default/CollectionDir","").value<QString>().isEmpty())
		{
			noError = false;
			errorMessage.append(tr("* Collection Directory is not set\n"));
		} 
		else if(!((QDir)settings->value("Default/CollectionDir", "").value<QString>()).exists() & !settings->value("Default/CollectionDir","").value<QString>().isEmpty())
		{
			
			noError = false;
			errorMessage.append(tr("* Collection Directory does not exist\n"));
		}
	}
	
	//password
	if(encryptCheck->isChecked())
	{
		if(passwordLine->text().isEmpty())
		{
			noError = false;
			errorMessage.append(tr("* Password is not set\n"));
		}
		else if(passwordLine->text().size() < 3)
		{
			noError = false;
			errorMessage.append(tr("* Password too short (3 Characters minimum)\n"));
		}
	}

	if(noError == false)
	{
		QMessageBox errorBox;
		errorBox.setText(tr("Input Error"));
		errorBox.setWindowTitle(tr("Upload Error"));
		errorBox.setInformativeText(errorMessage);
		errorBox.setIcon(QMessageBox::Critical);
		errorBox.exec();
	}

	return noError;
}

void DialogNewUpload::keyPressEvent(QKeyEvent* _key)
{
	if (_key->key() == Qt::Key_Return) 
	{  
		createUpload();
	} 
	if(_key->key() == Qt::Key_Escape)
	{
		close();
	}
}
#include "DialogNewUpload.moc"
