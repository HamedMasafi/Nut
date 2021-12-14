#include <QtGlobal>

#ifdef DATABASE
#undef DATABASE
#endif

#define DATABASE                                                                                   \
    QStringLiteral("DRIVER={SQL Server Native Client 11.0};Server=.;Database=nut_test_%1_db")      \
        .arg(QString::fromUtf8(metaObject()->className()).toLower())

#define DRIVER QStringLiteral("QODBC")
#define HOST QStringLiteral("localhost")
#define USERNAME QStringLiteral("sa")
#define PASSWORD QStringLiteral("NUT_sa_PASS_1_???")
