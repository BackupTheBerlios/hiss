#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H
#include <QMainWindow>
#include <QSettings>
#include <QTranslator>
#include "ui_DialogSettings.h"

class DialogSettings : public QDialog, public Ui::DialogSettings
{
		Q_OBJECT

	public:
		DialogSettings ( QMainWindow *parent = 0 );
		~DialogSettings();
		
		void setSettings(QSettings* _settings);
		
		
	public slots:
		void save();
		void apply();
		
	private slots:
		void selectDestinationFolder();
		void selectTempDirFolder();
		void selectCollectionFolder();
		
	signals:
		void writeSettings();
		
	private:
		void loadLanguages();
		bool check();
		
		QSettings* settings;
		QTranslator translator;
		QStringList languages;
};

#endif //DIALOGSETTINGS_H