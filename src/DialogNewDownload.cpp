#include <QtGui>
#include <QFileDialog>
#include "DialogNewDownload.h"
#include <QDebug>

DialogNewDownload::DialogNewDownload(QMainWindow *parent)
		: QDialog(parent)
{
	setupUi(this);

	buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));
	buttonBox->button(QDialogButtonBox::Ok)->setText(tr("&Ok"));
	metafileOpen->button(QDialogButtonBox::Open)->setText(tr("Open"));
	destinationOpen->button(QDialogButtonBox::Open)->setText(tr("Open"));
	tempDirOpen->button(QDialogButtonBox::Open)->setText(tr("Open"));
	
	// OK-Button creates new Process
	connect(buttonBox, SIGNAL(accepted()),
		  this, SLOT(createDownload()));

	//closes dialog
	connect(buttonBox, SIGNAL(rejected()),
		  this, SLOT(close()));

	//passes download information to main window
	connect(this, SIGNAL(createNewDownload(QString, QDir, QDir, QString, QString)),
		  parent, SLOT(createDownloadProcess(QString, QDir, QDir, QString, QString)));

	//toggles password echo
	connect(showPWcheck, SIGNAL(toggled(bool)),
		  this, SLOT(togglePasswordEcho(bool)));

	//selects metafile
	connect(metafileOpen, SIGNAL(accepted()),
		  this, SLOT(selectMetafile()));

	//selects destination folder
	connect(destinationOpen, SIGNAL(accepted()),
		  this, SLOT(selectDestinationFolder()));

	//selects temporary folder
	connect(tempDirOpen, SIGNAL(accepted()),
		  this, SLOT(selectTempDirFolder()));
		  
	decryptCheck->toggle();
}

void DialogNewDownload::setSettings(QSettings* _settings)
{
	if(_settings->value("Default/use_TempDir", false).value<bool>())
		tempDirLine->setText(_settings->value("Default/TempDir", "").value<QString>());

	if(_settings->value("Default/use_DestinationDir", "").value<bool>())
		destinationLine->setText(_settings->value("Default/DestinationDir", "").value<QString>());
}

void DialogNewDownload::setMetafile(QString _metafile)
{
	metafileLine->setText(_metafile);
}

void DialogNewDownload::reset()
{
	jobLine->setText("");
	metafileLine->setText("");
	tempDirLine->setText("");
	destinationLine->setText("");
	passwordLine->setText("");
	showPWcheck->setChecked(false);
	decryptCheck->setChecked(false);
}

void DialogNewDownload::createDownload()
{
	//if no jobname is set
	if(jobLine->text().isEmpty())
		jobLine->setText(metafileLine->text().section('/', -1));
	
	if (downloadCondition())
	{
		qDebug() << "DialogNewDownload: create download";
		//if metafile is encrypted
		if (decryptCheck->isChecked() == true)
		{
			qDebug() << "DialogNewDownload: decrypt";
			emit createNewDownload((QString)metafileLine->text(),
						     (QDir)destinationLine->text(),
						     (QDir)tempDirLine->text(),
						     passwordLine->text(),
						     jobLine->text());
			reset();
			close();
		}
		else
		{
			qDebug() << "DialogNewDownload: nothing to decrypt";
			emit createNewDownload((QString)metafileLine->text(),
						     (QDir)destinationLine->text(),
						     (QDir)tempDirLine->text(),
						     (QString)"",
					             jobLine->text());
			reset();
			close();
		}
	}
	else
	{
		qDebug() << "DialogNewDownload: no download";

	}
}

void DialogNewDownload::togglePasswordEcho(bool _toggle)
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

void DialogNewDownload::selectMetafile()
{
	QString oldJobName = jobLine->text();
	QString oldMetafileName = metafileLine->text();
	
	QString temp(QFileDialog::getOpenFileName(this,
			 tr("Select Metafile"),
			 "",
			 tr("Metafiles (*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG *.bmp *.BMP *.xml);; All Files (*)")));

	if (!temp.isEmpty())
		metafileLine->setText(temp);
	
	if(jobLine->text().isEmpty())
		jobLine->setText(metafileLine->text().section('/', -1));
	if(oldJobName == oldMetafileName.section('/', -1))
		jobLine->setText(metafileLine->text().section('/', -1));
}

void DialogNewDownload::selectDestinationFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		destinationLine->setText(temp);
}

void DialogNewDownload::selectTempDirFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		tempDirLine->setText(temp);
}

bool DialogNewDownload::downloadCondition()
{		 
	bool noError = true;
	QString errorMessage;
	
	//Metafile
	if (metafileLine->text().isEmpty())
	{
		noError = false;
		errorMessage.append(tr("* Meta File is not set\n"));
	}else if (!metafileLine->text().isEmpty())
	{
		if (!((QFile)metafileLine->text()).exists())
		{
			//TODO better regexp for urls
			bool isUrl = metafileLine->text().contains(QRegExp("^http://*", Qt::CaseSensitive, QRegExp::RegExp2));
			qDebug() << "DialogNewUpload: isUrl? " << isUrl;
			if(!isUrl)
			{
				noError = false;
				errorMessage.append(tr("* Meta File does not exist\n"));
			}
		}
	}
	
	//Destination Directory
	if (destinationLine->text().isEmpty())
	{
		noError = false;
		errorMessage.append(tr("* Destination Directory is not set\n"));
	}
	else if (!((QDir)destinationLine->text()).exists())
	{
		noError = false;
		errorMessage.append(tr("* Destination Directory does not exist\n"));
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
	
	//password
	if(decryptCheck->isChecked())
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
		errorBox.setWindowTitle(tr("Download Error"));
		errorBox.setInformativeText(errorMessage);
		errorBox.setIcon(QMessageBox::Critical);
		errorBox.exec();
	}
	return noError;
}

void DialogNewDownload::keyPressEvent(QKeyEvent* _key)
{
	if (_key->key() == Qt::Key_Return) 
	{  
		createDownload();
	} 
}
#include "DialogNewDownload.moc"
