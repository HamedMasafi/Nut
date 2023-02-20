#ifndef JOINTEST_H
#define JOINTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "weblogdatabase.h"

class JoinTest : public QObject
{
    Q_OBJECT
    WeblogDatabase db;

public:
    explicit JoinTest(QObject *parent = 0);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();

    void join();
    void join2();
};

#endif // JOINTEST_H
