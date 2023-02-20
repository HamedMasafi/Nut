#ifndef TST_PROPERTIES_H
#define TST_PROPERTIES_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "sampledatabase.h"

class PropertiesTest : public QObject
{
    Q_OBJECT
    SampleDataBase db;

public:
    explicit PropertiesTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();
    void insert();
    void select();
    void parallelUpdate();
};

#endif // TST_PROPERTIES_H
