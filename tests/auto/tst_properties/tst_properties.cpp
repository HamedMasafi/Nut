#include <QtTest>
#include "tst_properties.h"
#include "sampledatabase.h"
#include "sampletable.h"
#include "../common/consts.h"

PropertiesTest::PropertiesTest(QObject *parent) : QObject(parent)
{
}

void PropertiesTest::initTestCase()
{

    REGISTER(SampleTable);
    REGISTER(SampleDataBase);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    bool ok = db.open();
    QVERIFY(ok);
}

void PropertiesTest::insert()
{
    auto s = new SampleTable;
    s->setId(1);
    s->setName("hamed");
    db.items()->append(s);
    db.saveChanges();
}

QTEST_MAIN(PropertiesTest)
