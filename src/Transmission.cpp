#include "Transmission.h"
#include <QCryptographicHash>
#include <Python.h>
#include <iostream>

using namespace std;

QStringList Transmission::hostmodules; //create Static Variable which was defined in Transmission. Quite strange syntax^^. Its for the hostmodule list from main.cpp initPython()

Transmission::Transmission(QDir _tempDir)
{
	tempDir = _tempDir;
	userAgent = getRandomUserAgent();
	srand((unsigned)time(NULL));

	// Initialize Python Interpreter now in Main Window
	//Py_Initialize();
}

QString Transmission::getRandomUserAgent()
{
	QVector<const char*> agents;
	agents.reserve(11);

	agents.push_back("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)");
	agents.push_back("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; WOW64; Trident/4.0; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; OfficeLiveConnector.1.3; OfficeLivePatch.0.0; .NET CLR 3.5.30729; .NET CLR 3.0.30618)");
	agents.push_back("Mozilla/4.77 [en] (X11; I; IRIX;64 6.5 IP30)");
	agents.push_back("Mozilla/5.0 (compatible; Konqueror/3.2; Linux 2.6.2) (KHTML, like Gecko)");
	agents.push_back("Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en) AppleWebKit/125.2 (KHTML, like Gecko) Safari/125.8");
	agents.push_back("Mozilla/5.0 (OS/2; U; Warp 4.5; de; rv:1.8.1.11) Gecko/20071129 PmWFx/2.0.0.11");
	agents.push_back("Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/525.13 (KHTML, like Gecko) Chrome/0.A.B.C Safari/525.13");
	agents.push_back("Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.0.10) Gecko/2009042316 Firefox/3.0.10");
	agents.push_back("Mozilla/5.0 (X11; U; Linux i586; en-US; rv:1.7.3) Gecko/20040924 Epiphany/1.4.4 (Ubuntu)");
	agents.push_back("Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko/2009033100 Ubuntu/9.04 (jaunty) Firefox/3.0.8");
	agents.push_back("Opera/9.80 (Macintosh; Intel Mac OS X; U; en) Presto/2.2.15 Version/10.00");
	return(agents[rand()%agents.size()]);
}

QString Transmission::generateHash(QString _file)
{
	QFile file(_file);
	QCryptographicHash hash(QCryptographicHash::Sha1);
	QByteArray line;
	file.open(QIODevice::ReadOnly);

	while (!file.atEnd()) {
		line = file.readLine();
		hash.addData(line);
	}

	//qDebug() << "Transmission: Generating hash... " << hash.result().toBase64(); //QString(QCryptographicHash::hash(line, QCryptographicHash::Sha1).toBase64())
	return(QString(hash.result().toBase64()));

}

