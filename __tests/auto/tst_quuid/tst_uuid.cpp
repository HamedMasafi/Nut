#include <QtTest>
#include <QJsonDocument>
#include <QSqlError>
#include <QDebug>
#include <QElapsedTimer>

#include "consts.h"

#include "tst_uuid.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"

#include "test.h"

UuidTest::UuidTest(QObject *parent) : QObject(parent)
{
}

void UuidTest::initTestCase()
{
    REGISTER(Test);
    REGISTER(TestDatabase);

    QFile::remove(DATABASE);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    bool ok = db.open();

    db.tests()->query().remove();
    uuid = QUuid::createUuid();

    QTEST_ASSERT(ok);
}

void UuidTest::save()
{
    TIC();
    auto t = Nut::create<Test>();
    t->setId(QUuid::createUuid());
    t->setUuid(uuid);
    db.tests()->append(t);
    int n = db.saveChanges();
    TOC();

    QTEST_ASSERT(n == 1);
}

void UuidTest::restore()
{
    TIC();
    auto test = db.tests()->query().first();
    TOC();
    QTEST_ASSERT(!test->id().isNull());
    QTEST_ASSERT(test->uuid() == uuid);
}

void UuidTest::cleanupTestCase()
{
//    qDeleteAll(Nut::TableModel::allModels());
//    Nut::DatabaseModel::deleteAllModels();
}

QTEST_MAIN(UuidTest)
