#ifndef DATABASE_CONSTS_H
#define DATABASE_CONSTS_H

#define DRIVER QStringLiteral("QSQLITE")
#define DATABASE QStringLiteral("nut_test_%1_db").arg(metaObject()->className()).toLower())
#define HOST QString()
#define USERNAME QString()
#define PASSWORD QString()

#endif // DATABASE_CONSTS_H
