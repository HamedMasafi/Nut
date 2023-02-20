#include <QtTest>

#include <QMetaType>
#include <QList>
#include <QString>
#include <QObject>

#include "tablemodel.h"
#include <QtNut/sqlitegenerator.h>
#include <QtNut/sqlservergenerator.h>
#include <QtNut/mysqlgenerator.h>
#include <QtNut/postgresqlgenerator.h>

#include "tst_generators.h"

GeneratorsTest::GeneratorsTest(QObject *parent) : QObject(parent)
{

}


void GeneratorsTest::types(Nut::AbstractSqlGenerator *g, QString name)
{
    QList<QMetaType::Type> types;
    types
            << QMetaType::Bool

            << QMetaType::Char
            << QMetaType::SChar
            << QMetaType::UChar
            << QMetaType::QChar

            << QMetaType::Short
            << QMetaType::UShort
            << QMetaType::Int
            << QMetaType::UInt
            << QMetaType::Long
            << QMetaType::LongLong
            << QMetaType::ULong
            << QMetaType::ULongLong

            << QMetaType::Double
            << QMetaType::Float

            << QMetaType::QString
            << QMetaType::QStringList

            << QMetaType::QBitArray
            << QMetaType::QByteArray

            << QMetaType::QDate
            << QMetaType::QTime
            << QMetaType::QDateTime

            << QMetaType::QUrl
            << QMetaType::QColor

            << QMetaType::QPoint
            << QMetaType::QPointF
            << QMetaType::QPolygon
            << QMetaType::QPolygonF
            << QMetaType::QSize
            << QMetaType::QSizeF
            << QMetaType::QRect
            << QMetaType::QRectF
            << QMetaType::QLine
            << QMetaType::QLineF

               //             << QMetaType::QRegion
               //             << QMetaType::QImage
               //             << QMetaType::QPixmap
               //             << QMetaType::QLocale
               //             << QMetaType::QMatrix
               //             << QMetaType::QMatrix4x4
               //             << QMetaType::QVector2D
               //             << QMetaType::QVector3D
               //             << QMetaType::QVector4D
            << QMetaType::QJsonValue
            << QMetaType::QJsonObject
            << QMetaType::QJsonArray
            << QMetaType::QJsonDocument

            << QMetaType::QUuid
               //             << QMetaType::QByteArrayList
               ;

    Nut::FieldModel m;
    for (auto &t: types) {
        m.type = t;
        QString fn = g->fieldType(&m);

        QString tn = QString(QMetaType::typeName(t));
        if (!table.contains(tn))
            table.insert(tn, row());

        table[tn].set(name, fn);

        if (fn.isEmpty())
            qDebug() << "No rule for" << t << "(" << QMetaType::typeName(t) << ")";
        Q_ASSERT(!fn.isEmpty());
    }
}

void GeneratorsTest::test_sqlite()
{
    auto g = new Nut::SqliteGenerator;
    types(g, "sqlite");
    g->deleteLater();
}

void GeneratorsTest::test_sqlserver()
{
    auto g = new Nut::SqlServerGenerator;
    types(g, "mssql");
    g->deleteLater();
}

void GeneratorsTest::test_psql()
{
    auto g = new Nut::PostgreSqlGenerator;
    types(g, "psql");
    g->deleteLater();
}

void GeneratorsTest::test_mysql()
{
    auto g = new Nut::MySqlGenerator;
    types(g, "mysql");
    g->deleteLater();
}

void GeneratorsTest::cleanupTestCase()
{
    QMap<QString, row>::const_iterator i;
    QString p = "\n| Type  | Sqlite | MySql  | Postgresql| Ms Sql server |"
                  "\n|--------|--------|--------|--------|--------|";
    for (i = table.constBegin(); i != table.constEnd(); ++i) {
        p.append(QStringLiteral("\n|%1|%2|%3|%4|%5|")
                 .arg(i.key(), i.value().sqlite, i.value().mysql,
                     i.value().psql, i.value().mssql));
    }

    QFile file(NUT_PATH "/doc/datatypes.md");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(p.toUtf8());
        file.close();
    }
}

QTEST_MAIN(GeneratorsTest)

//#include "tst_GeneratorsTest.moc"
