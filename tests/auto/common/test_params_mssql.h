#include <QtGlobal>

#ifdef DATABASE
#define DBNAME DATABASE
#undef DATABASE
#endif

#define DATABASE QLatin1String("DRIVER={SQL SERVER};SERVER=.;UID=sa;PWD=NUT_SA_PASS;DATABASE=") + DBNAME

#define DRIVER QStringLiteral("QODBC")
#define HOST QLatin1String("localhost")
#define USERNAME QLatin1String("sa")
#define PASSWORD QLatin1String("NUT_SA_PASS")
