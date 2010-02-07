#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H
#include <QMainWindow>
#include "ui_DialogAbout.h"

class DialogAbout : public QDialog, public Ui::DialogAbout
{
		Q_OBJECT

	public:
		DialogAbout ( QMainWindow *parent = 0 );
		
	private slots:
		void showLicense();
};

#endif //DIALOGABOUT_H
