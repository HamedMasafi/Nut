#include <QtTest>

#include "db.h"
#include "sampletable.h"
#include "query.h"

#include "tst_json.h"
#include "consts.h"

void TestJson::initDb(Nut::Database &db)
{
    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);
}

TestJson::TestJson()
{

}

TestJson::~TestJson()
{

}

void TestJson::initTestCase()
{
    QFile::remove(DATABASE);
    REGISTER(DB);
    REGISTER(Table);
}

void TestJson::store()
{
    initDb(db);

    db.open();

    auto t = Nut::create<Table>();
    QJsonParseError e;
    QJsonDocument doc = QJsonDocument::fromJson(R"({"a": 4, "b":3.14})", &e);
    qDebug() << e.errorString();
    t->setDoc(doc);
    db.sampleTable()->append(t);
    db.saveChanges();

    int id = t->id();
    auto newObj = db.sampleTable()->query()
            .where(Table::idField() == id)
            .orderBy(Table::idField())
            .first();

    Q_ASSERT(newObj != nullptr);
    Q_ASSERT(newObj->doc() == t->doc());
}

void TestJson::cleanupTestCase()
{
    PRINT_FORM(db);
}

QTEST_APPLESS_MAIN(TestJson)

