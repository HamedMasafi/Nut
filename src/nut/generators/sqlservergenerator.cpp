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

#include "sqlservergenerator.h"
#include "table.h"
#include "tablemodel.h"
#include "nut_p.h"

#include <QtCore/QPoint>
#include <QtCore/QRegularExpression>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

SqlServerGenerator::SqlServerGenerator(Database *parent)
    : AbstractSqlGenerator(parent)
{
}

QString SqlServerGenerator::masterDatabaseName(QString databaseName)
{
    return databaseName.replace(
        QRegularExpression(QStringLiteral("DATABASE\\=(\\w+)"),
                           QRegularExpression::CaseInsensitiveOption),
        QStringLiteral("DATABASE="));
}

QString SqlServerGenerator::fieldType(FieldModel *field)
{
    switch (field->type) {
    case QMetaType::Bool:
        return QStringLiteral("BIT");

    case QMetaType::Char:
    case QMetaType::QChar:
        return QStringLiteral("CHAR(1)");

    case QMetaType::SChar:
    case QMetaType::UChar:
        return QStringLiteral("tinyint");

    case QMetaType::Short:
    case QMetaType::UShort:
        return QStringLiteral("smallint");

    case QMetaType::UInt:
    case QMetaType::Int:
        if (field->isAutoIncrement)
            return QStringLiteral("INT IDENTITY(1,1)");
        else
            return QStringLiteral("INT");

    case QMetaType::Long:
    case QMetaType::ULong:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return QStringLiteral("bigint");

    case QMetaType::Float:
        return QStringLiteral("FLOAT(24)");

    case QMetaType::Double:
        return QStringLiteral("REAL");

    case QMetaType::QBitArray:
    case QMetaType::QByteArray:
        if (field->length)
            return QStringLiteral("VARBINARY (") + QString::number(field->length) + QStringLiteral(")");
        else
            return QStringLiteral("VARBINARY (MAX)");
        break;

    case QMetaType::QDate:
        return QStringLiteral("DATE");

    case QMetaType::QDateTime:
        return QStringLiteral("DATETIME");

    case QMetaType::QTime:
        return QStringLiteral("TIME");

    case QMetaType::QPoint:
    case QMetaType::QPointF:
        return QStringLiteral("TEXT");

    case QMetaType::QString:
        if (field->length)
            return QStringLiteral("NVARCHAR(%1)").arg(field->length);
        else
            return QStringLiteral("NVARCHAR(MAX)");

    case QMetaType::QUuid:
        return QStringLiteral("UNIQUEIDENTIFIER");

    case QMetaType::QPolygon:
    case QMetaType::QPolygonF:
    case QMetaType::QSize:
    case QMetaType::QSizeF:
    case QMetaType::QRect:
    case QMetaType::QRectF:
    case QMetaType::QLine:
    case QMetaType::QLineF:
    case QMetaType::QColor:
    case QMetaType::QStringList:
    case QMetaType::QJsonArray:
    case QMetaType::QJsonValue:
    case QMetaType::QJsonObject:
    case QMetaType::QJsonDocument:
    case QMetaType::QUrl:
        return QStringLiteral("TEXT");

    default:
//        Q_UNREACHABLE();
        return QString();
    }
}

QString SqlServerGenerator::diffField(FieldModel *oldField, FieldModel *newField)
{
    if (!oldField && !newField)
        return QString();

    if (oldField && newField)
        if (*oldField == *newField)
            return QString();

    QString sql = QString();
    if (!newField) {
        sql = QStringLiteral("DROP COLUMN ") + oldField->name;
    } else {
        if (oldField)
            sql = QStringLiteral("MODIFY COLUMN ");
        else
            sql = QStringLiteral("ADD ");

        sql.append(fieldDeclare(newField));
    }
    return sql;
}

QString SqlServerGenerator::escapeValue(const QVariant &v) const
{
    switch (METATYPE_ID(v)) {
    case QMetaType::QString:
    case QMetaType::QChar:
    case QMetaType::QPolygon:
    case QMetaType::QPolygonF:
    case QMetaType::QSize:
    case QMetaType::QSizeF:
    case QMetaType::QRect:
    case QMetaType::QRectF:
    case QMetaType::QLine:
    case QMetaType::QLineF:
    case QMetaType::QColor:
    case QMetaType::QStringList:
//    case QMetaType::QJsonArray:
//    case QMetaType::QJsonValue:
//    case QMetaType::QJsonObject:
//    case QMetaType::QJsonDocument:
    case QMetaType::QUrl:
        return QStringLiteral("N") + AbstractSqlGenerator::escapeValue(v);

//    case QMetaType::QPoint: {
//        QPoint pt = v.toPoint();
//        return QString("geography::POINT(%1, %2, 4326)").arg(pt.x()).arg(
//                    pt.y());
//    }
//    case QMetaType::QPointF: {
//        QPointF pt = v.toPointF();
//        return QString("geography::POINT(%1, %2, 4326)").arg(pt.x()).arg(
//                    pt.y());
//    }

    case QMetaType::QTime:
        return v.toTime().toString(QStringLiteral("''HH:mm:ss''"));

    case QMetaType::QDate:
        return v.toDate().toString(QStringLiteral("''yyyy-MM-dd''"));

    case QMetaType::QDateTime:
        return v.toDateTime().toString(QStringLiteral("''yyyy-MM-dd HH:mm:ss''"));

    default:
        return AbstractSqlGenerator::escapeValue(v);
    }
}

QVariant SqlServerGenerator::unescapeValue(const QMetaType::Type &type, const QVariant &dbValue)
{
    if (type == QMetaType::QDateTime)
        return dbValue.toDateTime();

    if (type == QMetaType::QTime)
        return dbValue.toTime();

    if (type == QMetaType::QDate)
        return dbValue.toDate();

    return AbstractSqlGenerator::unescapeValue(type, dbValue);
}

void SqlServerGenerator::appendSkipTake(QString &sql, int skip, int take)
{
    if (skip != -1)
        sql.append(QStringLiteral(" OFFSET %1 ROWS")
                   .arg(skip));
    if (take > 0)
        sql.append(QStringLiteral(" FETCH %2 %1 ROWS ONLY")
                       .arg(take)
                       .arg(skip > 1 ? QStringLiteral("NEXT") : QStringLiteral("FIRST")));
}

QString SqlServerGenerator::createConditionalPhrase(const PhraseData *d) const
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
            return QStringLiteral("DATEADD(%3, %2, %1)")
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
            return QStringLiteral("DATEPART(%2, %1)")
                    .arg(createConditionalPhrase(d->left),
                         AbstractSqlGenerator::dateTimePartName(op));

        default:
            break;
        }
    }

    return AbstractSqlGenerator::createConditionalPhrase(d);
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
