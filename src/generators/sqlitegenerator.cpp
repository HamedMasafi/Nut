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

#include "sqlitegenerator.h"
#include "../table.h"
#include "../tablemodel.h"

NUT_BEGIN_NAMESPACE

SqliteGenerator::SqliteGenerator(Database *parent) : SqlGeneratorBase(parent)
{

}

QString SqliteGenerator::fieldType(FieldModel *field)
{
    switch (field->type) {
    case QMetaType::Bool:           return "BOOLEAN";
    case QMetaType::QBitArray:
    case QMetaType::QByteArray:     return "BLOB";
    case QMetaType::QDate:          return "DATE";
    case QMetaType::QDateTime:      return "DATETIME";
    case QMetaType::QTime:          return "TIME";
    case QMetaType::Double:         return "DOUBLE";
    case QMetaType::Float:          return "FLOAT";

    case QMetaType::SChar:
    case QMetaType::Char:           return "TINYINT";
    case QMetaType::UChar:          return "TINYINT UNSIGNED";
    case QMetaType::Short:          return "SMALLINT";
    case QMetaType::UShort:         return "SMALLINT UNSIGNED";
    case QMetaType::Int:            return "INT";
    case QMetaType::UInt:           return "INT UNSIGNED";
    case QMetaType::Long:           return "MEDIUMINT";
    case QMetaType::ULong:          return "MEDIUMINT UNSIGNED";
    case QMetaType::LongLong:       return "BIGINT";
    case QMetaType::ULongLong:      return "BIGINT UNSIGNED";

    case QMetaType::QChar:          return "NCHAR(1)";

    case QMetaType::QUrl:
    case QMetaType::QJsonArray:
    case QMetaType::QJsonValue:
    case QMetaType::QJsonObject:
    case QMetaType::QJsonDocument:
    case QMetaType::QPoint:
    case QMetaType::QPointF:
    case QMetaType::QSize:
    case QMetaType::QSizeF:
    case QMetaType::QLine:
    case QMetaType::QLineF:
    case QMetaType::QRect:
    case QMetaType::QRectF:
    case QMetaType::QPolygon:
    case QMetaType::QPolygonF:
    case QMetaType::QStringList:
    case QMetaType::QColor:
    case QMetaType::QUuid:          return "TEXT";

//        if (field->isAutoIncrement)
//            dbType.append(" PRIMARY KEY AUTOINCREMENT");

    case QMetaType::QString:
        if(field->length)
            return QString("VARCHAR(%1)").arg(field->length);
        else
            return "TEXT";
    default:
//        qWarning("The type (%s) does not supported",
//                 QMetaType::typeName(field->type));
        return QString();
    }
}

QString SqliteGenerator::fieldDeclare(FieldModel *field)
{
    QString type = fieldType(field);
    if (type.isEmpty())
        return type;

    if (isNumeric(field->type) && field->isPrimaryKey) {
        type = "INTEGER PRIMARY KEY";
        if (field->isAutoIncrement)
            type.append(" AUTOINCREMENT");
    }

    if (field->notNull)
        type.append(" NOT NULL");

    if (field->isUnique)
        type.append(" UNIQUE");

    return field->name + " " + type;
}

bool SqliteGenerator::supportAutoIncrement(const QMetaType::Type &type)
{
    return isNumeric(type);
}


