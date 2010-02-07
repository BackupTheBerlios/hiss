#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDir>
#include <QSettings>
#include <QTranslator>

#include "ui_MainWindow.h"
#include "DialogNewDownload.h"
#include "DialogNewUpload.h"
#include "DialogAbout.h"
#include "DialogSettings.h"

#include "ProcessListView.h"


/*! main application window */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
		Q_OBJECT

	public:
		/*! Main Window constructor */
		MainWindow(QMainWindow *parent = 0);

	public slots:
		/*! shows new download dialog */
		void showNewDownloadDialog();
		void showNewDownloadDialog(QString _metafile);
		
		/*! shows new upload dialog */
		void showNewUploadDialog();
		void showNewUploadDialog(QString _file);
		
		/*! shows about dialog */
		void showAboutDialog();
		
		/*! shows about qt dialog */
		void showAboutQtDialog();
		
		/*! shows settings dialog */
		void showSettingsDialog();
		
		/*! creates a new download process */
		void createDownloadProcess(QString _metaFile, QDir _destination, QDir _tempDir, QString _password, QString _jobName);
		
		/*! creates new upload process */
		void createUploadProcess(QString _file, QDir _imageDir, QDir _tempDir, QString _password, QString _jobName);
		
	private slots:
		void readSettings();
		void writeSettings();
	private:
		/*! setups connections to menu and toolbar actions */
		void initConnections();
		void closeEvent(QCloseEvent* _event);
		
		DialogAbout *aboutDialog;
		DialogSettings *settingsDialog;
		DialogNewDownload *downloadDialog;
		DialogNewUpload *uploadDialog;
		
		ProcessListView *processList;
		
		QSettings* settings;
		QTranslator translator;
};

#endif //MAINWINDOW_H
