#include <QtGlobal>

#ifdef DATABASE
#define DBNAME DATABASE
#undef DATABASE
#endif

#define DATABASE QLatin1String("DRIVER={SQL SERVER};SERVER=.;DATABASE=;" DBNAME)

#define DRIVER QStringLiteral("QODBC")
#define HOST QLatin1String("localhost")
#define USERNAME QLatin1String()
#define PASSWORD QLatin1String()
