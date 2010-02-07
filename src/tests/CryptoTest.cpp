#include "CryptoTest.h"
#include <QtTest/QTest>
#include <botan/botan.h>

void CryptoTest::initTestCase()
{}

void CryptoTest::init()
{
	Botan::LibraryInitializer init;
}

void CryptoTest::cleanup()
{}

void CryptoTest::cleanupTestCase()
{}

void CryptoTest::testWhole()
{
	QByteArray input("tb39dtebtcebsceg");

	QByteArray reference = input;

	QString userKey;
	userKey = "lol";

	qDebug() << "cleartext: " << input;

	Crypto crypto;
	crypto.encrypt(input, userKey);
	qDebug() <<"encrypted: " <<  input;
	qDebug() << reference.length();
	qDebug() << input.length();
	crypto.decrypt(input, userKey);
	qDebug() << "again clear: " << input;
}

QTEST_MAIN(CryptoTest)
#include "CryptoTest.moc"
