#ifndef MAINTEST_H
#define MAINTEST_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#ifdef QT_GUI_LIB
#include <QColor>
#include <QPolygonF>
#endif
#include <QUrl>
#include <QUuid>

#include "db.h"
class DateTimeTest : public QObject
{
    Q_OBJECT
    DB db;

    QDateTime _baseDateTime;

public:
    explicit DateTimeTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();
    void dateAdd();
    void timeAdd();
    void dateTimeAdd();
    void datePart();
    void timePart();
    void dateTimePart();
    void cleanupTestCase();
};

#endif // MAINTEST_H
