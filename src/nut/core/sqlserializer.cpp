/**************************************************************************
**
** This file is part of Nut project.
** https://github.com/HamedMasafi/Nut
**
** Nut is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Nut is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Nut.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include "sqlserializer.h"

#include <QtCore/QUuid>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QSizeF>
#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QLineF>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QBitArray>
#include <QtCore/QBuffer>
#include "nut_p.h"
#include "nut_global.h"

#ifdef QT_GUI_LIB
#include <QtGui/QFont>
#include <QtGui/QPolygon>
#include <QtGui/QPolygonF>
#include <QtGui/QColor>
#include <QtGui/QImage>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QVector4D>
#endif

#define DATE_FORMAT QStringLiteral("yyyy-MM-dd")
#define TIME_FORMAT QStringLiteral("HH:mm:ss.zzz")

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

SqlSerializer::SqlSerializer()
{

}

SqlSerializer::~SqlSerializer()
{

}

QVariant SqlSerializer::fromString(const QString &value, const QMetaType::Type &type) const
{
    switch (type) {
    case QMetaType::Bool:       return !value.compare(QStringLiteral("true"), Qt::CaseInsensitive) || value == QStringLiteral("1");
    case QMetaType::Char:
    case QMetaType::SChar:
    case QMetaType::Int:        return value.toInt();
    case QMetaType::UChar:
    case QMetaType::UInt:       return value.toUInt();
    case QMetaType::Short:      return value.toShort();
    case QMetaType::UShort:     return value.toUShort();
    case QMetaType::Long:       return QVariant::fromValue(value.toLong());
    case QMetaType::ULong:      return QVariant::fromValue(value.toULong());
    case QMetaType::LongLong:   return value.toLongLong();
    case QMetaType::ULongLong:  return QVariant::fromValue(value.toULongLong());
    case QMetaType::Float:      return value.toFloat();
    case QMetaType::Double:     return value.toDouble();
    case QMetaType::QString:    return unescapeString(value);
    case QMetaType::QDate:      return QDate::fromString(value, DATE_FORMAT);
    case QMetaType::QTime:      return QTime::fromString(value, TIME_FORMAT);
    case QMetaType::QDateTime:  return QDateTime::fromString(value, DATE_FORMAT + QStringLiteral(" ") + TIME_FORMAT);
    case QMetaType::QUrl:       return QUrl(value);
    case QMetaType::QChar:      return value.at(0);
    case QMetaType::QStringList: {
        QStringList ret;
        QString copy(value);
        QString out;

        while (readString(copy, out) )
            ret.append(out);

        return ret;
    }

    case QMetaType::QPoint: {
        QList<int> parts = toListInt(value);
        if (parts.size() != 2)
            return QPoint();

        return QPoint(parts.at(0), parts.at(1));
    }
    case QMetaType::QSize: {
        QList<int> parts = toListInt(value);
        if (parts.size() != 2)
            return QSize();

        return QSize(parts.at(0), parts.at(1));
    }
    case QMetaType::QRect: {
        QList<int> parts = toListInt(value);
        if (parts.size() != 4)
            return QRect();

        return QRect(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
    }
    case QMetaType::QLine: {
        QList<int> parts = toListInt(value);
        if (parts.size() != 4)
            return QLine();

        return QLine(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
    }
    case QMetaType::QPointF: {
        QList<qreal> parts = toListReal(value);
        if (parts.size() != 2)
            return QPointF();

        return QPointF(parts.at(0), parts.at(1));
    }
    case QMetaType::QSizeF: {
        QList<qreal> parts = toListReal(value);
        if (parts.size() != 2)
            return QSizeF();

        return QSizeF(parts.at(0), parts.at(1));
    }
    case QMetaType::QRectF: {
        QList<qreal> parts = toListReal(value);
        if (parts.size() != 4)
            return QRectF();

        return QRectF(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
    }
    case QMetaType::QLineF: {
        QList<qreal> parts = toListReal(value);
        if (parts.size() != 4)
            return QLineF();

        return QLineF(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
    }

    case QMetaType::QUuid:
        return QUuid(value);

    case QMetaType::QByteArray:
        return value.toUtf8();

    case QMetaType::QJsonDocument:
        return QJsonDocument::fromJson(value.toUtf8());

    case QMetaType::QJsonObject:
        return QJsonDocument::fromJson(value.toUtf8()).object();

    case QMetaType::QJsonArray:
        return QJsonDocument::fromJson(value.toUtf8()).array();

    case QMetaType::QJsonValue: {
        if (value == QStringLiteral("true"))
            return QJsonValue(true);
        else if (value == QStringLiteral("false"))
            return QJsonValue(false);
        bool ok;
        int n = value.toInt(&ok);
        if (ok)
            return QJsonValue(n);

        double d = value.toDouble(&ok);
        if (ok)
            return QJsonValue(d);

        return QJsonValue(value);
    }

    case QMetaType::QBitArray: {
        QBitArray bita(value.size());
        for (int i = 0; i < value.size(); ++i) {
            if (value.at(i) == QStringLiteral("0"))
                bita.setBit(i, false);
            else
                bita.setBit(i, true);
        }
        return bita;
    }

    case QMetaType::QVariantMap: {
        QVariantMap ret;
        QStringList parts = value.split(QStringLiteral("\n"));
        for (auto &p: parts) {
            if (p.isEmpty())
                continue;
            QString name;
            QString value;
            if (readString(p, name) && readString(p, value))
                ret.insert(name, value);
        }
        return ret;
    }

    case QMetaType::QVariantList: {
        QVariantMap ret;
        QStringList parts = value.split(QStringLiteral("\n"));
        for (auto &p: parts) {
            if (p.isEmpty())
                continue;
            QString name;
            QString value;
            if (readString(p, name) && readString(p, value))
                ret.insert(name, value);
        }
        return ret;
    }

#ifdef QT_GUI_LIB
    case QMetaType::QVector2D: {
        QList<float> parts = toListFloat(value);
        if (parts.size() != 2)
            return QVector2D();

        return QVector2D(parts.at(0), parts.at(1));
    }
    case QMetaType::QVector3D: {
        QList<float> parts = toListFloat(value);
        if (parts.size() != 3)
            return QVector3D();

        return QVector3D(parts.at(0), parts.at(1), parts.at(2));
    }
    case QMetaType::QVector4D: {
        QList<float> parts = toListFloat(value);
        if (parts.size() != 4)
            return QVector4D();

        return QVector4D(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
    }

    case QMetaType::QImage: {
        QImage image;
        image.loadFromData(QByteArray::fromBase64(value.toLocal8Bit()));
        return image;
    }

    case QMetaType::QColor:
        return QColor(value);

    case QMetaType::QPolygon: {
        QStringList parts = value.split(QStringLiteral(" "));
        QPolygon pol;
        for (int i = 0; i < parts.count(); i++)
            pol.append(fromString(parts.at(i), QMetaType::QPoint).toPoint());
        return pol;
    }
    case QMetaType::QPolygonF: {
        QStringList parts = value.split(QStringLiteral(" "));
        QPolygonF pol;
        for (int i = 0; i < parts.count(); i++)
            pol.append(fromString(parts.at(i), QMetaType::QPointF).toPointF());
        return pol;
    }
    case QMetaType::QFont: {
        QFont f;
        f.fromString(value);
        return f;
    }
#endif
    default:
        return QVariant();
    }
}

QString SqlSerializer::toString(const QVariant &value) const
{
    auto type = METATYPE_ID(value);
    switch (type) {
    case QMetaType::Bool:
    case QMetaType::Char:
    case QMetaType::Short:
    case QMetaType::UShort:
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::Long:
    case QMetaType::ULong:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
    case QMetaType::Double:
    case QMetaType::Float:
    case QMetaType::QChar:
    case QMetaType::QUrl:
        return value.toString();

    case QMetaType::UChar:
    case QMetaType::SChar:
        return QString::number(value.toInt());

    case QMetaType::QString:
        return value.toString();

    case QMetaType::QStringList: {
        QString ret;
        QStringList sl = value.toStringList();
        for (auto &s: sl) {
            if (!ret.isEmpty())
                ret.append(QStringLiteral(" "));

            ret.append(QStringLiteral("\"") + escapeString(s) + QStringLiteral("\""));
        }

        return ret;
    }

    case QMetaType::QUuid:
        return value.toUuid().toString();

    case QMetaType::QByteArray:
        return QString::fromUtf8(value.toByteArray());

    case QMetaType::QDate:          return value.toDate().toString(DATE_FORMAT);
    case QMetaType::QTime:          return value.toTime().toString(TIME_FORMAT);
    case QMetaType::QDateTime:      return value.toDateTime().toString(DATE_FORMAT + QStringLiteral(" ") + TIME_FORMAT);

    case QMetaType::QPoint: {
        QPoint pt = value.toPoint();
        return fromVariantList({pt.x(), pt.y()});
    }
    case QMetaType::QPointF: {
        QPointF pt = value.toPointF();
        return fromVariantList({pt.x(), pt.y()});
    }
    case QMetaType::QSize: {
        QSize pt = value.toSize();
        return fromVariantList({pt.width(), pt.height()});
    }
    case QMetaType::QSizeF: {
        QSizeF pt = value.toSizeF();
        return fromVariantList({pt.width(), pt.height()});
    }
    case QMetaType::QRect: {
        QRect rc = value.toRect();
        return fromVariantList({rc.x(), rc.y(), rc.width(), rc.height()});
    }
    case QMetaType::QRectF: {
        QRectF rc = value.toRectF();
        return fromVariantList({rc.x(), rc.y(), rc.width(), rc.height()});
    }
    case QMetaType::QLine: {
        QLine rc = value.toLine();
        return fromVariantList({rc.x1(), rc.y1(), rc.x2(), rc.y2()});
    }
    case QMetaType::QLineF: {
        QLineF rc = value.toLineF();
        return fromVariantList({rc.x1(), rc.y1(), rc.x2(), rc.y2()});
    }
    case QMetaType::QJsonDocument:
        return QString::fromUtf8(value.toJsonDocument().toJson(QJsonDocument::Compact));

    case QMetaType::QJsonObject: {
        QJsonDocument doc;
        doc.setObject(value.toJsonObject());
        return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }

    case QMetaType::QJsonArray: {
        QJsonDocument doc;
        doc.setArray(value.toJsonArray());
        return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }

    case QMetaType::QJsonValue:
        return value.toJsonValue().toVariant().toString();

    case QMetaType::QBitArray: {
        QString ret;
        QBitArray bita = value.toBitArray();
        for (int i = 0; i < bita.size(); ++i)
            ret.append(bita.at(i) ? QStringLiteral("1") : QStringLiteral("0"));
        return ret;
    }
    case QMetaType::QVariantMap: {
        QString ret;
        QVariantMap map = value.toMap();
        for (auto i = map.begin(); i != map.end(); ++i) {
            if (!ret.isEmpty())
                ret.append(QStringLiteral("\n"));
            QVariant v = map.value(i.key());
            QString str = toString(v);
            ret.append(QStringLiteral("\"%1\" \"%2\"")
                           .arg(escapeString(i.key()), escapeString(str)));

        }
        return ret;
    }

#ifdef QT_GUI_LIB
    case QMetaType::QVector2D: {
        QVector2D vec = value.value<QVector2D>();
        return fromList(QList<float>() << vec.x() << vec.y());
    }
    case QMetaType::QVector3D: {
        QVector3D vec = value.value<QVector3D>();
        return fromList(QList<float>() << vec.x() << vec.y() << vec.z());
    }
    case QMetaType::QVector4D: {
        QVector4D vec = value.value<QVector4D>();
        return fromList(QList<float>() << vec.x() << vec.y() << vec.z() << vec.w());
    }

    case QMetaType::QImage: {
        QImage image = value.value<QImage>();
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        image.save(&buffer, "PNG");
        QString base64 = QString::fromLatin1(byteArray.toBase64().data());
        return base64;
    }

    case QMetaType::QColor:
        return value.value<QColor>().name();

    case QMetaType::QPolygon: {
        QStringList list;
        QPolygon pol = value.value<QPolygon>();
        QPoint pt;
        for (int i = 0; i < pol.size(); ++i) {
            pt = pol.at(i);
            list.append(toString(pt));
        }

        return list.join(QStringLiteral(" "));
    }
    case QMetaType::QPolygonF: {
        QStringList list;
        QPolygonF pol = value.value<QPolygonF>();
        QPointF pt;
        for (int i = 0; i < pol.size(); ++i) {
            pt = pol.at(i);
            list.append(toString(pt));
        }

        return list.join(QStringLiteral(" "));
    }
    case QMetaType::QFont:
        return value.value<QFont>().toString();
#endif

    case QMetaType::QVariantList: {
        auto l = value.toList();
        QString ret;

        for (auto &v: l) {
            if (!ret.isEmpty())
                ret.append(QStringLiteral(", "));
            ret.append(toString(v));
        }

        return QStringLiteral("(") + ret + QStringLiteral(")");
        break;
    }

    default:
        qWarning("The type (%s) does not supported",
                 METATYPE_TO_NAME(type));
        return QString();
    }
}

QList<int> SqlSerializer::toListInt(const QString &s) const
{
    return toListInt(s, QStringLiteral(","));
}

QList<int> SqlSerializer::toListInt(const QString &s, const QString &sep) const
{
    auto parts = s.split(sep);
    QList<int> ret;
    for (auto &p: parts) {
        bool ok;
        ret.append(p.toInt(&ok));
        if (!ok)
            return ret;
    }

    return ret;
}

QList<qreal> SqlSerializer::toListReal(const QString &s) const
{
    return toListReal(s, QStringLiteral(","));
}

QString SqlSerializer::fromList(const QList<int> &list) const
{
    QString ret;
    for (auto &n: list) {
        if (!ret.isEmpty())
            ret.append(QStringLiteral(","));
        ret.append(QString::number(n));
    }
    return ret;
}

QList<qreal> SqlSerializer::toListReal(const QString &s, const QString &sep) const
{
    auto parts = s.split(sep);
    QList<qreal> ret;
    for (auto &p: parts) {
        bool ok;
        ret.append(p.toDouble(&ok));
        if (!ok)
            return ret;
    }

    return ret;
}

QList<float> SqlSerializer::toListFloat(const QString &s) const
{
    auto parts = s.split(QStringLiteral(","));
    QList<float> ret;
    for (auto &p: parts) {
        bool ok;
        ret.append(p.toFloat(&ok));
        if (!ok)
            return ret;
    }

    return ret;
}

QString SqlSerializer::fromList(const QList<qreal> &list) const
{
    QString ret;
    for (auto &n: list) {
        if (!ret.isEmpty())
            ret.append(QStringLiteral(","));
        ret.append(QString::number(n, 'f', -1));
    }
    return ret;
}

QString SqlSerializer::fromList(const QList<float> &list) const
{
    QString ret;
    for (auto &n: list) {
        if (!ret.isEmpty())
            ret.append(QStringLiteral(","));
        ret.append(QString::number(static_cast<double>(n)));
    }
    return ret;
}

QString SqlSerializer::fromVariantList(const QVariantList &list) const
{
    QString ret;
    for (auto &n: list) {
        if (!ret.isEmpty())
            ret.append(QStringLiteral(","));
        ret.append(n.toString());
    }
    return ret;
}

#define CASE_W(o, r) \
case o:                  \
    ret.append(QStringLiteral(r));     \
    break;
QString SqlSerializer::escapeString(const QString &str) const
{
//    QString ret;
//    for (int i = 0; i < str.length(); ++i) {
//        switch (str.at(i).cell()) {
//            CASE_W('\\', "\\\\")
//            CASE_W('\r', "\\r")
//            CASE_W('\n', "\\n")
//            CASE_W('\a', "\\a")
//            CASE_W('\b', "\\b")
//            CASE_W('\f', "\\f")
//            //        CASE_W('\'', "\\'")
//            CASE_W('\t', "\\t")
//            CASE_W('\v', "\\v")
//            CASE_W('\"', "\\\"")

//        default:
//            ret.append(str.at(i));
//        }
//    }
//    return ret;
    QString ret(str);
    return ret.replace(QStringLiteral("'"), QStringLiteral("''"));
}

QString SqlSerializer::unescapeString(const QString &str) const
{
//    QString ret;
//    for (int i = 0; i < str.length(); ++i) {
//        if (str.at(i) == '\\' && str.length() > i) {
//            switch (str.at(++i).cell()) {
//            case '\\':
//                ret.append(QStringLiteral("\\"));
//                break;
//            case 'r':
//                ret.append(QStringLiteral("\r"));
//                break;
//            case 'n':
//                ret.append(QStringLiteral("\n"));
//                break;
//            case 'a':
//                ret.append(QStringLiteral("\a"));
//                break;
//            case 'b':
//                ret.append(QStringLiteral("b"));
//                break;
//            case 'f':
//                ret.append(QStringLiteral("\f"));
//                break;
//                //            case '\'':
//                //                ret.append(QStringLiteral("\\'"));
//                //                break;
//            case 't':
//                ret.append(QStringLiteral("\t"));
//                break;
//            case 'v':
//                ret.append(QStringLiteral("\v"));
//                break;
//            case '"':
//                ret.append(QStringLiteral("\\\""));
//                break;

//            default:
//                ret.append(str.at(i));
//            }
//        } else {
//            ret.append(str.at(i));
//        }
//    }

//    return ret;
    QString ret(str);
    return ret.replace(QStringLiteral("''"), QStringLiteral("'"));
}

bool SqlSerializer::readString(QString &text, QString &out) const
{
    int start = -1;
    int end = -1;

    for (int i = 0; i < text.length(); ++i) {
        if (text.at(i) == QLatin1Char('"') && (i == 0 || text.at(i - 1) != QStringLiteral("\\"))) {
            if (start == -1)
                start = i;
            else
                end = i;
        }
        if (end != -1){
            out = text.mid(start + 1, end - start - 1);
            text = text.mid(end + 1);
            return true;
        }
    }
    return false;
}

QVariant SqlSerializer::deserialize(const QString &value, const QMetaType::Type &type) const
{
    return fromString(unescapeString(value), type);
}

QString SqlSerializer::serialize(const QVariant &value) const
{
    return escapeString(toString(value));
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
