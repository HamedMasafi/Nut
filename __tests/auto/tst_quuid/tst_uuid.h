#ifndef MAINTEST_H
#define MAINTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QUuid>

#include "testdatabase.h"
class Test;
class UuidTest : public QObject
{
    Q_OBJECT
    TestDatabase db;
    QUuid uuid;

public:
    explicit UuidTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();
    void save();
    void restore();

    void cleanupTestCase();
};

#endif // MAINTEST_H
