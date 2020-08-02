#ifndef CONSTS_H
#define CONSTS_H

#include <qsystemdetection.h>
#include <qcompilerdetection.h>

#define REGISTER(x) qDebug() << (#x) << "type id:" << qMetaTypeId<x*>()
#define PRINT(x)
//qDebug() << (#x "=") << (x);
#define TIC()  QElapsedTimer timer; timer.start()
#define TOC()  qDebug() << QStringLiteral("Elapsed time: %1ms for %2") \
    .arg(timer.elapsed() / 1000.) \
    .arg(__func__)

#define DRIVER QStringLiteral("QSQLITE")
#define DATABASE QStringLiteral("nut_test_%1_db").arg(metaObject()->className()).toLower()
#define HOST QString()
#define USERNAME QString()
#define PASSWORD QString()

#ifdef Q_OS_LINUX
#   define OS "Linux"
#elif defined(Q_OS_WIN)
#   define OS "Windows"
#elif defined(Q_OS_OSX)
#   define OS "macOS"
#else
#   define OS "Unknown"
#endif

#ifdef Q_CC_GNU
#   ifdef Q_CC_MINGW
#       define CC "MinGW"
#   else
#       define CC "GNU"
#   endif
#elif defined (Q_CC_MSVC)
#   define CC "msvc"
#elif defined (Q_CC_CLANG)
#   define CC "clang"
#else
#   define CC "Unknown"
#endif

#define PRINT_FORM(db) \
    qDebug() << "\n\n****************************"                             \
             << "\nAll tests passed,"                                          \
             << "please fill in bellow form and email it to me at"             \
             << "hamed.masafi@gmail.com"                                       \
             << "\n\tDriver:" << db.driver()                                   \
             << "\n\tOS: " OS " (version: ________)"                           \
             << "\n\tCompiler: " CC " (version: ________)"                     \
             << "\n\tQt version: " QT_VERSION_STR                              \
             << "\n\tTest:" << metaObject()->className()                       \
             << "\n****************************\n";


#endif // CONSTS_H
