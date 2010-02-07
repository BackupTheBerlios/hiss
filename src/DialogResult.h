#ifndef DIALOGRESULT_H
#define DIALOGRESULT_H

#include <QWidget>
#include "ui_DialogResult.h"
#include "Process.h"

class DialogResult : public QDialog, public Ui::DialogResult
{
		Q_OBJECT
	public:
		DialogResult(QWidget* _parent, QString _url, Process* _process);
		~DialogResult();
		
	private slots:
		void toClipboard();
};

#endif
