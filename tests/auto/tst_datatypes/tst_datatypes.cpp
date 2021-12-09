#include <QtTest>
#include <QDebug>
#include <QSqlError>
#include <QElapsedTimer>

#include "consts.h"

#include "tst_datatypes.h"
#include "query.h"
#include "tableset.h"
#include "tablemodel.h"
#include "databasemodel.h"

#include "sampletable.h"

#include <QtNut/sqlitegenerator.h>
#include <QtNut/sqlservergenerator.h>

DataTypesTest::DataTypesTest(QObject *parent) : QObject(parent)
{
}

void DataTypesTest::initTestCase()
{
    //register all entities with Qt-MetaType mechanism
    REGISTER(SampleTable);
    REGISTER(DB);

    db.setDriver(DRIVER);
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    QFile::remove(DATABASE);
    bool ok = db.open();
    f_int8 = 8;
    f_int16 = 16;
    f_int32 = 32l;
    f_int64 = 64ll;
    f_uint8 = 8u;
    f_uint16 = 16u;
    f_uint32 = 32ul;
    f_uint64 = 64ull;
    f_real = 1.2;
    f_float = 2.3f;

    f_url = QUrl(QStringLiteral("http://google.com/search?q=nut"));

    f_time = QTime::currentTime();
    f_time.setHMS(f_time.hour(), f_time.minute(), f_time.second());

    f_date = QDate::currentDate();
    f_dateTime = QDateTime::currentDateTime();
    f_dateTime.setTime(f_time);

    f_uuid = QUuid::createUuid();
    f_jsonDoc = QJsonDocument::fromJson("{\"a\": 1}");
    f_jsonObj = f_jsonDoc.object();
    f_jsonArray.insert(0, QJsonValue(1));
    f_jsonArray.insert(1, QJsonValue(QString::fromUtf8("Hi")));
    f_jsonArray.insert(2, QJsonValue(true));

    f_jsonValue = QJsonValue(true);

    f_stringList.append(QStringLiteral("One"));
    f_stringList.append(QStringLiteral("Two"));
    f_stringList.append(QStringLiteral("Three"));
    f_string = QStringLiteral("this is \n sample ' unescapped \r\n text");

    f_qchar = QLatin1Char('z');

#ifdef QT_GUI_LIB
    f_point = QPoint(1, 2);
    f_pointf  = QPointF(1.2, 3.4);
    f_polygon = QPolygon() << QPoint(1, 2) << QPoint(3, 4) << QPoint(5, 6);
    f_polygonf = QPolygonF() << QPointF(1.2, 2.3) << QPointF(3.4, 4.5) << QPointF(5.6, 6.7);
    f_color = Qt::red;
#endif

    QTEST_ASSERT(ok);

    db.sampleTables()->query().remove();
}

void DataTypesTest::insert()
{
    auto t = Nut::create<SampleTable>();

    t->setInt8(f_int8);
    t->setInt16(f_int16);
    t->setInt32(f_int32);
    t->setInt64(f_int64);

    t->setUint8(f_uint8);
    t->setUint16(f_uint16);
    t->setUint32(f_uint32);
    t->setUint64(f_uint64);

    t->setReal(f_real);
    t->setFloat(f_float);

    t->setUrl(f_url);

    t->setTime(f_time);
    t->setDate(f_date);
    t->setDateTime(f_dateTime);
    t->setUuid(f_uuid);

    t->setJsonDoc(f_jsonDoc);
    t->setJsonObj(f_jsonObj);
    t->setJsonArray(f_jsonArray);
    t->setJsonValue(f_jsonValue);

    t->setString(f_string);
    t->setStringList(f_stringList);
    t->setQchar(f_qchar);
#ifdef QT_GUI_LIB
    t->setColor(f_color);

    t->setPoint(f_point);
    t->setPointf(f_pointf);

    t->setPolygon(f_polygon);
    t->setPolygonf(f_polygonf);
#endif
    db.sampleTables()->append(t);
    db.saveChanges();
}

void DataTypesTest::retrive()
{
    Nut::RowList<SampleTable> list = db.sampleTables()->query().toList();
    QTEST_ASSERT(list.count() == 1);
    Nut::Row<SampleTable> t = list.first();

    QTEST_ASSERT(t->f_int8() == f_int8);
    QTEST_ASSERT(t->f_int16() == f_int16);
    QTEST_ASSERT(t->f_int32() == f_int32);
    QTEST_ASSERT(t->f_int64() == f_int64);

    QTEST_ASSERT(t->f_uint8() == f_uint8);
    QTEST_ASSERT(t->f_uint16() == f_uint16);
    QTEST_ASSERT(t->f_uint32() == f_uint32);
    QTEST_ASSERT(t->f_uint64() == f_uint64);

    qDebug() << t->f_real()
             << f_real
             << qAbs(t->f_real() - f_real) * 1000000000000.f
             << qFuzzyCompare(t->f_real(), f_real);
//    QTEST_ASSERT(qFuzzyCompare(t->f_real(), f_real));
//    QTEST_ASSERT(qFuzzyCompare(t->f_float(), f_float));


    QTEST_ASSERT(t->f_url() == f_url);
    QTEST_ASSERT(t->f_uuid() == f_uuid);

    QTEST_ASSERT(t->f_time() == f_time);
    QTEST_ASSERT(t->f_date() == f_date);
    QTEST_ASSERT(t->f_dateTime() == f_dateTime);

    QTEST_ASSERT(t->f_jsonDoc() == f_jsonDoc);
    QTEST_ASSERT(t->f_jsonObj() == f_jsonObj);
    QTEST_ASSERT(t->f_jsonArray() == f_jsonArray);
    QTEST_ASSERT(t->f_jsonValue() == f_jsonValue);

    QTEST_ASSERT(t->f_string() == f_string);
    QTEST_ASSERT(t->f_stringList() == f_stringList);
    QTEST_ASSERT(t->f_qchar() == f_qchar);
#ifdef QT_GUI_LIB
    QTEST_ASSERT(t->f_point() == f_point);
    QTEST_ASSERT(t->f_pointf() == f_pointf);

    QTEST_ASSERT(t->f_polygon() == f_polygon);
    QTEST_ASSERT(t->f_polygonf() == f_polygonf);
    QTEST_ASSERT(t->f_color() == f_color);
#endif
}

#define CHECK(name) \
    c = db.sampleTables()->query()                                             \
            .where(SampleTable::f_ ## name ## Field() == f_ ## name)          \
            .count();                                                         \
    QTEST_ASSERT(c == 1);

void DataTypesTest::check()
{
    int c;

    CHECK(int8)
    CHECK(int16)
    CHECK(int32)
    CHECK(int64)
    CHECK(uint8)
    CHECK(uint16)
    CHECK(uint32)
    CHECK(uint64)
    CHECK(real)
    CHECK(float)
    CHECK(url)

    CHECK(time)
    CHECK(date)
    CHECK(dateTime)

    CHECK(uuid)
//    CHECK(jsonDoc)
//    CHECK(jsonObj)
//    CHECK(jsonArray)
//    CHECK(jsonValue)

    CHECK(string)
    CHECK(stringList)

    CHECK(qchar)
#ifdef QT_GUI_LIB
    CHECK(point)
    CHECK(pointf)
    CHECK(polygon)
    CHECK(polygonf)
    CHECK(color)
#endif
}

void DataTypesTest::cleanupTestCase()
{
    db.sampleTables()->query().remove();
    db.close();

    PRINT_FORM(db);
}

QTEST_MAIN(DataTypesTest)
