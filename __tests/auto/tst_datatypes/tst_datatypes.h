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
class DataTypesTest : public QObject
{
    Q_OBJECT
    DB db;

    qint8 f_int8;
    qint16 f_int16;
    qint32 f_int32;
    qint64 f_int64;
    quint8 f_uint8;
    quint16 f_uint16;
    quint32 f_uint32;
    quint64 f_uint64;
    qreal f_real;
    float f_float;

    QTime f_time;
    QDate f_date;
    QDateTime f_dateTime;

    QJsonDocument f_jsonDoc;
    QJsonObject f_jsonObj;
    QJsonArray f_jsonArray;
    QJsonValue f_jsonValue;

    QString f_string;
    QStringList f_stringList;

    QChar f_qchar;
    QUrl f_url;
    QUuid f_uuid;
#ifdef QT_GUI_LIB
    QPoint f_point;
    QPointF f_pointf;
    QPolygon f_polygon;
    QPolygonF f_polygonf;
    QColor f_color;
#endif

public:
    explicit DataTypesTest(QObject *parent = nullptr);

Q_SIGNALS:

private Q_SLOTS:
    void initTestCase();

    void insert();
    void retrive();
    void check();
    void cleanupTestCase();
};

#endif // MAINTEST_H
