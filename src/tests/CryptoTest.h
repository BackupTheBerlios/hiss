#ifndef CryptoTEST_H
#define CryptoTEST_H

#include <QtCore/QObject>
#include "../Crypto.h"

class CryptoTest : public QObject
{
Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void testWhole();
};

#endif // CryptoTEST_H
