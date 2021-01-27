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

signals:

private slots:
    void initTestCase();
};

#endif // TST_PROPERTIES_H
