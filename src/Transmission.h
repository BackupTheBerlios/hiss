#ifndef TRANSMISSION_H_INCLUDED
#define TRANSMISSION_H_INCLUDED

#include "globals.h"
#include <QUrl>
#include <QHttp>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QString>
#include <QVector>
#include <QUrl>
#include <QFile>
#include <QObject>
#include <QIODevice>
#include <iostream>

class Transmission
{
	public:
	Transmission(QDir _tempDir);

	QString generateHash(QString _file);
	
	Item_Remote upload(QString _file, int recursionDeep = 0);
	bool download(Item* _item);

	static QStringList hostmodules;
	QString buildHeader(QString& _url,  QString& _referrer);
	QString getRandomUserAgent();
	QString getRandomFileName(const QString& _seed, const QString& _extension);	
	string getExtension(const string& _image);
	Item_Link createMirror(QUrl& _url);

	private:
	QDir tempDir;
	QString userAgent;

};

#endif
