#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QPoint>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QUrl>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#ifdef QT_GUI_LIB
#include <QColor>
#include <QPolygon>
#endif

#include "table.h"

#define FIELD_Q(type) NUT_DECLARE_FIELD(q##type, f##type, f##type, setF##type)

class SampleTable : public NUT_WRAP_NAMESPACE(Table)
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(qint8, f_int8, f_int8, setInt8)
    NUT_DECLARE_FIELD(qint16, f_int16, f_int16, setInt16)
    NUT_DECLARE_FIELD(qint32, f_int32, f_int32, setInt32)
    NUT_DECLARE_FIELD(qint64, f_int64, f_int64, setInt64)

    NUT_DECLARE_FIELD(quint8, f_uint8, f_uint8, setUint8)
    NUT_DECLARE_FIELD(quint16, f_uint16, f_uint16, setUint16)
    NUT_DECLARE_FIELD(quint32, f_uint32, f_uint32, setUint32)
    NUT_DECLARE_FIELD(quint64, f_uint64, f_uint64, setUint64)

    NUT_DECLARE_FIELD(qreal, f_real, f_real, setReal)
    NUT_DECLARE_FIELD(float, f_float, f_float, setFloat)
//    NUT_DECLARE_FIELD(long double, fldouble, fldouble, setFldouble)
    NUT_DECLARE_FIELD(QString, f_string, f_string, setString)

    NUT_DECLARE_FIELD(QTime, f_time, f_time, setTime)
    NUT_DECLARE_FIELD(QDate, f_date, f_date, setDate)
    NUT_DECLARE_FIELD(QDateTime, f_dateTime, f_dateTime, setDateTime)

    NUT_DECLARE_FIELD(QUuid, f_uuid, f_uuid, setUuid)

    NUT_DECLARE_FIELD(QUrl, f_url, f_url, setUrl)

    NUT_DECLARE_FIELD(QJsonDocument, f_jsonDoc, f_jsonDoc, setJsonDoc)
    NUT_DECLARE_FIELD(QJsonObject, f_jsonObj, f_jsonObj, setJsonObj)
    NUT_DECLARE_FIELD(QJsonArray, f_jsonArray, f_jsonArray, setJsonArray)
    NUT_DECLARE_FIELD(QJsonValue, f_jsonValue, f_jsonValue, setJsonValue)

    NUT_DECLARE_FIELD(QStringList, f_stringList, f_stringList, setStringList)
    NUT_DECLARE_FIELD(QChar, f_qchar, f_qchar, setQchar)
#ifdef QT_GUI_LIB
    NUT_DECLARE_FIELD(QPoint, f_point, f_point, setPoint)
    NUT_DECLARE_FIELD(QPointF, f_pointf, f_pointf, setPointf)
    NUT_DECLARE_FIELD(QPolygon, f_polygon, f_polygon, setPolygon)
    NUT_DECLARE_FIELD(QPolygonF, f_polygonf, f_polygonf, setPolygonf)
    NUT_DECLARE_FIELD(QColor, f_color, f_color, setColor)
#endif
public:
    Q_INVOKABLE SampleTable(QObject *parent = Q_NULLPTR);
};

Q_DECLARE_METATYPE(SampleTable*)

#endif // SAMPLETABLE_H
