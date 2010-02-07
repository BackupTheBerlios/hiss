#ifndef ChunkerTEST_H
#define ChunkerTEST_H

#include <QtCore/QObject>
#include "../Chunker.h"

class ChunkerTest : public QObject
{
Q_OBJECT
private slots:
	void initTestCase();
	void init();
	void cleanup();
	void cleanupTestCase();
	void chunkFile();
private:
	QFile testfile;
	QString filename;
};

#endif // ChunkerTEST_H
