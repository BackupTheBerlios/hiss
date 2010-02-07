#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QDir>
#include <QVector>

using namespace std;

struct Item_Image {
	QString file;
	quint64 datalength;
	quint64 padding;
	quint64 method;
};

struct Item_Link {
	QString referrer;
	QString url;
};

struct Item_Remote {
	QString hash;
	QVector<Item_Link> link;
};

struct Item {
	Item_Image image;
	Item_Remote remote;
	qint32 id;
};

#endif
