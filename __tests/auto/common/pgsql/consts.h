#ifndef DATABASE_CONSTS_H
#define DATABASE_CONSTS_H

#define DRIVER QStringLiteral("QPSQL")
#define DATABASE QStringLiteral("nut_test_%1_db").arg(metaObject()->className()).toLower())
#define HOST QString()
#define USERNAME QStringLiteral("postgres")
#define PASSWORD QStringLiteral("root")

#endif // DATABASE_CONSTS_H
