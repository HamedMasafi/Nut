#ifndef TST_GENERATORS_H
#define TST_GENERATORS_H

#include <QMap>
#include <QObject>

namespace Nut {
class AbstractSqlGenerator;
}

class GeneratorsTest : public QObject
{
    Q_OBJECT

    struct row {
        QString sqlite;
        QString psql;
        QString mysql;
        QString mssql;
        void set(QString name, QString value) {
            if (name == "sqlite")
                sqlite = value.toUpper();
            else if (name == "psql")
                psql = value.toUpper();
            else if (name == "mysql")
                mysql = value.toUpper();
            else if (name == "mssql")
                mssql = value.toUpper();
        }
    };
    QMap<QString, row> table;

public:
    explicit GeneratorsTest(QObject *parent = nullptr);

    void types(Nut::AbstractSqlGenerator *g, QString name);

private Q_SLOTS:
    void test_sqlite();
    void test_psql();
    void test_sqlserver();
    void test_mysql();

    void cleanupTestCase();

};

#endif // TST_GENERATORS_H
