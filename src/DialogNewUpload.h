#ifndef DIALOGNEWUPLOAD_H
#define DIALOGNEWUPLOAD_H
#include <QMainWindow>
#include <QDir>
#include <QSettings>
#include <QString>
#include "ui_DialogNewUpload.h"
// #include "MainWindow.h"

/*! dialog for a new upload */
class DialogNewUpload : public QDialog, public Ui::DialogNewUpload
{
		Q_OBJECT

	public:
		/*! upload dialog constructor */
		DialogNewUpload(QMainWindow *parent = 0);
		
		void setSettings(QSettings* _settings);
		void setFile(QString _file);
		
	public slots:
		void reset();

	private slots:
		/*! creates an upload */
		void createUpload();
		
		/*! toggles password echo */
		void togglePasswordEcho(bool _toggle);
		
		/*! selects a file to upload */
		void selectFile();
		
		/*! selects an image folder */
		void selectImageFolder();
		
		/*! selects folder wherein temporary files will be written */
		void selectTempDirFolder();
		
	private:
		/*! checks if all required dialog fields are filled */
		bool uploadCondition();
		
		/*! key events e.g. for creating a upload with return key */
		virtual void keyPressEvent(QKeyEvent* _key);
		
		QSettings* settings;

	signals:
		/*! passes the gathered data to the main window */
		void createNewUpload(QString _file, QDir _imageDir, QDir _tempDir, QString _password, QString _jobName);
};

#endif //DIALOGNEWUPLOAD_H
