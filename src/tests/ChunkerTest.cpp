#include "ChunkerTest.h"
#include <QtTest/QTest>

void ChunkerTest::initTestCase()
{
	testfile.setFileName("src/tests/testfile.txt");
	testfile.open(QIODevice::WriteOnly);
	QByteArray tempArray="bit8h3tbastixb";
	testfile.write(tempArray);
	testfile.close();
}

void ChunkerTest::init()
{}

void ChunkerTest::cleanup()
{}

void ChunkerTest::cleanupTestCase()
{
	testfile.remove();
}

void ChunkerTest::chunkFile()
{
		Chunker chunker("src/tests/testfile.txt",32,false);
		
		bool haveNext = chunker.hasNext();
		QCOMPARE(haveNext,true); //is there still data in the file?
		quint64 size = 10;
		QByteArray tempData = chunker.getNext(size);
		qDebug() << "chunk: " << tempData.toBase64();
		qDebug() << "size: " << tempData.size();
		
		haveNext = chunker.hasNext();
		QCOMPARE(haveNext,true);
		
		tempData = chunker.getNext(size);
		qDebug() << "base64 chunk: " << tempData.toBase64();
		
		haveNext = chunker.hasNext();
		QCOMPARE(haveNext,false);
}

QTEST_MAIN(ChunkerTest)
#include "ChunkerTest.moc"
