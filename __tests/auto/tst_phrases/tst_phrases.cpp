#include <QtTest>
#include <QDate>
#include <qtestcase.h>

#include "tst_phrases.h"
#include "phrase.h"
#include "generator.h"

using namespace Nut;

#define COMPARE_WHERE(w, sql) QCOMPARE(g.where(w), sql);
#define COMPARE_ORDER(o, sql) QCOMPARE(g.order(o), sql);
#define COMPARE_SELECT(s, sql) QCOMPARE(g.select(s), sql);

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")

PhrasesTest::PhrasesTest(QObject *parent) : QObject(parent)
{

}

void PhrasesTest::initTestCase()
{

}

void PhrasesTest::no1()
{
    {
        FieldPhrase<int> id("main", "id");
        FieldPhrase<QString> name("main", "name");
        FieldPhrase<QString> last_name("main", "last_name");
        FieldPhrase<QDate> date("main", "date");
        auto w = (id == 4 && name == QStringLiteral("hi"));

        Generator g;

        COMPARE_WHERE(id == 10 || id.in({1, 2, 3, 4}), "([main].id = '10' OR [main].id IN ('1', '2', '3', '4'))");
    }
}

void PhrasesTest::condition_numeric_sqlite()
{
    Generator g;


    FieldPhrase<int> n("main", "int");
    FieldPhrase<float> f("main", "float");

    COMPARE_WHERE(n < 1, "[main].int < '1'");
    COMPARE_WHERE(n > 1, "[main].int > '1'");
    COMPARE_WHERE(n <= 1, "[main].int <= '1'");
    COMPARE_WHERE(n >= 1, "[main].int >= '1'");
    COMPARE_WHERE(n != 1, "[main].int <> '1'");
    COMPARE_WHERE(n == 1, "[main].int = '1'");
    COMPARE_WHERE(n++, "[main].int + '1'");
    COMPARE_WHERE(++n, "[main].int + '1'");
    COMPARE_WHERE(n.between(10, 20), "[main].int BETWEEN '10' AND '20'");
    COMPARE_WHERE(n + 1 < n + 4, "[main].int + '1' < [main].int + '4'");

    auto p1 = n == 1;
    auto p2 = n <= 4;
    auto p3 = n >= 5;
    auto p4 = n < 7;

    COMPARE_WHERE(p1 && p2, "([main].int = '1' AND [main].int <= '4')");
    COMPARE_WHERE(p3 == p4, "[main].int >= '5' = [main].int < '7'");
    COMPARE_WHERE(f == n + 1, "[main].float = [main].int + '1'");
    COMPARE_WHERE(f == 1.4 || (n == n + 1 && n < 100),
                  "([main].float = '1.4' OR ([main].int = [main].int + '1' AND [main].int < '100'))");

    auto p24 = n = 4;
    auto p26 = (n = 4) & (n = 5);
    auto p27 = n | f;
}

void PhrasesTest::condition_string_sqlite()
{
    Generator g;
    FieldPhrase<QString> str("main", "string");

    COMPARE_WHERE(str == "Hi", "[main].string = 'Hi'");
    COMPARE_WHERE(str.like("%hi%"), "[main].string LIKE '%hi%'");
    COMPARE_WHERE(str.isNull(), "[main].string IS NULL");
    COMPARE_WHERE(!str.isNull(), "[main].string IS NOT NULL");
    COMPARE_WHERE(str.in(QStringList() << "one"
                                       << "two"
                                       << "three"),
                  "[main].string IN ('one', 'two', 'three')");

    COMPARE_WHERE(!str.in(QStringList() << "one"
                                       << "two"
                                       << "three"),
                  "[main].string NOT IN ('one', 'two', 'three')");
    COMPARE_WHERE(str != "hi" && str.like("%s"),
                  "([main].string <> 'hi' AND [main].string LIKE '%s')");
}

void PhrasesTest::condition_bool_sqlite()
{
    Generator g;
    FieldPhrase<bool> b("main", "bool");

    COMPARE_WHERE(b, "[main].bool = 'true'");
    COMPARE_WHERE(!b, "[main].bool = 'false'");
    COMPARE_WHERE(b == true, "[main].bool = 'true'");
    COMPARE_WHERE(b == false, "[main].bool = 'false'");
}

