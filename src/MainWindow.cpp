#include <QtGui>
#include <QApplication>
#include "MainWindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QMainWindow *parent)
		: QMainWindow(parent)
{
	setupUi(this);
	
	//for translation purposes
	QString dummy_language = tr("English", "Language from language file");
	
	readSettings();
	
	//init Process List View
	processList = new ProcessListView(centralwidget);
	processList->setSettings(settings);
	verticalLayout->addWidget(processList);

	settingsDialog = new DialogSettings(this);
	downloadDialog = new DialogNewDownload(this);
	uploadDialog = new DialogNewUpload(this);
	aboutDialog = new DialogAbout(this);
	
	initConnections();
	retranslateUi(this);

//	display disclaimer
	if(settings->value("Default/Disclaimer_accepted", false).value<bool>() == false)
	{
		qDebug() << "MainWindow: Show Disclaimer";
		QMessageBox disclaimer(this);
		disclaimer.setWindowTitle(tr("Disclaimer"));
		disclaimer.setText(tr("Legal Information"));
		disclaimer.setInformativeText(tr("The authors of this program don't take the responsibilty for your actions"));
		disclaimer.addButton(tr("&Ok"),QMessageBox::YesRole);
/*		disclaimer.addButton(tr("Oh &Noes"),QMessageBox::NoRole);
		connect(disclaimer.buttons().at(1), SIGNAL(clicked(bool)), 
			this, SLOT(close()));
*/
		disclaimer.setIcon(QMessageBox::Question);
		int ret = disclaimer.exec();
		
		switch(ret)
		{
			//yes
			case 0:
				qDebug() << "MainWindow: Accept Disclaimer";
				if(settings->value("Default/use_Settings", false).value<bool>())
					settings->setValue("Default/Disclaimer_accepted", true);
				break;
				
			//no
			case 1:
				qDebug() << "MainWindow: Reject Disclaimer";
				qApp->quit();
				break;
		}
	}
}

void MainWindow::showNewDownloadDialog()
{
	downloadDialog->reset();
	downloadDialog->setSettings(settings);
	downloadDialog->show();
	downloadDialog->activateWindow();
}

void MainWindow::showNewDownloadDialog(QString _metafile)
{
	downloadDialog->reset();
	downloadDialog->setSettings(settings);
	downloadDialog->setMetafile(_metafile);
	downloadDialog->show();
	downloadDialog->activateWindow();
}

void MainWindow::showNewUploadDialog()
{
	uploadDialog->reset();
	uploadDialog->setSettings(settings);
	uploadDialog->show();
	uploadDialog->activateWindow();
}

void MainWindow::showNewUploadDialog(QString _file)
{
	uploadDialog->reset();
	uploadDialog->setSettings(settings);
	uploadDialog->setFile(_file);
	uploadDialog->show();
	uploadDialog->activateWindow();
}

void MainWindow::showAboutDialog()
{
	aboutDialog->show();
	aboutDialog->activateWindow();
}

void MainWindow::showAboutQtDialog()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::showSettingsDialog()
{
	settingsDialog->setSettings(settings);
	settingsDialog->show();
	settingsDialog->activateWindow();
}

void MainWindow::createDownloadProcess(QString _metaFile, QDir _destination, QDir _tempDir, QString _password, QString _jobName)
{
	qDebug()<<"MainWindow: Create download process";
	processList->createDownloadProcess(_metaFile, _destination, _tempDir, _password, _jobName);
}

void MainWindow::createUploadProcess(QString _file, QDir _imageDir, QDir _tempDir, QString _password, QString _jobName)
{
	qDebug()<<"MainWindow: Create upload process";
	processList->createUploadProcess(_file, _imageDir, _tempDir,  _password, _jobName);
}