Item_Remote Transmission::upload(QString _file, int recursionDeep)
{

	Item_Remote temp;
	int i;
	size_t string_length;

	//erstmal verzeichnis der hostmodule finden
	QDir moduledir = QDir::current();
	moduledir.cd("../hostmodule");
	QString tempString = moduledir.absolutePath();
	tempString.append("\")");
	tempString.prepend("sys.path.append(\"");

	const char* modulepathchar = tempString.toLatin1().data();

	//zum testen hoster so setzen
	//const char* hoster = "hellameke_com";
	QString randomHostModule = hostmodules.at((_file.length()+rand()+_file[rand()%_file.length()].toAscii())%hostmodules.size());
	randomHostModule.remove(".py");
	const char* hoster = randomHostModule.trimmed().toStdString().c_str();
	unsigned int loopleaver = 0;

	while(strlen(hoster) != randomHostModule.length() && loopleaver > 256)
	{
		cout <<  hoster << "(" << strlen(hoster) << ")" << " / qt: " << randomHostModule.toStdString() << "(" << randomHostModule.length() << ")" << endl;
		hoster = randomHostModule.trimmed().toStdString().c_str();
		loopleaver++;
	}


	//Script das genommen wurde ausgeben
	//cout << "Random Script: " << hostmodules.at(rand()%hostmodules.size()).toStdString() << endl;
//	cout << "Choosen-Hoster: " << hoster << endl;

	//todo: random hoster auswählen
	//bei fail(leerer string), anderen hoster auswählen

	//set up call for the python hostmodules
	PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs, *pReturn;



	//set basedir for python hostmodules
	//PyRun_SimpleString("import sys");
	//PyRun_SimpleString(modulepathchar);
	//we can do it while(strcomp) ... and a secure break after 100 tries with a warning "gt+python sux"
	/*if(!(strcomp(modulepathchar, "sys.path.append(\"/home/ubuntu/git/release/teamprojekt/hostmodule\")");
	{
		cout << "Modulepath Check: " << modulepathchar << " != sys.path.append(\"/home/ubuntu/git/release/teamprojekt/hostmodule\")" << endl;
	}*/

	//PyRun_SimpleString("sys.path.append(\"/home/ubuntu/git/release/teamprojekt/hostmodule\")");

	pName = PyString_FromString(hoster); //name of the python source file
	pModule = PyImport_Import(pName);
	pDict = PyModule_GetDict(pModule); //get all functions from the python source file
	pFunc = PyDict_GetItemString(pDict, "upload"); //select function x from the source file

	pArgs = PyTuple_New(1);
	//const char* tempfile = _file.toLatin1().data();
	const char* tempfile = _file.toLocal8Bit().data();
	char* workAroundCleaner = (char*) tempfile;
	string_length = strlen(tempfile);

	/*for(i = 0; i < string_length; i++)
	{
		if(workAroundCleaner[i] == '`')
		{
			workAroundCleaner[i] = '\0';
		}
	}*/

	pValue = PyString_FromString(tempfile);
	PyTuple_SetItem(pArgs, 0, pValue);

	//call the python upload function
	if (PyCallable_Check(pFunc))
	{
		pReturn = PyObject_CallObject(pFunc, pArgs);
		if(!pReturn)
		{
			PyErr_Print();
			workAroundCleaner = "";

			unsigned int loop_check = 0;
			while(strlen(workAroundCleaner) != _file.size() && loop_check < 200)
			{
				workAroundCleaner = _file.toLatin1().data();
				loop_check++;
			}

			cout << "Re-Try it with new Conversion("<< workAroundCleaner <<")" << endl;
			pArgs = PyTuple_New(1);
			pValue = PyString_FromString(workAroundCleaner);
			PyTuple_SetItem(pArgs, 0, pValue);
			if (PyCallable_Check(pFunc))
			{
				pReturn = PyObject_CallObject(pFunc, pArgs);
				if(!pReturn)
				{
					PyErr_Print();
					workAroundCleaner[strlen(workAroundCleaner)-1] = '\0';
					cout << "Re-Try it with one char less in Filestring("<< workAroundCleaner <<")" << endl;
					pArgs = PyTuple_New(1);
					pValue = PyString_FromString(workAroundCleaner);
					PyTuple_SetItem(pArgs, 0, pValue);

					if (PyCallable_Check(pFunc))
					{
						pReturn = PyObject_CallObject(pFunc, pArgs);
						if(!pReturn)
						{
							cout << "QT + Python Sux " << _file.toLatin1().data() << " / " <<  workAroundCleaner << endl;
							cout << "An Error Occured while Uploading. So do it again. " << endl;
							return (this->upload(_file));
						}
					}
				}
			}
		}

		//test: url ausgeben
		Item_Link linkTemp;
		QString combined;
		QStringList urlReferrer;
		combined = PyString_AsString(pReturn); 		

		if(combined == "\"\"" || combined == "")
		{
			if(recursionDeep > 256)
			{ 
				cout << "An Error Occured in Uploading for " << recursionDeep << " times. That's it. Your network connection is lost, python modules are broken or something in this programm ist wrong. Exit " << endl;		
				exit(1);
			}

			cout << "An Error Occured while Uploading File. Response from Server was empty. So do it again. " << endl;	
			return (this->upload(_file, recursionDeep+1));
		}

		else
		{
			urlReferrer = combined.split(" ");

			linkTemp.url = urlReferrer.at(0);
			linkTemp.referrer = urlReferrer.size() == 1 ? "" : urlReferrer.at(1);
			//linkTemp.url = PyString_AsString(pReturn);
			temp.link.push_back(linkTemp); 
			temp.hash = generateHash(_file);
			
			if(recursionDeep >= 0)
			{
				Item_Remote tempMirror = upload(_file, -1000);
				temp.link.push_back(tempMirror.link[0]);
			}
			

			qDebug() << "out URL: " << temp.link[0].url << " / Referrer: " << temp.link[0].referrer;
		}
		
		//splitten nach leerzeichen für referrer
		//hoster in item_remote schreiben, ggf referrer
	}
	else {
		PyErr_Print();
		
		if(recursionDeep > 256)
		{ 
			cout << "An Error Occured in Uploading for " << recursionDeep << " times. That's it. Your network connection is lost, python modules are broken or something in this programm ist wrong. Exit " << endl;		
			exit(1);
		}
		else
		{
			cout << "An Error Occured while Uploading. So do it again. " << endl;	
			return (this->upload(_file, recursionDeep+1));
		}
	}

	qDebug() << "Transmission: Uploading "<< _file;
	return temp;
}

