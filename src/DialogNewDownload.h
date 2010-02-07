#ifndef DIALOGNEWDOWNLOAD_H
#define DIALOGNEWDOWNLOAD_H
#include <QMainWindow>
#include <QDir>
#include <QString>
#include <QSettings>
#include "ui_DialogNewDownload.h"
// #include "MainWindow.h"

/*! dialog for a new download */
class DialogNewDownload : public QDialog, public Ui::DialogNewDownload
{
		Q_OBJECT

	public:
		/*! new download constructor */
		DialogNewDownload ( QMainWindow *parent = 0 );
		
		void setSettings(QSettings* _settings);
		void setMetafile(QString _metafile);
		
	public slots:
		void reset();
	
	private slots:
		/*! creates a new Download */
		void createDownload();
		
		/*! toggles password echo */
		void togglePasswordEcho(bool _toggle);

		/*! selects metafile and opens a File */
		void selectMetafile();
		
		/*! selects folder wherein the hidden file will be written */
		void selectDestinationFolder();
		
		/*! selects folder wherein temporary files will be written */
		void selectTempDirFolder();
	
	 private:
		/*! checks if all required dialog fields are filled */
		bool downloadCondition();
		
		/*! key events e.g. for creating a download with return key */
		virtual void keyPressEvent(QKeyEvent* _key);
		
	signals:
		/*! passes the gathered data to the main window */
		void createNewDownload(QString _metaFile, QDir _destination, QDir _tempDir, QString _password, QString _jobName);
};

#endif //DIALOGNEWDOWNLOAD_H