void MainWindow::readSettings()
{
// 	QCoreApplication::setOrganizationName("MySoft");
// 	QCoreApplication::setOrganizationDomain("mysoft.com");
	QCoreApplication::setApplicationName("Hide My Data");
	
	//translation
	translator.load("hidemydata_" + QLocale::system().name(), qApp->applicationDirPath());
	qApp->installTranslator(&translator);
	
	settings = new QSettings();
	if(settings->value("Default/use_Settings", false).value<bool>())
	{
		//language
		translator.load("hidemydata_" + settings->value("Default/Language", "en").value<QString>(), qApp->applicationDirPath());
		qApp->installTranslator(&translator);
	}
}

void MainWindow::writeSettings()
{

}

void MainWindow::initConnections()
{
	//opens About Dialog
	connect(actionAbout, SIGNAL(triggered(bool)),
	        this, SLOT(showAboutDialog()));
		
	//opens About Qt Dialog
	connect(actionAbout_Qt, SIGNAL(triggered(bool)),
		this, SLOT(showAboutQtDialog()));

	//quits Programme
	connect(action_Quit, SIGNAL(triggered(bool)),
	        this, SLOT(close()));

	//opens New Download Dialog
	connect(actionNew_Download, SIGNAL(triggered(bool)),
	        this, SLOT(showNewDownloadDialog()));

	//opens New Upload Dialog
	connect(actionNew_Upload, SIGNAL(triggered(bool)),
	        this, SLOT(showNewUploadDialog()));

	//opens Settings Dialog
	connect(action_Settings, SIGNAL(triggered(bool)),
	        this, SLOT(showSettingsDialog()));
		
	//starts all processes
	connect(actionStart, SIGNAL(triggered(bool)),
		processList, SLOT(startAllProcesses()));
		
	//cancel all processes
	connect(actionRemove_All, SIGNAL(triggered(bool)),
		processList, SLOT(removeAllProcesses()));
		
	//start selected processes
	connect(actionStart_selected, SIGNAL(triggered(bool)),
		processList, SLOT(start()));
		
	//remove selected process
	connect(actionRemove_selected, SIGNAL(triggered(bool)),
		processList, SLOT(remove()));
		
	//cancel selected process
	connect(action_Cancel, SIGNAL(triggered(bool)),
		processList, SLOT(cancel()));
		
	//remove completed processes
	connect(actionRemove_Completed, SIGNAL(triggered(bool)),
		processList, SLOT(removeCompletedProcesses()));
		
	//dropped file upload
	connect(processList, SIGNAL(createUpload(QString)),
		this, SLOT(showNewUploadDialog(QString)));
		
	//dropped url
	connect(processList, SIGNAL(createDownload(QString)),
		this, SLOT(showNewDownloadDialog(QString)));
		
	//set queue mode for processes
	connect(actionQueue, SIGNAL(toggled(bool)),
		processList, SLOT(setQueue(bool)));
}

void MainWindow::closeEvent(QCloseEvent* _event)
{
	qDebug() << "MainWindow: Completed " << processList->getCompletedCount() << "/" << processList->getProcessCount();
	if(processList->getProcessCount() != processList->getCompletedCount())
	{
		QMessageBox quitBox;
		quitBox.setWindowTitle(tr("Quit"));
		quitBox.setText(tr("Are you sure to quit?"));
		quitBox.setInformativeText(tr("Quitting this application leads to possible data loss while jobs are running."));
		quitBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		quitBox.button(QMessageBox::Yes)->setText(tr("&Yes"));
		quitBox.button(QMessageBox::No)->setText(tr("&No"));
		quitBox.setIcon(QMessageBox::Question);
		int ret = quitBox.exec();
		
		switch(ret)
		{
			case QMessageBox::Yes:
				writeSettings();
				processList->removeAllProcesses();
				_event->accept();
				break;
			case QMessageBox::No:
				_event->ignore();
				break;
		}
	}
	else
	{
		processList->removeAllProcesses();
		_event->accept();
	}
}

#include "MainWindow.moc"

