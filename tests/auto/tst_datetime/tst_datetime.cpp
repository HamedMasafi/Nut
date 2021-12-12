#include <QtTest>
#include <QDebug>
#include <QSqlError>
#include <QElapsedTimer>

#include "consts.h"

#include "tst_datetime.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "sampletable.h"

DateTimeTest::DateTimeTest(QObject *parent) : QObject(parent)
{
    _baseDateTime = QDateTime::currentDateTime();
}

void DateTimeTest::initTestCase()
{
    //register all entities with Qt-MetaType mechanism
    REGISTER(SampleTable);
    REGISTER(DB);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    QVERIFY(db.open());

    db.sampleTables()->query().remove();
}

#define TEST_DATE(date, command, n)                                                                \
    do {                                                                                           \
        auto s = Nut::create<SampleTable>();                                                       \
        s->setD(date);                                                                             \
        db.sampleTables()->append(s);                                                              \
        db.saveChanges();                                                                          \
        auto count = db.sampleTables()                                                             \
                         ->query()                                                                 \
                         .where(SampleTable::dField().command(n) == date.command(n))               \
                         .count();                                                                 \
        QVERIFY(count > 0);                                                                        \
        db.sampleTables()->query().remove();                                                       \
    } while (false)

#define TEST_TIME(time, command, n, num)                                                           \
    do {                                                                                           \
        auto s = Nut::create<SampleTable>();                                                       \
        s->setT(time);                                                                             \
        db.sampleTables()->append(s);                                                              \
        db.saveChanges();                                                                          \
        auto count = db.sampleTables()                                                             \
                         ->query()                                                                 \
                         .where(SampleTable::tField().command(n) == time.addSecs(num))             \
                         .count();                                                                 \
        QVERIFY(count > 0);                                                                        \
        db.sampleTables()->query().remove();                                                       \
    } while (false)

#define TEST_DATE2(datetime, command, n)                                                           \
    do {                                                                                           \
        auto s = Nut::create<SampleTable>();                                                       \
        s->setDT(datetime);                                                                        \
        db.sampleTables()->append(s);                                                              \
        db.saveChanges();                                                                          \
        auto count = db.sampleTables()                                                             \
                         ->query()                                                                 \
                         .where(SampleTable::dtField().command(n) == datetime.command(n))          \
                         .count();                                                                 \
        QVERIFY(count > 0);                                                                        \
        db.sampleTables()->query().remove();                                                       \
    } while (false)

#define TEST_TIME2(datetime, command, n, num)                                                      \
    do {                                                                                           \
        auto s = Nut::create<SampleTable>();                                                       \
        s->setDT(datetime);                                                                        \
        db.sampleTables()->append(s);                                                              \
        db.saveChanges();                                                                          \
        auto count = db.sampleTables()                                                             \
                         ->query()                                                                 \
                         .where(SampleTable::dtField().command(n) == datetime.addSecs(num))        \
                         .count();                                                                 \
        QVERIFY(count > 0);                                                                        \
        db.sampleTables()->query().remove();                                                       \
    } while (false)

#define MINUTE(m) m * 60
#define HOUR(h) MINUTE(h) * 60

void DateTimeTest::dateAdd()
{
    QDate d = QDate::currentDate();

    TEST_DATE(d, addYears, 10);
    TEST_DATE(d, addMonths, 10);
    TEST_DATE(d, addDays, 10);

    TEST_DATE(d, addYears, -10);
    TEST_DATE(d, addMonths, -10);
    TEST_DATE(d, addDays, -10);
}

void DateTimeTest::timeAdd()
{
    QTime t = QTime::currentTime();

    TEST_TIME(t, addHours, 10, HOUR(10));
    TEST_TIME(t, addMinutes, 10, MINUTE(10));
    TEST_TIME(t, addSeconds, 10, 10);

    TEST_TIME(t, addHours, -10, HOUR(-10));
    TEST_TIME(t, addMinutes, -10, MINUTE(-10));
    TEST_TIME(t, addSeconds, -10, -10);
}

void DateTimeTest::dateTimeAdd()
{
    QDateTime dt = QDateTime::currentDateTime();

    TEST_DATE2(dt, addYears, 10);
    TEST_DATE2(dt, addMonths, 10);
    TEST_DATE2(dt, addDays, 10);

    TEST_DATE2(dt, addYears, -10);
    TEST_DATE2(dt, addMonths, -10);
    TEST_DATE2(dt, addDays, -10);


    TEST_TIME2(dt, addHours, 10, HOUR(10));
    TEST_TIME2(dt, addMinutes, 10, MINUTE(10));
    TEST_TIME2(dt, addSeconds, 10, 10);

    TEST_TIME2(dt, addHours, -10, HOUR(-10));
    TEST_TIME2(dt, addMinutes, -10, MINUTE(-10));
    TEST_TIME2(dt, addSeconds, -10, -10);
}

void DateTimeTest::datePart()
{
    db.sampleTables()->query().remove();

    QDate d = QDate::currentDate();
    auto s = Nut::create<SampleTable>();
    s->setD(d);
    db.sampleTables()->append(s);
    db.saveChanges();

    int count;

    count = db.sampleTables()->query().where(SampleTable::dField().year() == d.year()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::dField().month() == d.month()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::dField().day() == d.day()).count();
    QVERIFY(count);

}

void DateTimeTest::timePart()
{
    db.sampleTables()->query().remove();

    QTime t = QTime::currentTime();
    auto s = Nut::create<SampleTable>();
    s->setT(t);
    db.sampleTables()->append(s);
    db.saveChanges();

    int count;

    count = db.sampleTables()->query().where(SampleTable::tField().hour() == t.hour()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::tField().minute() == t.minute()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::tField().second() == t.second()).count();
    QVERIFY(count);
}

void DateTimeTest::dateTimePart()
{
    db.sampleTables()->query().remove();

    QDateTime dt = QDateTime::currentDateTime();
    auto s = Nut::create<SampleTable>();
    s->setDT(dt);
    db.sampleTables()->append(s);
    db.saveChanges();

    int count;

    count = db.sampleTables()->query().where(SampleTable::dtField().year() == dt.date().year()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::dtField().month() == dt.date().month()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::dtField().day() == dt.date().day()).count();
    QVERIFY(count);

    count = db.sampleTables()->query().where(SampleTable::dtField().hour() == dt.time().hour()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::dtField().minute() == dt.time().minute()).count();
    QVERIFY(count);
    count = db.sampleTables()->query().where(SampleTable::dtField().second() == dt.time().second()).count();
    QVERIFY(count);
}

void DateTimeTest::cleanupTestCase()
{
    db.sampleTables()->query().remove();
    db.close();
}

QTEST_MAIN(DateTimeTest)
