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

#include "mysqlgenerator.h"
#include "tablemodel.h"

#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QUuid>

#ifdef QT_GUI_LIB
#   include <QtGui/QPolygon>
#   include <QtGui/QPolygonF>
#endif

#include "sqlserializer.h"
#include "nut_p.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

MySqlGenerator::MySqlGenerator(Database *parent) : AbstractSqlGenerator(parent)
{

}

QString MySqlGenerator::fieldType(FieldModel *field)
{
    QString dbType;

    switch (field->type) {
    case QMetaType::Bool:        return QStringLiteral("BOOLEAN");
    case QMetaType::Char:
    case QMetaType::QChar:       return QStringLiteral("CHAR(1)");
    case QMetaType::SChar:
    case QMetaType::UChar:       return QStringLiteral("TINYINT");
    case QMetaType::Short:
    case QMetaType::UShort:      return QStringLiteral("SMALLINT");
    case QMetaType::UInt:
    case QMetaType::Int:
        dbType = QStringLiteral("INT");
        if(field->isAutoIncrement)
            dbType += QStringLiteral(" AUTO_INCREMENT");
        break;
    case QMetaType::Long:
    case QMetaType::ULong:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return QStringLiteral("BIGINT");

    case QMetaType::Float:
        return QStringLiteral("FLOAT");

    case QMetaType::Double:
        return QStringLiteral("REAL");

    case QMetaType::QBitArray:      return QStringLiteral("VARBINARY");
    case QMetaType::QByteArray:     return QStringLiteral("BLOB");
    case QMetaType::QDate:          return QStringLiteral("DATE");
    case QMetaType::QTime:          return QStringLiteral("TIME");
    case QMetaType::QDateTime:      return QStringLiteral("DATETIME");

    case QMetaType::QString:
        if(field->length)
            dbType = QStringLiteral("VARCHAR(%1)").arg(field->length);
        else
            dbType = QStringLiteral("TEXT");
        break;

    case QMetaType::QUuid:
        dbType = QStringLiteral("VARCHAR(38)");
        break;

    case QMetaType::QPolygon:
    case QMetaType::QPolygonF:
//        dbType = "POLYGON";
//        break;


    case QMetaType::QPoint:
    case QMetaType::QPointF:
//        dbType = "POINT";
//        break;
    case QMetaType::QSize:
    case QMetaType::QSizeF:
    case QMetaType::QRect:
    case QMetaType::QRectF:
    case QMetaType::QLine:
    case QMetaType::QLineF:
    case QMetaType::QColor:
    case QMetaType::QUrl:
    case QMetaType::QJsonArray:
    case QMetaType::QJsonValue:
    case QMetaType::QJsonObject:
    case QMetaType::QJsonDocument:
    case QMetaType::QStringList:    return QStringLiteral("TEXT");

    default:
        qWarning("Type %s::%s(%d) is not supported",
                 qPrintable(field->name),
                 METATYPE_TO_NAME(field->type),
                 field->type);
        dbType = QString();
    }

//    if(field->typeName == QStringLiteral("Nut::DbGeography"))
//        dbType = "GEOMETRY";

    return dbType;
}

QString MySqlGenerator::escapeValue(const QVariant &v) const
{
    if (VARIANT_TYPE_COMPARE_X(v, Bool, Bool))
        return v.toBool() ? QStringLiteral("1") : QStringLiteral("0");

    if (VARIANT_TYPE_COMPARE(v, Time))
        return v.toTime().toString(QStringLiteral("''HH:mm:ss''"));

    if (VARIANT_TYPE_COMPARE(v, Date))
        return v.toDate().toString(QStringLiteral("''yyyy-MM-dd''"));

    if (VARIANT_TYPE_COMPARE(v, DateTime))
        return v.toDateTime().toString(QStringLiteral("''yyyy-MM-dd HH:mm:ss''"));

    if (VARIANT_TYPE_COMPARE(v, Uuid))
        return QStringLiteral("'") + v.toUuid().toString(QUuid::Id128) + QStringLiteral("'");

//#ifdef QT_GUI_LIB
//    if (v.type() == QVariant::Polygon) {
//        QString ret;
//        QPoint pt;
//        QPolygon pol = v.value<QPolygon>();
//        for (int i = 0; i < pol.size(); ++i) {
//            pt = pol.at(i);
//            if (!ret.isEmpty())
//                ret.append(", ");
//            ret.append(QString::number(pt.x()) + " " + QString::number(pt.y()));
//        }

//        return "GeomFromText('POLYGON(" + ret + "))')";
//    }
//    if (v.type() == QVariant::PolygonF) {
//        QString ret;
//        QPointF pt;
//        QPolygonF pol = v.value<QPolygonF>();
//        for (int i = 0; i < pol.size(); ++i) {
//            pt = pol.at(i);
//            if (!ret.isEmpty())
//                ret.append("),(");
//            ret.append(QString::number(pt.x()) + " " + QString::number(pt.y()));
//        }
//        return "GeomFromText('POLYGON(" + ret + "))')";
//    }
//#endif
//    switch (v.type()) {
//    case QMetaType::QPoint: {
//        QPoint pt = v.toPoint();
//        return QString("GeomFromText('POINT(%1 %2)',0)").arg(pt.x()).arg(pt.y());
//    }

//    case QMetaType::QPointF: {
//        QPointF pt = v.toPointF();
//        return QString("GeomFromText('POINT(%1 %2)',0)").arg(pt.x()).arg(pt.y());
//    }

//    default:
    return AbstractSqlGenerator::escapeValue(v);
//    }
}

