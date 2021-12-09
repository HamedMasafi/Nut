#include <QtGlobal>

#ifdef DATABASE
#define DBNAME DATABASE
#endif

#define DATABASE                                                                                   \
    QLatin1String("DRIVER={SQL SERVER};SERVER=.;UID=sa;PWD=NUT_SA_PASS;DATABASE=")                 \
        + QStringLiteral("nut_test_%1_db")                                                         \
              .arg(QString::fromUtf8(metaObject()->className()))                                   \
              .toLower()

#define DRIVER QStringLiteral("QODBC")
#define HOST QLatin1String("localhost")
#define USERNAME QLatin1String("sa")
#define PASSWORD QLatin1String("NUT_SA_PASS")
