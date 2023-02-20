#ifndef TST_TESTJSON_H
#define TST_TESTJSON_H

#include "db.h"

#include <QObject>

namespace Nut {
class Database;
}
class TestJson : public QObject
{
    Q_OBJECT

    DB db;

    void initDb(Nut::Database &db);

    int id;
public:
    TestJson();
    ~TestJson();

private Q_SLOTS:
    void initTestCase();

    void store();

    void cleanupTestCase();

};

#endif // TST_TESTJSON_H