QVariant MySqlGenerator::unescapeValue(const QMetaType::Type &type, const QVariant &dbValue)
{

//#ifdef QT_GUI_LIB
//    if (type == QMetaType::QPolygon) {
//        qDebug() << "p=" << dbValue;
//        QString p;
//        QString ref = dbValue.toString();
//        QPolygon pol;
//        if (!readInsideParentese(ref, p))
//            return pol;
//        QStringList parts = p.split(",");
//        for (auto &v: parts) {
//            QList<int> l = _serializer->toListInt(p.trimmed(), " ");
//            if (l.count() != 2)
//                return QPolygon();
//            pol.append(QPoint(l.at(0), l.at(1)));
//        }
//        return pol;
//    }
//    if (type == QMetaType::QPolygonF) {
//        QString p;
//        QString ref = dbValue.toString();
//        QPolygonF pol;
//        if (!readInsideParentese(ref, p))
//            return pol;

//        QStringList parts = p.split(",");
//        for (auto &v: parts) {
//            QList<qreal> l = _serializer->toListReal(p.trimmed(), " ");
//            if (l.count() != 2)
//                return QPolygonF();
//            pol.append(QPointF(l.at(0), l.at(1)));
//        }
//        return pol;
//    }
//#endif

    if (type == QMetaType::QDateTime)
        return dbValue.toDateTime();

    if (type == QMetaType::QTime)
        return dbValue.toTime();

    if (type == QMetaType::QDate)
        return dbValue.toDate();

    if (type == QMetaType::QUuid)
        return dbValue.toUuid();

    return AbstractSqlGenerator::unescapeValue(type, dbValue);
}

bool MySqlGenerator::readInsideParentese(QString &text, QString &out)
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
//QString MySqlGenerator::phrase(const PhraseData *d) const
//{
//    if (d->operatorCond == PhraseData::Distance) {
//        return QString("ST_Distance(%1, %2)")
//                .arg(d->left->text)
//                .arg(escapeValue(d->operand.toPointF()));
//    }

//    return SqlGeneratorBase::phrase(d);
//}

//QString MySqlGenerator::selectCommand(SqlGeneratorBase::AgregateType t,
//                                      QString agregateArg,
//                                      QString tableName,
//                                      QList<WherePhrase> &wheres,
//                                      QList<WherePhrase> &orders,
//                                      QList<RelationModel*> joins,
//                                      int skip, int take)
//{
//    QString command = SqlGeneratorBase::selectCommand(t, agregateArg,
//                                                      tableName,
//                                                      wheres, orders,
//                                                      joins, skip, take);

//    if (take != -1 && skip != -1)
//        command.append(QString(" LIMIT %1 OFFSET %2")
//                       .arg(take)
//                       .arg(skip));
//    return command;
//}

QString MySqlGenerator::createConditionalPhrase(const PhraseData *d) const
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
            return QStringLiteral("DATE_ADD(%1, INTERVAL %2 %3)")
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
            return QStringLiteral("%2(%1)")
                    .arg(createConditionalPhrase(d->left),
                         AbstractSqlGenerator::dateTimePartName(op));

        default:
            break;
        }
    }

    return AbstractSqlGenerator::createConditionalPhrase(d);
}

void MySqlGenerator::appendSkipTake(QString &sql, int skip, int take)
{
    if (take > 0 && skip > 0) {
        sql.append(QStringLiteral(" LIMIT %1 OFFSET %2")
                   .arg(take)
                   .arg(skip));
    } else if (take > 0) {
        sql.append(QStringLiteral(" LIMIT %1").arg(take));
    }
}

QString MySqlGenerator::primaryKeyConstraint(const TableModel *table) const
{
    return  QStringLiteral("PRIMARY KEY (%1)")
        .arg(table->primaryKey());
}

QString MySqlGenerator::escaleFieldName(const QString &fieldName) const
{
    return "`" + fieldName + "`";
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
