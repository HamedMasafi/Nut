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

#include <QtCore/QDateTime>
#include <QtCore/QPoint>

#ifdef QT_GUI_LIB
#   include <QtGui/QPolygon>
#   include <QtGui/QPolygonF>
#endif
#include <QtCore/QVariant>
#include <QtCore/QJsonDocument>

#include "postgresqlgenerator.h"
#include "table.h"
#include "tablemodel.h"
#include "sqlserializer.h"
#include "nut_p.h"

NUT_BEGIN_NAMESPACE

bool PostgreSqlGenerator::readInsideParentese(QString &text, QString &out)
{
    int start = -1;
    int end = -1;
    int pc = 0;
    for (int i = 0; i < text.length(); ++i) {
        QChar ch = text.at(i);

        if (ch == QLatin1Char('(')) {
            if (start == -1)
                start = i;
            pc++;
        }
        if (ch == QLatin1Char(')')) {
            pc--;

            if (!pc && end == -1)
                end = i;
        }
        if (start != -1 && end != -1){
            out = text.mid(start + 1, end - start - 1);
            text = text.mid(end + 1);
            return true;
        }
    }
    return false;
}

bool PostgreSqlGenerator::isPostGisType(const QMetaType::Type &t) const
{
    return t == QMetaType::QPoint
           || t == QMetaType::QPointF
           || t == QMetaType::QRect
           || t == QMetaType::QRectF
           || t == QMetaType::QPolygon
           || t == QMetaType::QPolygonF;
}

PostgreSqlGenerator::PostgreSqlGenerator(Database *parent) : AbstractSqlGenerator (parent)
{

}

QString PostgreSqlGenerator::fieldType(FieldModel *field)
{
    QString dbType;

    switch (field->type) {
    case QMetaType::Bool:
        dbType = QStringLiteral("BOOLEAN");
        break;

    case QMetaType::QBitArray:
    case QMetaType::QByteArray:
        dbType = QStringLiteral("BYTEA");
        break;
    case QMetaType::QDate:
        dbType = QStringLiteral("DATE");
        break;
    case QMetaType::QDateTime:
        dbType = QStringLiteral("TIMESTAMP");
        break;
    case QMetaType::QTime:
        dbType = QStringLiteral("TIME");
        break;

    case QMetaType::SChar:
    case QMetaType::UChar:
    case QMetaType::Short:
    case QMetaType::UShort:
        dbType = QStringLiteral("SMALLINT");
        break;

    case QMetaType::Float:
        dbType = QStringLiteral("FLOAT");
        break;

    case QMetaType::Double:
        dbType = QStringLiteral("REAL");
        break;

    case QMetaType::Int:
    case QMetaType::UInt:
        if(field->isAutoIncrement)
            dbType = QStringLiteral("SERIAL");
        else
            dbType = QStringLiteral("INTEGER");
        break;

    case QMetaType::Long:
    case QMetaType::ULong:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        if(field->isAutoIncrement)
            dbType = QStringLiteral("BIGSERIAL");
        else
            dbType = QStringLiteral("BIGINT");
        break;

    case QMetaType::Char:
    case QMetaType::QChar:
        return QStringLiteral("CHAR(1)");

    case QMetaType::QString:
        if(field->length)
            dbType = QStringLiteral("VARCHAR(%1)").arg(field->length);
        else
            dbType = QStringLiteral("TEXT");
        break;

    case QMetaType::QPoint:
    case QMetaType::QPointF:
        dbType = QStringLiteral("POINT");
        break;

    case QMetaType::QUuid:
        dbType = QStringLiteral("UUID");
        break;

    case QMetaType::QPolygon:
    case QMetaType::QPolygonF:
        dbType = QStringLiteral("POLYGON");
        break;

    case QMetaType::QLine:
    case QMetaType::QLineF:
        return QStringLiteral("LINE");

    case QMetaType::QRect:
    case QMetaType::QRectF:
        return QStringLiteral("BOX");

    case QMetaType::QJsonArray:
    case QMetaType::QJsonValue:
    case QMetaType::QJsonObject:
    case QMetaType::QJsonDocument:
        return QStringLiteral("JSONB");

    case QMetaType::QStringList:
        return QStringLiteral("TEXT[]");

    case QMetaType::QSize:
    case QMetaType::QSizeF:
    case QMetaType::QUrl:
    case QMetaType::QColor:
        return QStringLiteral("TEXT");

    default:
        dbType = QString();
    }

    return dbType;
}

