#include "DialogResult.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QClipboard>
#include <QApplication>

DialogResult::DialogResult(QWidget* _parent, QString _url, Process* _process) : QDialog(_parent)
{
	setupUi(this);
	setWindowTitle(tr("Result of ") + _process->getJobName());
	resultLine->setText(_url);
	jobnameLabel->setText(_process->getJobName());
	buttonBox->button(QDialogButtonBox::Save)->setText(tr("&Copy to Clipboard"));
	buttonBox->button(QDialogButtonBox::Close)->setText(tr("Cl&ose"));
	
	buttonBox->button(QDialogButtonBox::Save)->disconnect();
	connect(buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)),
		this, SLOT(close()));
	connect(buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked(bool)),
		this, SLOT(toClipboard()));
	
}

DialogResult::~DialogResult()
{
}

void DialogResult::toClipboard()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(resultLine->text());
}
#include "DialogResult.moc"