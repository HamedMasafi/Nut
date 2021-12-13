#include <QtGlobal>

#ifdef DATABASE
#undef DATABASE
#endif

#define DATABASE                                                                                   \
    QLatin1String("DRIVER=SQL SERVER;SERVER=localhost;Trusted_Connection=true;Database=")          \
        + QStringLiteral("nut_test_%1_db")                                                         \
              .arg(QString::fromUtf8(metaObject()->className()))                                   \
              .toLower()

#define DRIVER QStringLiteral("QODBC")
#define HOST QLatin1String("localhost")
#define USERNAME QLatin1String("sa")
#define PASSWORD QLatin1String("NUT_sa_PASS_1_???")