QString PostgreSqlGenerator::diff(FieldModel *oldField, FieldModel *newField)
{
    QString sql = QString();
    if(oldField && newField)
        if(*oldField == *newField)
            return QString();

    if(!newField){
        sql = QStringLiteral("DROP COLUMN ") + oldField->name;
    }else{
        if(oldField){
            sql = QStringLiteral("ALTER COLUMN ");
            sql.append(newField->name + QStringLiteral(" TYPE ") + fieldType(newField));
        } else {
            sql = QStringLiteral("ADD COLUMN ");
            sql.append(fieldDeclare(newField));
        }
    }
    return sql;
}

QString PostgreSqlGenerator::escapeValue(const QVariant &v) const
{
    if (VARIANT_TYPE_COMPARE(v, Time))
        return v.toTime().toString(QStringLiteral("''HH:mm:ss''"));

    if (VARIANT_TYPE_COMPARE(v, Date))
        return v.toDate().toString(QStringLiteral("''yyyy-MM-dd''"));

    if (VARIANT_TYPE_COMPARE(v, DateTime))
        return v.toDateTime().toString(QStringLiteral("''yyyy-MM-dd HH:mm:ss''"));

    if (VARIANT_TYPE_COMPARE(v, StringList))
        return QStringLiteral("'{")
                                       + v.toStringList().join(QStringLiteral(","))
                                       + QStringLiteral("}'");

    if (VARIANT_TYPE_COMPARE(v, Point)) {
        QPoint pt = v.toPoint();
        return QStringLiteral("point(%1, %2)").arg(pt.x()).arg(pt.y());
    }
    if (VARIANT_TYPE_COMPARE(v, PointF)) {
        QPointF pt = v.toPointF();
        return QStringLiteral("point(%1, %2)").arg(pt.x()).arg(pt.y());
    }
    if (v.userType() == QMetaType::QJsonDocument) {
        return QStringLiteral("'")
            + QString::fromUtf8(v.toJsonDocument().toJson(QJsonDocument::Compact))
            + QStringLiteral("'");
    }

#ifdef QT_GUI_LIB
    if (v.typeId() == QMetaType::QPolygon) {
        QString ret;
        QPoint pt;
        QPolygon pol = v.value<QPolygon>();
        for (int i = 0; i < pol.size(); ++i) {
            pt = pol.at(i);
            if (!ret.isEmpty())
                ret.append(QStringLiteral("),("));
            ret.append(QString::number(pt.x())
                       + QStringLiteral(", ") + QString::number(pt.y()));
        }
        return QStringLiteral("'((") + ret + QStringLiteral("))'");
    }
    if (v.typeId() == QMetaType::QPolygonF) {
        QString ret;
        QPointF pt;
        QPolygonF pol = v.value<QPolygonF>();
        for (int i = 0; i < pol.size(); ++i) {
            pt = pol.at(i);
            if (!ret.isEmpty())
                ret.append(QStringLiteral("),("));
            ret.append(QString::number(pt.x())
                       + QStringLiteral(", ") + QString::number(pt.y()));
        }
        return QStringLiteral("'((") + ret + QStringLiteral("))'");
    }
#endif

    return AbstractSqlGenerator::escapeValue(v);
}

