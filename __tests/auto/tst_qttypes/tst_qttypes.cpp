#include "db.h"


#include "consts.h"

#include "sampletable.h"
#include "tst_qttypes.h"

#include <QtNut/Query>


QtTypes::QtTypes()
{
    REGISTER(SampleTable);
    REGISTER(DB);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    QFile::remove(DATABASE);

    QVERIFY(db.open());
}

QtTypes::~QtTypes()
{

}

void QtTypes::insert()
{
    auto t = Nut::create<SampleTable>();
    t->setPointf({1.2, 3.4});
    db.sampleTables()->append(t);
    db.saveChanges();

    auto t2 = db.sampleTables()->query().first();
    QCOMPARE(t->f_pointf(), t2->f_pointf());
}

void QtTypes::update(){
    auto t = db.sampleTables()->query().first();
    t->setPointf({5.6, 7.8});
    db.saveChanges();

    auto t2 = db.sampleTables()->query().first();
    QCOMPARE(t->f_pointf(), t2->f_pointf());
}

QTEST_APPLESS_MAIN(QtTypes)
