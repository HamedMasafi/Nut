#include <QtTest>
#include <QDate>
#include <qtestcase.h>

#include "tst_phrases.h"
#include "phrase.h"
#include "generator.h"

using namespace Nut;

#define COMPARE_WHERE(w, sql) QCOMPARE(g.where(w), sql);

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

        COMPARE_WHERE(id == 10 || id.in({1, 2, 3, 4}), "([main].id = '10' OR [main].id IN (1, 2, 3, 4))");
    }
}

void PhrasesTest::numeric()
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

void PhrasesTest::string()
{
    Generator g;
    FieldPhrase<QString> str("main", "string");

    COMPARE_WHERE(str == "Hi", "[main].string = 'Hi'");
    COMPARE_WHERE(str.like("%hi%"), "[main].string LIKE '%hi%'");
    COMPARE_WHERE(str.isNull(), "[main].string IS NULL");
    COMPARE_WHERE(str.in(QStringList() << "one"
                                       << "two"
                                       << "three"),
                  "[main].string IN ('one', 'two', 'three')");
    COMPARE_WHERE(str != "hi" && str.like("%s"),
                  "([main].string <> 'hi' AND [main].string LIKE '%s')");
}

void PhrasesTest::boolean()
{
    FieldPhrase<bool> b("main", "bool");

    auto p1 = b;
    auto p2 = !b;
    auto p3 = b == false;

    QTEST_ASSERT(p1.data);
    QTEST_ASSERT(p2.data);
    QTEST_ASSERT(p3.data);
}

void PhrasesTest::datetime()
{
    Generator g;

    FieldPhrase<QTime> time("main", "time");
    FieldPhrase<QDate> date("main", "date");
    FieldPhrase<QDateTime> datetime("main", "datetime");

    QDate d(2020, 2, 20);
    QTime t(12, 34, 56);
    QDateTime dt(d, t);

    auto p1 = time <= QTime::currentTime();
    auto p2 = time.addHours(2) < QTime::currentTime();
    auto p3 = date == QDate::currentDate();
    auto p4 = date.addDays(1) == QDate::currentDate();
    auto p5 = datetime > QDateTime::currentDateTime();
    auto p6 = datetime.addMonths(1) >= QDateTime::currentDateTime();
    auto p7 = time.between(QTime::currentTime().addSecs(-100), QTime::currentTime());
    auto p8 = time.hour() == 3;
    auto p9 = time = QTime::currentTime();

    COMPARE_WHERE(date.addDays(2) == d, "DATE([main].date,'+2 DAY') = '2020-02-20'");
    COMPARE_WHERE(time.addMinutes(-3) == t, "TIME([main].time,'-3 MINUTE') = '12:34:56'");
    COMPARE_WHERE(datetime.addMinutes(1) == dt, "DATETIME([main].datetime,'+1 MINUTE') = '2020-02-20 12:34:56'");
}

void PhrasesTest::extra()
{
    FieldPhrase<QUrl> url("main", "url");

    auto p1 = url == QUrl();
    auto p2 = url == "http://google.com";
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
