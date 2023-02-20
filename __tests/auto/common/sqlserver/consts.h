#ifndef DATABASE_CONSTS_H
#define DATABASE_CONSTS_H

#define DRIVER "QODBC"
#define DATABASE QString("DRIVER={SQL Server};Server=.;Database=%1;Uid=sa;Port=1433;Pwd=qwe123!@#;WSID=.") \
    .arg(QString("nut_test_%1_db").arg(metaObject()->className()).toLower())
#define HOST QStringLiteral(".")
#define USERNAME QString()
#define PASSWORD QString()

#endif // DATABASE_CONSTS_H