QVariant PostgreSqlGenerator::unescapeValue(const QMetaType::Type &type, const QVariant &dbValue)
{
    if (type == QMetaType::QDateTime)
        return dbValue.toDateTime();

    if (type == QMetaType::QTime)
        return dbValue.toTime();

    if (type == QMetaType::QDate)
        return dbValue.toDate();

    if (type == QMetaType::QPoint)
        return AbstractSqlGenerator::unescapeValue(QMetaType::QPoint,
                                               dbValue.toString()
                                                   .replace(QStringLiteral("("),
                                                            QStringLiteral(""))
                                                   .replace(QStringLiteral(")"),
                                                            QStringLiteral("")));
    if (type == QMetaType::QPointF)
        return AbstractSqlGenerator::unescapeValue(QMetaType::QPointF,
                                               dbValue.toString()
                                                   .replace(QStringLiteral("("),
                                                            QStringLiteral(""))
                                                   .replace(QStringLiteral(")"),
                                                            QStringLiteral("")));
    if (type == QMetaType::QStringList)
        return dbValue.toString()
            .replace(QStringLiteral("{"), QStringLiteral(""))
            .replace(QStringLiteral("}"), QStringLiteral(""))
            .split(QStringLiteral(","));

#ifdef QT_GUI_LIB
    if (type == QMetaType::QPolygon) {
        QString p;
        QString ref = dbValue.toString();
        QPolygon pol;
        if (!readInsideParentese(ref, p))
            return pol;

        ref = p;
        while (readInsideParentese(ref, p)) {
            QList<int> l = _serializer->toListInt(p);
            if (l.count() != 2)
                return QPolygon();
            pol.append(QPoint(l.at(0), l.at(1)));
        }
        return pol;
    }
    if (type == QMetaType::QPolygonF) {
        QString p;
        QString ref = dbValue.toString();
        QPolygonF pol;
        if (!readInsideParentese(ref, p))
            return pol;

        ref = p;
        while (readInsideParentese(ref, p)) {
            QList<qreal> l = _serializer->toListReal(p);
            if (l.count() != 2)
                return QPolygonF();
            pol.append(QPointF(l.at(0), l.at(1)));
        }
        return pol;
    }
#endif
    return AbstractSqlGenerator::unescapeValue(type, dbValue);
}

QString PostgreSqlGenerator::createConditionalPhrase(const PhraseData *d) const
{
    if (!d)
        return QString();

    PhraseData::Condition op = d->operatorCond;
    //apply not (!)
    if (d->isNot) {
        if (op < 20)
            op = static_cast<PhraseData::Condition>((op + 10) % 20);
    }

    if (d->type == PhraseData::WithVariant) {
        if (isPostGisType(METATYPE_ID(d->operand)) && d->operatorCond == PhraseData::Equal) {
            return QStringLiteral("%1 ~= %2")
                    .arg(AbstractSqlGenerator::createConditionalPhrase(d->left),
                         escapeValue(d->operand));
        }
        switch (op) {
        case PhraseData::AddYears:
        case PhraseData::AddYearsDateTime:
        case PhraseData::AddMonths:
        case PhraseData::AddMonthsDateTime:
        case PhraseData::AddDays:
        case PhraseData::AddDaysDateTime:
        case PhraseData::AddHours:
        case PhraseData::AddHoursDateTime:
        case PhraseData::AddMinutes:
        case PhraseData::AddMinutesDateTime:
        case PhraseData::AddSeconds:
        case PhraseData::AddSecondsDateTime:
            return QStringLiteral("%1 + interval '%2 %3'")
                    .arg(createConditionalPhrase(d->left),
                         d->operand.toString(),
                         AbstractSqlGenerator::dateTimePartName(op));

        default:
            break;
        }
    }

    if (d->type == PhraseData::WithoutOperand) {
        switch (op) {
        case PhraseData::DatePartYear:
        case PhraseData::DatePartMonth:
        case PhraseData::DatePartDay:
        case PhraseData::DatePartHour:
        case PhraseData::DatePartMinute:
        case PhraseData::DatePartSecond:
            return QStringLiteral("date_part('%2', %1)")
                    .arg(createConditionalPhrase(d->left),
                         AbstractSqlGenerator::dateTimePartName(op));

        default:
            break;
        }
    }

    return AbstractSqlGenerator::createConditionalPhrase(d);
}

NUT_END_NAMESPACE
