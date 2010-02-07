#include <QtGui>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include "DialogSettings.h"

DialogSettings::DialogSettings(QMainWindow *parent)
	:QDialog(parent)
{
	setupUi(this);
	
	buttonBox->button(QDialogButtonBox::Apply)->setText(tr("&Apply"));
	buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));
	buttonBox->button(QDialogButtonBox::Ok)->setText(tr("&Ok"));
	tempDirButton->button(QDialogButtonBox::Open)->setText(tr("Open"));
	destDirButton->button(QDialogButtonBox::Open)->setText(tr("Open"));
	collectionDirButton->button(QDialogButtonBox::Open)->setText(tr("Open"));
	
	connect(buttonBox, SIGNAL(accepted()),
		this, SLOT(save()));

	connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), 
		this, SLOT(apply()));
	
	//cancelButton quits Dialog
	connect(buttonBox, SIGNAL(rejected()),
		this, SLOT(close()));

	connect(tempDirButton, SIGNAL(accepted()),
		this, SLOT(selectTempDirFolder()));
		
	connect(destDirButton, SIGNAL(accepted()),
		this, SLOT(selectDestinationFolder()));
		
	connect(collectionDirButton, SIGNAL(accepted()),
		this, SLOT(selectCollectionFolder()));
	
	loadLanguages();
}

DialogSettings::~DialogSettings()
{

}

void DialogSettings::setSettings(QSettings* _settings)
{
	settings  = _settings;
	
	settingsCheck->setChecked(settings->value("Default/use_settings",false).value<bool>());
	
	destDirCheck->setChecked(settings->value("Default/use_DestinationDir", false).value<bool>());
	destDirLine->setText(settings->value("Default/DestinationDir", "").value<QString>());
	
	collectionDirCheck->setChecked(settings->value("Default/use_CollectionDir", false).value<bool>());
	collectionDirLine->setText(settings->value("Default/CollectionDir", "").value<QString>());
	
	tempDirCheck->setChecked(settings->value("Default/use_TempDir").value<bool>());
	tempDirLine->setText(settings->value("Default/TempDir").value<QString>());
	
	resultdialogCheck->setChecked(settings->value("Default/hide_ResultDialog", false).value<bool>());
	
	languageCombo->setCurrentIndex(languageCombo->findData(settings->value("Default/Language", "en")));
}

bool DialogSettings::check()
{
	bool noError = true;
	QString errorMessage;
	
	if(destDirCheck->isChecked())
		if(!((QDir)(destDirLine->text())).exists() | destDirLine->text().isEmpty())
		{
			if(!destDirLine->text().isEmpty())
			{
				noError = false;
				errorMessage.append(tr("* Destination Directory does not exist\n"));
			}else
			{
				noError = false;
				errorMessage.append(tr("* Destination Directory is not set\n"));
			}
		}
	
	if(tempDirCheck->isChecked())
		if(!((QDir)(tempDirLine->text())).exists() | tempDirLine->text().isEmpty())
		{
			if(!tempDirLine->text().isEmpty())
			{
				noError = false;
				errorMessage.append(tr("* Temporary Directory does not exist\n"));
			}else
			{
				noError = false;
				errorMessage.append(tr("* Temporary Directory is not set\n"));
			}
		}
		
	if(collectionDirCheck->isChecked())
		if(!((QDir)(collectionDirLine->text())).exists() | collectionDirLine->text().isEmpty())
		{
			if(!collectionDirLine->text().isEmpty())
			{
				noError = false;
				errorMessage.append(tr("* Collection Directory does not exist\n"));
			}else
			{
				noError = false;
				errorMessage.append(tr("* Collection Directory is not set\n"));
			}
		}
		
	if(noError == false)
	{
		QMessageBox errorBox;
		errorBox.setText(tr("Input Error"));
		errorBox.setWindowTitle(tr("Settings Error"));
		errorBox.setInformativeText(errorMessage);
		errorBox.setIcon(QMessageBox::Critical);
		errorBox.exec();
	}
	return noError;
}

