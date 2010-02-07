#include <Python.h>
#include <QApplication>
#include "MainWindow.h"
#include <iostream>
#include <stdio.h>
#include "Transmission.h"

void initPythonModules(const char* directory);
using namespace std;


int main(int argc, char** argv)
{
	initPythonModules(argv[0]);
	QApplication app(argc, argv);
	MainWindow *mainwin = new MainWindow;
	mainwin->show();
	return app.exec();
}

void initPythonModules(const char* directory)
{
	QString temp(directory);
	//QDir moduledir = QDir::current();
	QDir moduledir(temp);
	moduledir.cd("../../hostmodule");

	QString tempString;
	QString pathString = (moduledir.absolutePath().toStdString()).c_str();
	
	//tempString.append("\")");
	//tempString.prepend("sys.path.append(\"");
	//tempString = (string()+"import sys\nimport os \nif os.path.exists(\""+moduledir.absolutePath().toStdString()+"/hostmodule"+"\"):\n\tprint '"+ moduledir.absolutePath().toStdString()+"/hostmodule" +" exists'\n\tsys.path.append(\"" +moduledir.absolutePath().toStdString()+"/hostmodule"+"\")\n\nelse:\n\traise(StandardError(\""+moduledir.absolutePath().toStdString()+"/hostmodule"+" does not exist\"))").c_str();
	tempString = (string()+"import sys\nimport os \nif os.path.exists(\""+pathString.toStdString()+"\"):\n\tprint '"+ pathString.toStdString() +" exists'\n\tsys.path.append(\"" + pathString.toStdString() +"\")\n\nelse:\n\traise(StandardError(\"" + pathString.toStdString() + " does not exist\"))").c_str();

	//cout << endl << tempString.toStdString() << endl;
	const char* modulepathchar = tempString.toLatin1().data();

	Py_Initialize();
	//PyRun_SimpleString("import sys");

	//cout << "Return from .sys.path.append: " <<  PyRun_SimpleString("import os \nif os.path.exists(\"/home/ubuntu/git/release/Xteamprojekt/hostmodule\"):\n\tprint 'exist'\n\tsys.path.append(\"/home/ubuntu/git/release/teamprojekt/hostmoduleYZU\")\n\nelse:\n\traise(StandardError(\"Path does not exist\"))") << endl;
	//cout << "Return from .sys.path.append: " <<  PyRun_SimpleString("import os \nif os.path.exists(\"/home/ubuntu/git/release/teamprojekt/hostmodule\"):\n\tprint 'exist'\n\tsys.path.append(\"/home/ubuntu/git/release/teamprojekt/hostmoduleYZU\")\n\nelse:\n\traise(StandardError(\"Path does not exist\"))") << endl;
	//cout << "Return from .sys.path.append("<< tempString.toStdString().c_str() <<"): " <<  PyRun_SimpleString(tempString.toStdString().c_str()) << endl;
	if(PyRun_SimpleString(tempString.toStdString().c_str()) != 0)
	{
		cout << "Error setting sys.path.append("<<pathString.toStdString()<<")" << " Retry it with last char removed from path" << endl;
		pathString.remove(pathString.length()-1,1);
		tempString = (string()+"import sys\nimport os \nif os.path.exists(\""+pathString.toStdString()+"\"):\n\tprint '"+ pathString.toStdString() +" exists'\n\tsys.path.append(\"" + pathString.toStdString() +"\")\n\nelse:\n\traise(StandardError(\"" + pathString.toStdString() + " does not exist\"))").c_str();
		
		if(PyRun_SimpleString(tempString.toStdString().c_str()) == -1)
		{
			cout << "Qt+Python sux: " << tempString.toStdString().c_str() << endl;
			exit(0);
		}
	}

	Transmission::hostmodules = moduledir.entryList(QDir::Files);

	QList<QString>::iterator it;
	for (it = Transmission::hostmodules.begin(); it != Transmission::hostmodules.end(); ++it)
	{
		if(!(*it).endsWith(".py"))
		{
			Transmission::hostmodules.erase(it);
			it = Transmission::hostmodules.begin();
		}
	}

	int i;
	for (i = 0; i < Transmission::hostmodules.length(); i++)
	{
		cout << Transmission::hostmodules.at(i).toStdString() << endl;
	}
	//cout << "Value of sys.path.append: " << PyRun_SimpleString("sys.path.append(\"/home/ubuntu/git/release/teamprojekt/hostmoduleYZU\")") << endl;
}
