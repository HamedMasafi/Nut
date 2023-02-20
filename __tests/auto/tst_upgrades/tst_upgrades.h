#ifndef TST_UPGRADES_H
#define TST_UPGRADES_H

#include <QObject>

namespace Nut {
class Database;
}
class Upgrades : public QObject
{
    Q_OBJECT

    void initDb(Nut::Database &db);

    int id;
public:
    Upgrades();
    ~Upgrades();

private Q_SLOTS:
    void initTestCase();

    void version1();
    void version2();
    void version3();

    void cleanupTestCase();

};

#endif // TST_UPGRADES_H