void PhrasesTest::condition_datetime_sqlite()
{
    Generator g;

    FieldPhrase<QTime> time("main", "time");
    FieldPhrase<QDate> date("main", "date");
    FieldPhrase<QDateTime> datetime("main", "datetime");

    QDate d(2020, 2, 20);
    QTime t(12, 34, 56);
    QDateTime dt(d, t);

    COMPARE_WHERE(time.hour() == 1, "CAST(strftime('%H', [main].time) AS INT) = '1'");
    COMPARE_WHERE(time.minute() == 2, "CAST(strftime('%M', [main].time) AS INT) = '2'");
    COMPARE_WHERE(time.second() == 3, "CAST(strftime('%S', [main].time) AS INT) = '3'");

    COMPARE_WHERE(date.year() == 1, "CAST(strftime('%Y', [main].date) AS INT) = '1'");
    COMPARE_WHERE(date.month() == 2, "CAST(strftime('%m', [main].date) AS INT) = '2'");
    COMPARE_WHERE(date.day() == 3, "CAST(strftime('%d', [main].date) AS INT) = '3'");

    COMPARE_WHERE(time.isNull(), "[main].time IS NULL");
    COMPARE_WHERE(!time.isNull(), "[main].time IS NOT NULL");
    COMPARE_WHERE(time == t, "[main].time = '12:34:56'");
    COMPARE_WHERE(time.between(t.addSecs(-10),
                               t),
                  "[main].time BETWEEN '12:34:46' AND '12:34:56'");
    COMPARE_WHERE(date.addDays(2) == d, "DATE([main].date,'+2 DAY') = '2020-02-20'");
    COMPARE_WHERE(time.addMinutes(-3) == t, "TIME([main].time,'-3 MINUTE') = '12:34:56'");
    COMPARE_WHERE(datetime.addMinutes(1) == dt, "DATETIME([main].datetime,'+1 MINUTE') = '2020-02-20 12:34:56'");
}

void PhrasesTest::order_sqlite()
{
    Generator g;

    FieldPhrase<int> id("main", "id");
    FieldPhrase<QString> name("main", "name");
    FieldPhrase<QString> last_name("main", "last_name");

    COMPARE_ORDER(id, "[main].id");
    COMPARE_ORDER(id | name, "[main].id, [main].name");
    COMPARE_ORDER(id | !name | last_name, "[main].id, [main].name DESC, [main].last_name");
}

void PhrasesTest::select_sqlite()
{
    Generator g;

    FieldPhrase<int> id("main", "id");
    FieldPhrase<QString> name("main", "name");
    FieldPhrase<QString> last_name("main", "last_name");

    COMPARE_ORDER(id, "[main].id");
    COMPARE_ORDER(id | name, "[main].id, [main].name");
    COMPARE_ORDER(id | name | last_name, "[main].id, [main].name, [main].last_name");
}

void PhrasesTest::extra()
{
    Generator g;
    FieldPhrase<QUrl> url("main", "url");

    COMPARE_WHERE(url == QUrl("http://google.com"), "[main].url = 'http://google.com'");
}

void PhrasesTest::mix()
{
    FieldPhrase<int> id("", "");
    FieldPhrase<QString> name("", "");
    FieldPhrase<QString> lastName("", "");
    FieldPhrase<QDate> birthDate("", "");

    select(id);
    select(id | name | lastName);
    update((name = "john") & (lastName = "snow"));
    insert(id = 0);
    insert((id = 4) & (name = "john"));
    order_by(id);
    order_by(id | !name);
}

void PhrasesTest::select(const PhraseList &ph)
{
    QTEST_ASSERT(ph.data.count());
}

void PhrasesTest::where(const ConditionalPhrase &ph)
{
    QTEST_ASSERT(ph.data);
}

void PhrasesTest::update(const AssignmentPhraseList &p)
{
    QTEST_ASSERT(p.data.count());
}

void PhrasesTest::insert(const AssignmentPhraseList &p)
{
    QTEST_ASSERT(p.data.count());
}

void PhrasesTest::order_by(const PhraseList &ph)
{
    QTEST_ASSERT(ph.data.count());
}

QTEST_MAIN(PhrasesTest)

QT_WARNING_POP
