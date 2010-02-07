#include <QtGui>
#include "DialogAbout.h"

DialogAbout::DialogAbout(QMainWindow *parent)
	:QDialog(parent)
{
	setupUi(this);
	QString applicationName = "HideMyData";
	QString applicationVersion = VERSION;
	QString copyrightYear = (QString)QChar(0x00A9)+"2010";
	QString license = "<a href=http://bla>GPL</a>";
	
	licenseLabel->setOpenExternalLinks(false);
	versionLabel->setText(applicationName + " " + applicationVersion);
	aboutLabel->setText(tr("A filesharing application for distributing safely files"));
	copyrightLabel->setText(copyrightYear + " " + applicationName + " " +tr("Developers"));
	licenseLabel->setText(tr("License:") + " " + license);
	authorLabel->setText("Jerg *. <br /> Gerrit G. <br /> Rashid K. <br /> Andreas L. <br /> Thomas L. <br /> Adrian W.");
	
	//OK-button closes window
	buttonBox->button(QDialogButtonBox::Close)->setText(tr("&Close"));
	connect(buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)),
	this, SLOT(close()));
	
	connect(licenseLabel, SIGNAL(linkActivated(QString)),
		this, SLOT(showLicense()));
}

void DialogAbout::showLicense()
{
	QDialog* licenseWidget = new QDialog(this);
	QVBoxLayout* licenseVerticalLayout = new QVBoxLayout(licenseWidget);
	QPlainTextEdit* licenseView = new QPlainTextEdit(licenseWidget);
	QDialogButtonBox* licenseButton = new QDialogButtonBox(licenseWidget);
	QString licenseText = "bla bla \n moep moep";
	
	licenseVerticalLayout->addWidget(licenseView);
	licenseVerticalLayout->addWidget(licenseButton);
	
	licenseButton->setStandardButtons(QDialogButtonBox::Close);
	connect(licenseButton->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)),
		licenseWidget, SLOT(close()));
	
	licenseView->isReadOnly();
	licenseView->setPlainText(licenseText);
	licenseWidget->setWindowTitle(tr("License"));
	licenseWidget->resize(600, 350);
	
	licenseWidget->show();
}

#include "DialogAbout.moc"
