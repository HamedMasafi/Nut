#ifndef TST_QTTYPES_H
#define TST_QTTYPES_H

#include "db.h"

#include <QtTest>
class QtTypes : public QObject
{
    Q_OBJECT

    DB db;

public:
    QtTypes();
    ~QtTypes();

private slots:
    void insert();
    void update();

};

#endif // TST_QTTYPES_H
