#include <QtTest>
#include "tst_properties.h"
#include "sampledatabase.h"
#include "sampletable.h"
#include "../common/consts.h"
#include <QtNut/Query>

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

    db.items()->query().remove();
}

void PropertiesTest::insert()
{
    auto s = new SampleTable;
    s->setId(1);
    s->setName("hamed");
    s->setLastName("masafi");
    db.items()->append(s);
    auto c = db.saveChanges(); // returns count of affected rows
    QCOMPARE(c, 1);
}

void PropertiesTest::select()
{
    auto item = db.items()->query()
        .where(SampleTable::nameField() == "hamed" && SampleTable::lastNameField() == "masafi")
        .first();
    QCOMPARE(item->name(), "hamed");
}

void PropertiesTest::parallelUpdate()
{
    auto item1 = db.items()->query()
        .first();
    {
        auto item2 = db.items()->query()
            .first();

        item2->setLastName("masafi 2");
        db.saveChanges();
    }
    item1->setName("hamed 2");
    db.saveChanges();

    auto item = db.items()->query()
        .first();

    QCOMPARE(item->name(), "hamed 2");
    QCOMPARE(item->lastName(), "masafi 2");
}

QTEST_MAIN(PropertiesTest)
