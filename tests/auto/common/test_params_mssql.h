#include <QtGlobal>

#ifdef DATABASE
#undef DATABASE
#endif

#define DATABASE                                                                                   \
    QStringLiteral("DRIVER={SQL Server Native Client 11.0};Server=localhost;DATABASE=nut_test_%1_db;UID=sa;PWD=NUT_sa_PASS_1_???;")      \
        .arg(QString::fromUtf8(metaObject()->className()).toLower())

#define DRIVER QStringLiteral("QODBC")
#define HOST QLatin1String("")
#define USERNAME QLatin1String("")
#define PASSWORD QLatin1String("")