Item_Link Transmission::createMirror(QUrl& _url)
{
	Item_Link mirror;
	return (mirror);
}

bool Transmission::download(Item* _item)
{
	qDebug() << "Transmission: Downloading ";// << _item.remote.url[0].download;
	
	//=====================================
	QVector<Item_Link>& links = (*_item).remote.link;
	QString& hash = (*_item).remote.hash;

        QEventLoop loop;
	QNetworkAccessManager downloader;
	QNetworkRequest qheader;
	int downloadMirror = 0;
	QUrl poorGuy;
	QNetworkReply *reply;
	QString fileName;
	QFile file;

	do
	{
		if(downloadMirror >= links.size())
		{
			cout << "All Mirros have returned corrupt data" << endl;
			return(false);
		}

		poorGuy = links[downloadMirror].url;
	
		qheader.setUrl(poorGuy);
		qheader.setRawHeader("Host", poorGuy.host().toStdString().c_str());
		qheader.setRawHeader("Referrer", links[downloadMirror].referrer.size() < 4 ? poorGuy.host().toStdString().c_str() : links[downloadMirror].referrer.toStdString().c_str());
		qheader.setRawHeader("Referrer", links[downloadMirror].referrer.toStdString().c_str());
		qheader.setRawHeader("User-Agent", userAgent.toStdString().c_str());
		qheader.setRawHeader("Accept", "image/png,image/*;q=0.8,*/*;q=0.5");
		qheader.setRawHeader("Accept-Language", "en-us,en;q=0.5");
		qheader.setRawHeader("Accept-Encoding", "gzip,deflate");
		qheader.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
		qheader.setRawHeader("Keep-Alive", "300");
		qheader.setRawHeader("Connection", "keep-alive");


		reply = downloader.get( qheader );
	        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	        loop.exec();

		fileName = tempDir.absolutePath()+tempDir.separator()+getRandomFileName(hash+getRandomUserAgent(),links[0].url);

		while(QFile::exists(fileName))
		{
			fileName = tempDir.absolutePath()+tempDir.separator()+getRandomFileName(hash+fileName,links[0].url);
		}

		file.setFileName(fileName); 
		//cout << "Written to: " << fileName.toStdString() << endl;
		file.open(QIODevice::WriteOnly);
		file.write(reply->readAll());
		file.close();
		_item->image.file = fileName;
		if(hash != generateHash(fileName))
		{
			cout << "Hash is different("<<poorGuy.toString().toStdString()<<"): " << hash.toStdString() << " : " << generateHash(fileName).toStdString() << endl;
			//try an other mirror
		}
		downloadMirror++;

	}
	while(hash != generateHash(fileName) && hash != ""); //if hash is empty its ok because we dont know it



        //QNetworkReply *reply2 = manager.get( request2 );
	//... for parallel downloads

	cout << "Itemfile(" << _item->remote.link[downloadMirror-1].url.toStdString() << "): " << _item->image.file.toStdString() << " / " << _item->image.datalength << " / " << _item->image.method << endl;

	return true;
}

QString Transmission::buildHeader(QString& _url,  QString& _referrer)
{
	QUrl host(_url);
	return(QString()+"Host: "+ host.host() +"\nUser-Agent: "+ userAgent +"\nAccept: image/png,image/*;q=0.8,*/*;q=0.5\nAccept-Language: en-us,en;q=0.5\nAccept-Encoding: gzip,deflate\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\nKeep-Alive: 300\nConnection: keep-alive\nReferer: "+_referrer);
}

QString Transmission::getRandomFileName(const QString& _seed, const QString& _extension)
{

	char name[33];
	int i;
	for(i = 0; i < 32; i++)
	{
		name[i] = ((_seed[rand()%_seed.size()].toAscii()+rand())%26)+97;
	}
	name[32] = '\0';

	return(QString("") + name + getExtension(_extension.toStdString()).c_str());
}

string Transmission::getExtension(const string& _image)
{
    int j = _image.length()-1;

    while(j > -1)
    {
        if(_image[j] == '.')
        {
            break;
        }
        j--;
    }

    if(j>-1)
    {
        return(_image.substr(j, _image.length()-j));
    }

    else
    {
        return("");
    }
}