QStringList SqliteGenerator::diff(TableModel *oldTable, TableModel *newTable)
{
    QStringList ret;

    if (oldTable && newTable)
        if (*oldTable == *newTable)
            return ret;

    QStringList newTableSql = SqlGeneratorBase::diff(nullptr, newTable);

    if (!newTable)
        return QStringList() << "DROP TABLE " + oldTable->name();

    if (!oldTable)
        return newTableSql;

    QList<QString> fieldNames;
    QList<QString> relations;

    foreach (FieldModel *f, oldTable->fields())
        if (!fieldNames.contains(f->name))
            fieldNames.append(f->name);
    foreach (RelationModel *r, oldTable->foreignKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    foreach (FieldModel *f, newTable->fields())
        if (!fieldNames.contains(f->name))
            fieldNames.append(f->name);
    foreach (RelationModel *r, newTable->foreignKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QString columns;
    foreach (FieldModel *f, oldTable->fields()) {
        if (!newTable->field(f->name))
            continue;

        if (!columns.isEmpty())
            columns.append(", ");
        columns.append(f->name);
    }

    /*
    ALTER TABLE sampleTable RENAME TO sqlitestudio_temp_table;

    CREATE TABLE sampleTable (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        t  BIGINT,
        m  CHAR
    );

    INSERT INTO sampleTable (
                                id,
                                t,
                                m
                            )
                            SELECT id,
                                   t,
                                   m
                              FROM sqlitestudio_temp_table;

    DROP TABLE sqlitestudio_temp_table;
    */

    ret.append("ALTER TABLE " + newTable->name() + " RENAME TO sqlitestudio_temp_table;");
    ret.append(newTableSql);
    ret.append(QString("INSERT INTO %1 ( %2 ) SELECT %2 FROM sqlitestudio_temp_table;")
               .arg(newTable->name(), columns));
    ret.append("DROP TABLE sqlitestudio_temp_table;");
    return ret;
}
void SqliteGenerator::appendSkipTake(QString &sql, int skip, int take)
{
    if (take > 0 && skip > 0) {
        sql.append(QString(" LIMIT %1 OFFSET %2")
                   .arg(take)
                   .arg(skip));
    } else if (take > 0) {
        sql.append(QString(" LIMIT %1").arg(take));
    }
}

QString SqliteGenerator::primaryKeyConstraint(const TableModel *table) const
{
    Q_UNUSED(table)
    return QString();
//    QString sql = QString("CONSTRAINT pk_%1 PRIMARY KEY (%2)")
//            .arg(table->name())
//            .arg(table->primaryKey());
//    if (table->field(table->primaryKey())->isAutoIncrement)
//        sql += " AUTOINCREMENT";
//    return sql;
}

QString SqliteGenerator::createConditionalPhrase(const PhraseData *d) const
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
        case PhraseData::AddMonths:
        case PhraseData::AddDays: {
            int i = d->operand.toInt();
            return QString("DATE(%1,'%2 %3')")
                    .arg(createConditionalPhrase(d->left),
                         (i < 0 ? "" : "+") + QString::number(i),
                         dateTimePartName(op));
            break;
        }
        case PhraseData::AddHours:
        case PhraseData::AddMinutes:
        case PhraseData::AddSeconds: {
            int i = d->operand.toInt();
            return QString("TIME(%1,'%2 %3')")
                    .arg(createConditionalPhrase(d->left),
                         (i < 0 ? "" : "+") + QString::number(i),
                         dateTimePartName(op));
            break;
        }
        case PhraseData::AddYearsDateTime:
        case PhraseData::AddMonthsDateTime:
        case PhraseData::AddDaysDateTime:
        case PhraseData::AddHoursDateTime:
        case PhraseData::AddMinutesDateTime:
        case PhraseData::AddSecondsDateTime: {
            int i = d->operand.toInt();
            return QString("DATETIME(%1,'%2 %3')")
                    .arg(createConditionalPhrase(d->left),
                         (i < 0 ? "" : "+") + QString::number(i),
                         dateTimePartName(op));
            break;
        }
        default:
            break;
        }
    }
    if (d->type == PhraseData::WithoutOperand) {
        switch (op) {
        case PhraseData::DatePartYear:
            return QString("CAST(strftime('%Y', %1) AS INT)")
                    .arg(createConditionalPhrase(d->left));

        case PhraseData::DatePartMonth:
            return QString("CAST(strftime('%m', %1) AS INT)")
                    .arg(createConditionalPhrase(d->left));

        case PhraseData::DatePartDay:
            return QString("CAST(strftime('%d', %1) AS INT)")
                    .arg(createConditionalPhrase(d->left));

        case PhraseData::DatePartHour:
            return QString("CAST(strftime('%H', %1) AS INT)")
                    .arg(createConditionalPhrase(d->left));

        case PhraseData::DatePartMinute:
            return QString("CAST(strftime('%M', %1) AS INT)")
                    .arg(createConditionalPhrase(d->left));

        case PhraseData::DatePartSecond:
            return QString("CAST(strftime('%S', %1) AS INT)")
                    .arg(createConditionalPhrase(d->left));

            //        case PhraseData::DatePartMilisecond:
            //            return QString("CAST(strftime('%Y', %1) AS INT)")
            //                    .arg(createConditionalPhrase(d->left));
        default:
            break;
        }
    }

    return SqlGeneratorBase::createConditionalPhrase(d);
}

QString SqliteGenerator::escapeValue(const QVariant &v) const
{
    if (v.type() == QVariant::Time)
        return "'" + v.toTime().toString("HH:mm:ss") + "'";

    if (v.type() == QVariant::Date)
        return "'" + v.toDate().toString("yyyy-MM-dd") + "'";

    if (v.type() == QVariant::DateTime)
        return "'" + v.toDateTime().toString("yyyy-MM-dd HH:mm:ss") + "'";

    return SqlGeneratorBase::escapeValue(v);
}

QVariant SqliteGenerator::unescapeValue(const QMetaType::Type &type, const QVariant &dbValue)
{
    if (type == QMetaType::QDateTime)
        return dbValue.toDateTime();

    if (type == QMetaType::QTime)
        return dbValue.toTime();

    if (type == QMetaType::QDate)
        return dbValue.toDate();

    return SqlGeneratorBase::unescapeValue(type, dbValue);
}

NUT_END_NAMESPACE