void DialogSettings::loadLanguages()
{
	QString languagePath = qApp->applicationDirPath();
	QDir languageDir(languagePath);
	languages = languageDir.entryList(QStringList("hidemydata_*.qm"));

	//fill language combobox
	qDebug() << "Add Language: " << "\"English\" " << "\"en\"";
	languageCombo->addItem("English", "en");
	for(int i = 0; i < languages.size(); i++)
	{
		//get locale from file name
		QString locale = languages.at(i);
		locale.remove(0, locale.indexOf('_') + 1);
		locale.truncate(locale.lastIndexOf('.'));
		
		//load actual language file
		translator.load(languages.at(i), languagePath);
	
		//get language name
		QString language = translator.translate("MainWindow", "English");
		
		qDebug() << "Add Language: " << language.append(" ") << locale;
		languageCombo->addItem(language, locale);
	}
}

void DialogSettings::save()
{
	if(check())
	{
		apply();
		close();
	}
}

void DialogSettings::apply()
{
	if(settingsCheck->isChecked() == true)
	{
		int ret = QMessageBox::Cancel;
		
		if(!settings->value("Default/use_Settings", false).value<bool>())
		{
			QMessageBox settingsBox;
			settingsBox.setWindowTitle("Settings");
			settingsBox.setText(tr("Are you sure saving settings?"));
			settingsBox.setInformativeText(tr("Saving settings will leave traces on the computer."));
			settingsBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			settingsBox.button(QMessageBox::Yes)->setText(tr("&Yes"));
			settingsBox.button(QMessageBox::No)->setText(tr("&No"));
			settingsBox.setIcon(QMessageBox::Question);
			ret = settingsBox.exec();
			
// 			QString oldlocale = settings->value("Default/Language", "en").value<QString>();
// 			QString newlocale = languageCombo->itemData(languageCombo->currentIndex()).value<QString>();
// 			if(oldlocale != newlocale)
// 			{
// 				translator.load("hidemydata_" + newlocale, qApp->applicationDirPath());
// 				qDebug() << "DialogSettings: Retranslate UI";
// 				retranslateUi(this);
// 			}
		}
		else
			ret = QMessageBox::Yes;
		
		if(ret == QMessageBox::Yes)
			if(check())
			{
				settings->setValue("Default/use_Settings", true);
				settings->setValue("Default/use_DestinationDir", destDirCheck->isChecked());
				settings->setValue("Default/DestinationDir", destDirLine->text());
	
				settings->setValue("Default/use_CollectionDir", collectionDirCheck->isChecked());
				settings->setValue("Default/CollectionDir", collectionDirLine->text());
	
				settings->setValue("Default/use_TempDir", tempDirCheck->isChecked());
				settings->setValue("Default/TempDir", tempDirLine->text());
				
				settings->setValue("Default/hide_ResultDialog", resultdialogCheck->isChecked());
				
				QString oldlocale = settings->value("Default/Language", "en").value<QString>();
				QString newlocale = languageCombo->itemData(languageCombo->currentIndex()).value<QString>();
				if(oldlocale != newlocale)
				{
					settings->setValue("Default/Language", languageCombo->itemData(languageCombo->currentIndex()));
					translator.load("hidemydata_" + newlocale, qApp->applicationDirPath());
					qDebug() << "DialogSettings: Retranslate UI";
					retranslateUi(this);
				}
			}
	}
	else
		settings->clear();
	
}

void DialogSettings::selectDestinationFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		destDirLine->setText(temp);
}

void DialogSettings::selectCollectionFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		collectionDirLine->setText(temp);
}

void DialogSettings::selectTempDirFolder()
{
	QString temp(QFileDialog::getExistingDirectory(this,
			 tr("Select Directory"),
			 "",
			 QFileDialog::ShowDirsOnly));

	if (!temp.isEmpty())
		tempDirLine->setText(temp);
}

#include "DialogSettings.moc"