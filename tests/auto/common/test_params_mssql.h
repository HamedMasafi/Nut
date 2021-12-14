#include <QtGlobal>

#ifdef DATABASE
#undef DATABASE
#endif

#define DATABASE                                                                                   \
    QStringLiteral("DRIVER={SQL Server Native Client 11.0};"                                       \
                   "Server=(LocalDB)\\MSSQLLocalDB;"                                                \
                   "Integrated Security=true;"                                                     \
                   "Database=nut_test_%1_db")                                                      \
        .arg(QString::fromUtf8(metaObject()->className()).toLower())

#define DRIVER QStringLiteral("QODBC")
#define HOST QStringLiteral("localhost")
#define USERNAME QLatin1String("")
#define PASSWORD QLatin1String("")
