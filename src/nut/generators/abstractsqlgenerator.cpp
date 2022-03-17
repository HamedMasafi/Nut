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
#include <QtCore/QPointF>
#include <QtCore/QTime>
#include <QtCore/QUuid>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

#include "abstractsqlgenerator.h"
#include "database.h"
#include "table.h"
#include "databasemodel.h"
#include "tablemodel.h"
#include "sqlserializer.h"
#include "nut_p.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

/*
 * Index:
 *  ALTER TABLE `travelLog` ADD INDEX(`driverId`);
 *
 * Foreign key:
 * ALTER TABLE `travelLog`
 *      ADD CONSTRAINT `travelLog_ibfk_1`
 *      FOREIGN KEY (`driverId`)
 *      REFERENCES `account` (`id`)
 *      ON DELETE CASCADE
 *      ON UPDATE CASCADE;
 *
 * SELECT
 *  FROM            dbo.GiftTypes
 *      INNER JOIN dbo.GiftCards ON dbo.GiftTypes.GiftTypeID = dbo.GiftCards.GiftTypeID
 *      INNER JOIN dbo.Entities ON dbo.GiftCards.GiftCardID = dbo.Entities.GiftCardID
 */
bool AbstractSqlGenerator::isNumeric(const QMetaType::Type &type)
{
    return type == QMetaType::SChar
            || type == QMetaType::Char
            || type == QMetaType::UChar
            || type == QMetaType::Short
            || type == QMetaType::UShort
            || type == QMetaType::Int
            || type == QMetaType::UInt
            || type == QMetaType::Long
            || type == QMetaType::ULong
            || type == QMetaType::LongLong
            || type == QMetaType::ULongLong;
}

AbstractSqlGenerator::AbstractSqlGenerator(Database *parent)
    : QObject(parent)
{
    if (parent)
        _database = parent;

    _serializer = new SqlSerializer;
}

QString AbstractSqlGenerator::masterDatabaseName(QString databaseName)
{
    Q_UNUSED(databaseName)
    return QString();
}

QString AbstractSqlGenerator::createTable(TableModel *table)
{
    Q_UNUSED(table)
    return QString();
}

QString AbstractSqlGenerator::saveRecord(Table *t, QString tableName)
{
    Q_ASSERT(!tableName.isEmpty() && !tableName.isNull());
    switch (t->status()) {
    case Table::Added:
        return insertRecord(t, tableName);

    case Table::Deleted:
        return deleteRecord(t, tableName);

    case Table::Modified:
        return updateRecord(t, tableName);

    case Table::NewCreated:
    case Table::FetchedFromDB:
        Q_UNREACHABLE();
    }

    return QString();
}

QString AbstractSqlGenerator::recordsPhrase(TableModel *table)
{
    if (!table)
        return QString();

    QString ret = QString();
    for (auto &f: table->fields()) {
        if (!ret.isEmpty())
            ret.append(QLatin1String(", "));
        ret.append(QStringLiteral("%1.%2 AS \"%1.%2\"").arg(table->name(), f->name));
    }
    return ret;
}

QString AbstractSqlGenerator::insertBulk(const QString &tableName, const PhraseList &ph, const QList<QVariantList> &vars)
{
    QString sql;
    for (auto &list: vars) {
        QStringList values;
        for (auto &v: list)
            values.append(escapeValue(v));

        if (!sql.isEmpty())
            sql.append(QLatin1String(", "));
        sql.append(QLatin1String("(") + values.join(QLatin1String(", ")) + QLatin1String(")"));
    }
    sql = QStringLiteral("INSERT INTO %1 (%2) VALUES %3")
              .arg(tableName, createFieldPhrase(ph), sql);

    removeTableNames(sql);
    return sql;
}

QString AbstractSqlGenerator::fieldDeclare(FieldModel *field)
{
    QString type = fieldType(field);
    if (type.isEmpty())
        return type;

    QString ret = escaleFieldName(field->name) + QStringLiteral(" ") + type;
    if (field->notNull)
        ret.append(QStringLiteral(" NOT NULL"));

    if (field->isUnique)
        ret.append(QStringLiteral(" UNIQUE"));

    return ret;
}

QString AbstractSqlGenerator::escaleFieldName(const QString &fieldName) const
{
    return fieldName;
}

QStringList AbstractSqlGenerator::constraints(TableModel *table)
{
    Q_UNUSED(table)
    return QStringList();
}

QString AbstractSqlGenerator::relationDeclare(const RelationModel *relation)
{
    return QStringLiteral("FOREIGN KEY (FK_%1) REFERENCES %2(%1)")
            .arg(relation->localColumn, relation->slaveTable->name());
}

QStringList AbstractSqlGenerator::diffDatabase(const DatabaseModel &lastModel,
                                   const DatabaseModel &newModel)
{
    QStringList ret;

    DatabaseModel unionModel = lastModel | newModel;
    DatabaseModel::iterator i;

    for (i = unionModel.begin(); i != unionModel.end(); ++i) {
        TableModel *oldTable = lastModel.tableByName((*i)->name());
        TableModel *newTable = newModel.tableByName((*i)->name());
        QStringList sql = diffTable(oldTable, newTable);
        if (!sql.isEmpty())
            ret << sql;

//        QString sqlRel = diffRelation(oldTable, newTable);
//        if (!sqlRel.isEmpty())
//            ret << sqlRel;
    }

    return ret;
}

QString AbstractSqlGenerator::diffField(FieldModel *oldField, FieldModel *newField)
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
            sql = QStringLiteral("ADD COLUMN ");
        sql.append(fieldDeclare(newField));
    }
    return sql;
}

QStringList AbstractSqlGenerator::diffTable(TableModel *oldTable, TableModel *newTable)
{
    if (!newTable && !oldTable)
        return QStringList();

    if (oldTable && newTable)
        if (*oldTable == *newTable)
            return QStringList();

    if (!newTable)
        return QStringList() << (QStringLiteral("DROP TABLE ") + oldTable->name());

    QList<QString> fieldNames;
    QList<QString> relations;

    if (oldTable) {
        for (auto &f: oldTable->fields())
            if (!fieldNames.contains(f->name))
                fieldNames.append(f->name);
        for (auto &r: oldTable->foreignKeys())
            if (!relations.contains(r->localColumn))
                relations.append(r->localColumn);
    }

    for (auto &f: newTable->fields())
        if (!fieldNames.contains(f->name))
            fieldNames.append(f->name);
    for (auto &r: newTable->foreignKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QStringList columnSql;
    for (auto &fieldName: fieldNames) {
        FieldModel *newField = newTable->field(fieldName);
        if (oldTable) {
            FieldModel *oldField = oldTable->field(fieldName);

            QString buffer = diffField(oldField, newField);
            if (!buffer.isEmpty())
                columnSql << buffer;
        } else {
            QString declare = fieldDeclare(newField);
            if (declare.isEmpty())
                return QStringList() << declare;
            columnSql << declare;
        }
    }
//    for (auto &fieldName: relations) {
//        RelationModel *newRelation = newTable->foreignKeyByField(fieldName);
//        if (oldTable) {
//            RelationModel *oldRelation = oldTable->foreignKeyByField(fieldName);

//            auto buffer = diff(oldRelation, newRelation);
//            if (buffer.size())
//                columnSql.append(buffer);
//        } else {
//            columnSql << relationDeclare(newRelation);
//        }
//    }
    QString sql;
    if (oldTable) {
        sql = QStringLiteral("ALTER TABLE %1 \n%2")
                  .arg(newTable->name(), columnSql.join(QStringLiteral(",\n")));
    } else {
        if (!newTable->primaryKey().isNull()) {
            QString pkCon = primaryKeyConstraint(newTable);
            if (!pkCon.isEmpty())
                columnSql << pkCon;
            columnSql << constraints(newTable);
        }

        sql = QStringLiteral("CREATE TABLE %1 \n(%2)")
                  .arg(newTable->name(), columnSql.join(QStringLiteral(",\n")));

    }
    return QStringList() << sql;
}

QStringList AbstractSqlGenerator::diffRelation(TableModel *oldTable, TableModel *newTable)
{
    QStringList ret;
    if (!newTable)
        return ret;

    QList<QString> relations;

    if (oldTable) {
        for (auto &r: oldTable->foreignKeys())
            if (!relations.contains(r->localColumn))
                relations.append(r->localColumn);
    }

    for (auto &r: newTable->foreignKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QStringList columnSql;
    for (auto &fieldName: relations) {
        RelationModel *newRelation = newTable->foreignKeyByField(fieldName);
        RelationModel *oldRelation = nullptr;
        if (oldTable)
            oldRelation = oldTable->foreignKeyByField(fieldName);

        QStringList buffer = diffRelation2(oldRelation, newRelation);
        if (!buffer.isEmpty())
            columnSql << buffer.at(0);
    }

    if (columnSql.count())
        ret.append(QStringLiteral("ALTER TABLE ") + newTable->name()
                   + QStringLiteral("\n")
                   + columnSql.join(QStringLiteral(",\n")));

    return ret;
}

QStringList AbstractSqlGenerator::diffRelation2(RelationModel *oldRel, RelationModel *newRel)
{
    QStringList ret;
    /*
        CONSTRAINT FK_PersonOrder FOREIGN KEY (PersonID)
            REFERENCES Persons(PersonID)

        ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) REFERENCES %2(%3)

        return QString("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                                 "REFERENCES %2(%3)")
                         .arg(newRelation->localColumn)
                         .arg(newRelation->masterTable->name())
                         .arg(newRelation->foreignColumn);
    */
    if (!oldRel)
        ret.append(QStringLiteral("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                       "REFERENCES %2(%3)")
                .arg(newRel->localColumn, newRel->masterTable->name(),
                     newRel->foreignColumn));

    if (!newRel)
        ret.append(QStringLiteral("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                       "REFERENCES %2(%3)")
                .arg(oldRel->localColumn, oldRel->masterTable->name(),
                     oldRel->foreignColumn));

//    if (*oldRel == *newRel)
    return ret;
}

QString AbstractSqlGenerator::join(const QString &mainTable,
                               const QList<RelationModel*> &list,
                               QStringList *order)
{
    QString ret = mainTable;
    QList<RelationModel*>::const_iterator i;
    for (i = list.begin(); i != list.end(); ++i) {
        if ((*i)->masterTable->name() == mainTable) {
            ret.append(QStringLiteral(" INNER JOIN %3 ON %1.%2 = %3.%4")
                       .arg((*i)->masterTable->name(),
                            (*i)->masterTable->primaryKey(),
                            (*i)->slaveTable->name(),
                            (*i)->localColumn));

            if (order != Q_NULLPTR)
                order->append((*i)->slaveTable->name() + QStringLiteral(".")
                              + (*i)->slaveTable->primaryKey());
        } else {
            ret.append(QStringLiteral(" INNER JOIN %3 ON %1.%2 = %3.%4")
                       .arg(mainTable,
                            (*i)->localColumn,
                            (*i)->masterTable->name(),
                            (*i)->masterTable->primaryKey()));

            if (order != Q_NULLPTR)
                order->append((*i)->masterTable->name() + QStringLiteral(".")
                              + (*i)->masterTable->primaryKey());
        }
    }
    return ret;
}

QString AbstractSqlGenerator::join(const QStringList &list, QStringList *order)
{
    //TODO: reorder list first!
    //TODO: make this ungly code better and bugless :-)
    /*
     * Known issues:
     *  Support only near joins, far supports with medium table finding not support yet
     */

    if (!list.count())
        return QString();

    if (list.count() == 1)
        return QStringLiteral("[")
               + list.first()
               + QStringLiteral("]");

    DatabaseModel model = _database->model();
    QStringList clone = list;
    QString mainTable = clone.takeFirst();
    QString ret = QStringLiteral("[")
                  + mainTable
                  + QStringLiteral("]");

    do {
        if (!clone.count())
            break;

        QString table = clone.first();// model.tableByClassName(clone.first())->name();
        RelationModel *rel = model.relationByClassNames(mainTable, clone.first());
        if (rel) {
            //mainTable is master of table
            ret.append(QStringLiteral(" INNER JOIN [%1] ON %4.%2 = %1.%3")
                       .arg(table, rel->masterTable->primaryKey(),
                            rel->localColumn, mainTable));

            if (order != Q_NULLPTR)
                order->append(mainTable + QStringLiteral(".")
                              + rel->masterTable->primaryKey());

        } else{
            rel = model.relationByClassNames(clone.first(), mainTable);
            if (rel) {
                // table is master of mainTable
                ret.append(QStringLiteral(" INNER JOIN [%1] ON %4.%2 = %1.%3")
                           .arg(table, rel->localColumn,
                           rel->masterTable->primaryKey(), mainTable));

                if (order != Q_NULLPTR)
                    order->append(mainTable + QStringLiteral(".")
                                  + rel->localColumn);

            } else {
//                qInfo("Relation for %s and %s not exists",
//                      qPrintable(table), qPrintable(mainTable));
            }
        }

        clone.takeFirst();
    } while (clone.count());

    return ret;
}

QString AbstractSqlGenerator::insertRecord(Table *t, QString tableName)
{
    QString sql = QString();
    auto model = _database->model().tableByName(tableName);

    QString key = model->isPrimaryKeyAutoIncrement() ? model->primaryKey() : QString();

    QStringList values;

    QSet<QString> props = t->changedProperties();
    QString changedPropertiesText = QString();
    for (auto &f : props) {
        if (f == key)
            continue;

        values.append(escapeValue(t->property(f.toLatin1().data())));

        if (changedPropertiesText != QLatin1String(""))
            changedPropertiesText.append(QStringLiteral(", "));
        changedPropertiesText.append(escaleFieldName(f));
    }
    sql = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3)")
              .arg(tableName, changedPropertiesText, values.join(QStringLiteral(", ")));

    removeTableNames(sql);

    return sql;
}

QString AbstractSqlGenerator::updateRecord(Table *t, QString tableName)
{
    QString sql = QString();
    auto model = _database->model().tableByName(tableName);
    QString key = model->primaryKey();
    QStringList values;

    for (const auto &f : t->changedProperties())
        if (f != key)
            values.append(f + QStringLiteral("=")
                          + escapeValue(t->property(f.toLatin1().data())));

    sql = QStringLiteral("UPDATE %1 SET %2 WHERE %3=%4")
              .arg(tableName,
                   values.join(QStringLiteral(", ")),
                   key,
                   t->property(key.toUtf8().data()).toString());

    removeTableNames(sql);

    return sql;
}

QString AbstractSqlGenerator::deleteRecord(Table *t, QString tableName)
{
    auto model = _database->model().tableByName(tableName);
    QString key = model->primaryKey();
    QString sql = QStringLiteral("DELETE FROM %1 WHERE %2='%3'")
        .arg(tableName, key, t->property(key.toUtf8().data()).toString());
    replaceTableNames(sql);
    return sql;
}

QString AbstractSqlGenerator::agregateText(const AgregateType &t,
                                       const QString &arg) const
{
    switch (t) {
    case Min:
        return QStringLiteral("MIN(") + arg + QStringLiteral(")");

    case Max:
        return QStringLiteral("MAX(") + arg + QStringLiteral(")");

    case Average:
        return QStringLiteral("AVG(") + arg + QStringLiteral(")");

    case Count:
        return QStringLiteral("COUNT(") + arg + QStringLiteral(")");

    case Sum:
        return QStringLiteral("SUM(") + arg + QStringLiteral(")");

    case SingleField:
        return arg;
    }
    return QString(); // never reach
}

QString AbstractSqlGenerator::fromTableText(const QString &tableName,
                                        QString &joinClassName,
                                        QString &orderBy) const
{
    QString tableNameText = tableName;
    if (!joinClassName.isNull()) {
        QString joinTableName = _database->tableName(joinClassName);
        RelationModel *rel
            = _database->model().relationByTableNames(tableName, joinTableName);
        if (rel) {
            QString pk = _database->model().tableByName(tableName)->primaryKey();
            tableNameText = QStringLiteral("%1 INNER JOIN %2 ON (%1.%3 = %2.%4)")
                                .arg(tableName, joinTableName,
                                     pk, rel->localColumn);
            orderBy = tableName + QStringLiteral(".") + pk;
        } else {
            qWarning("Relation between table %s and class %s (%s) not exists!",
                     qPrintable(tableName),
                     qPrintable(joinClassName),
                     qPrintable(joinTableName.isNull() ? QStringLiteral("NULL") : joinTableName));
            joinClassName = QString();
        }
    }

    return tableNameText;
}

QString AbstractSqlGenerator::deleteRecords(const QString &tableName, const QString &where)
{
    QString sql = QStringLiteral("DELETE FROM ") + tableName;

    if (!where.isEmpty())
        sql.append(QStringLiteral(" WHERE ") + where);

    replaceTableNames(sql);

    return sql;
}

QString AbstractSqlGenerator::selectCommand(const QString &tableName,
                                        const PhraseList &fields,
                                        const ConditionalPhrase &where,
                                        const PhraseList &order,
                                        const QList<RelationModel*> &joins,
                                        const int skip,
                                        const int take)
{
    Q_UNUSED(skip)
    Q_UNUSED(take)
    QString selectText;

    if (fields.data.count() == 0) {
        QSet<TableModel*> tables;
        tables.insert(_database->model().tableByName(tableName));
        for (auto &rel: joins)
            tables << rel->masterTable << rel->slaveTable;

        selectText = QString();
        for (auto &t: qAsConst(tables)) {
            if (!selectText.isEmpty())
                selectText.append(QStringLiteral(", "));
            selectText.append(recordsPhrase(t));
        }
    } else {
        selectText = createFieldPhrase(fields);
    }

    QStringList joinedOrders;
    QString orderText = createOrderPhrase(order);
    QString whereText = createConditionalPhrase(where.data);
    QString fromText = join(tableName, joins, &joinedOrders);

    QString sql = QStringLiteral("SELECT ") + selectText
                  + QStringLiteral(" FROM ") + fromText;

    if (!whereText.isEmpty())
        sql.append(QStringLiteral(" WHERE ") + whereText);

    if (!orderText.isEmpty())
        sql.append(QStringLiteral(" ORDER BY ") + orderText);

//    for (int i = 0; i < _database->model().count(); i++)
//        sql = sql.replace(_database->model().at(i)->className() + ".",
//                          _database->model().at(i)->name() + ".");

    appendSkipTake(sql, skip, take);
    replaceTableNames(sql);

    return sql + QStringLiteral(" ");
}

QString AbstractSqlGenerator::selectCommand(const QString &tableName,
                                        const AbstractSqlGenerator::AgregateType &t,
                                        const QString &agregateArg,
                                        const ConditionalPhrase &where,
                                        const QList<RelationModel *> &joins,
                                        const int skip,
                                        const int take)
{
    QStringList joinedOrders;
    QString selectText = agregateText(t, agregateArg);
    QString whereText = createConditionalPhrase(where.data);
    QString fromText = join(tableName, joins, &joinedOrders);

    QString sql = QStringLiteral("SELECT ") + selectText
                  + QStringLiteral(" FROM ") + fromText;

    if (!whereText.isEmpty())
        sql.append(QStringLiteral(" WHERE ") + whereText);

    for (int i = 0; i < _database->model().count(); i++)
        sql = sql.replace(_database->model().at(i)->className() + QStringLiteral("."),
                          _database->model().at(i)->name() + QStringLiteral("."));

    appendSkipTake(sql, skip, take);
    replaceTableNames(sql);

    return sql + QStringLiteral(" ");
}

QString AbstractSqlGenerator::deleteCommand(const QString &tableName,
                                        const ConditionalPhrase &where)
{
    QString command = QStringLiteral("DELETE FROM ") + tableName;
    QString whereText = createConditionalPhrase(where.data);

    if (!whereText.isEmpty())
        command.append(QStringLiteral(" WHERE ") + whereText);

    for (int i = 0; i < _database->model().count(); i++)
        command = command.replace(_database->model().at(i)->className() + QStringLiteral("."),
                                  _database->model().at(i)->name() + QStringLiteral("."));

    replaceTableNames(command);

    return command;
}

QString AbstractSqlGenerator::updateCommand(const QString &tableName,
                                        const AssignmentPhraseList &assigments,
                                        const ConditionalPhrase &where)
{
    QString assigmentTexts = QString();
    for (auto &d: assigments.data) {
        if (assigmentTexts != QLatin1String())
            assigmentTexts.append(QStringLiteral(", "));

        assigmentTexts.append(createConditionalPhrase(d));
    }
    QString whereText = createConditionalPhrase(where.data);

    QString sql = QStringLiteral("UPDATE ") + tableName
                  + QStringLiteral(" SET ") + assigmentTexts;

    if (!whereText.isEmpty())
        sql.append(QStringLiteral(" WHERE ") + whereText);

    for (int i = 0; i < _database->model().count(); i++)
        sql = sql.replace(_database->model().at(i)->className() + QStringLiteral("."),
                          _database->model().at(i)->name() + QStringLiteral("."));

    removeTableNames(sql);

    return sql;
}

QString AbstractSqlGenerator::insertCommand(const QString &tableName, const AssignmentPhraseList &assigments)
{

    QString fieldNames;
    QString values;
    for (auto &d: assigments.data) {
        if (!fieldNames.isEmpty())
            fieldNames.append(QStringLiteral(", "));

        if (!values.isEmpty())
            values.append(QStringLiteral(", "));

        fieldNames.append(escaleFieldName(QString::fromUtf8(d->left->fieldName)));
        values.append(escapeValue(d->operand));
    }
    return QStringLiteral("INSERT INTO %1 (%2) VALUES (%3);")
              .arg(tableName, fieldNames, values);
}

void AbstractSqlGenerator::replaceTableNames(QString &command)
{
    for (auto &m: _database->model())
        command = command
                      .replace(QStringLiteral("[") + m->className()
                                   + QStringLiteral("]"), m->name());
}

void AbstractSqlGenerator::removeTableNames(QString &command)
{
    for (auto &m: _database->model())
        command = command.replace(QStringLiteral("[")
                                      + m->className()
                                      + QStringLiteral("]."), QLatin1String());
}

QString AbstractSqlGenerator::dateTimePartName(const PhraseData::Condition &op) const
{
    switch (op) {
    case PhraseData::DatePartYear:
    case PhraseData::AddYears:
    case PhraseData::AddYearsDateTime:
        return QStringLiteral("YEAR");
    case PhraseData::DatePartMonth:
    case PhraseData::AddMonths:
    case PhraseData::AddMonthsDateTime:
        return QStringLiteral("MONTH");
    case PhraseData::DatePartDay:
    case PhraseData::AddDays:
    case PhraseData::AddDaysDateTime:
        return QStringLiteral("DAY");
    case PhraseData::DatePartHour:
    case PhraseData::AddHours:
    case PhraseData::AddHoursDateTime:
        return QStringLiteral("HOUR");
    case PhraseData::DatePartMinute:
    case PhraseData::AddMinutes:
    case PhraseData::AddMinutesDateTime:
        return QStringLiteral("MINUTE");
    case PhraseData::DatePartSecond:
    case PhraseData::AddSeconds:
    case PhraseData::AddSecondsDateTime:
        return QStringLiteral("SECOND");

    default:
        break;
    }
    return QString();
}

//QString SqlGeneratorBase::deleteCommand(QList<WherePhrase> &wheres,
//                                        QString tableName)
//{
//    QString command = "DELETE FROM " + tableName;
//    QString where = createWhere(wheres);

//    if (where != "")
//        command.append(" WHERE " + where);

//    for (int i = 0; i < _database->model().count(); i++)
//        command = command.replace(_database->model().at(i)->className() + ".",
//                                  _database->model().at(i)->name() + ".");

//    replaceTableNames(command);

//    return command;
//}

//QString SqlGeneratorBase::updateCommand(WherePhrase &phrase,
//                                        QList<WherePhrase> &wheres,
//                                        QString tableName)
//{
//    QString p = this->phrase(phrase.data());
//    QString where = createWhere(wheres);

//    QString sql = "UPDATE " + tableName + " SET " + p;

//    if (where != "")
//        sql.append(" WHERE " + where);

//    for (int i = 0; i < _database->model().count(); i++)
//        sql = sql.replace(_database->model().at(i)->className() + ".",
//                          _database->model().at(i)->name() + ".");

//    removeTableNames(sql);

//    return sql;
//}

QString AbstractSqlGenerator::escapeValue(const QVariant &v) const
{
    if (VARIANT_TYPE_COMPARE(v, String) && v.toString().isEmpty())
        return QStringLiteral("''");

    if (VARIANT_TYPE_COMPARE_X(v, QVariant::List, QMetaType::QVariantList)) {
        auto list = v.toList();
        QStringList ret;
        for (auto &vi: list) {
            ret.append(QStringLiteral("'")
                       + _serializer->serialize(vi)
                       + QStringLiteral("'"));
        }
        return QStringLiteral("(")
               + ret.join(QStringLiteral(", "))
               + QStringLiteral(")");
    }

    QString serialized = _serializer->serialize(v);
    if (serialized.isEmpty()) {
         qWarning("No field escape rule for: %s", v.typeName());
         return QString();
    }


    return QStringLiteral("'") + serialized + QStringLiteral("'");
}

QVariant AbstractSqlGenerator::unescapeValue(const QMetaType::Type &type,
                                     const QVariant &dbValue)
{
    return _serializer->deserialize(dbValue.toString(), type);
}

QString AbstractSqlGenerator::phrase(const PhraseData *d) const
{
    QString ret = QString();

    switch (d->type) {
    case PhraseData::Field:
        ret = d->toString();
        break;

    case PhraseData::WithVariant:
        ret = phrase(d->left)
              + QStringLiteral(" ")
              + operatorString(d->operatorCond)
              + QStringLiteral(" ")
              + escapeValue(d->operand);
        break;

    case PhraseData::WithOther:
        ret = phrase(d->left) + QStringLiteral(" ") + operatorString(d->operatorCond) + QStringLiteral(" ")
              + phrase(d->right);
        break;

    case PhraseData::WithoutOperand:
        ret = phrase(d->left) + QStringLiteral(" ") + operatorString(d->operatorCond);
        break;
    }

    if (d->operatorCond == PhraseData::And || d->operatorCond == PhraseData::Or)
        ret = QStringLiteral("(") + ret + QStringLiteral(")");

    return ret;
}

QString
AbstractSqlGenerator::operatorString(const PhraseData::Condition &cond) const
{
    switch (cond) {
    case PhraseData::Equal:
        return QStringLiteral("=");
    case PhraseData::NotEqual:
        return QStringLiteral("<>");
    case PhraseData::Less:
        return QStringLiteral("<");
    case PhraseData::Greater:
        return QStringLiteral(">");
    case PhraseData::LessEqual:
        return QStringLiteral("<=");
    case PhraseData::GreaterEqual:
        return QStringLiteral(">=");
    case PhraseData::Null:
        return QStringLiteral("IS NULL");

    case PhraseData::NotNull:
        return QStringLiteral("IS NOT NULL");

    case PhraseData::In:
        return QStringLiteral("IN");

    case PhraseData::NotIn:
        return QStringLiteral("NOT IN");

    case PhraseData::And:
        return QStringLiteral("AND");
    case PhraseData::Or:
        return QStringLiteral("OR");

    case PhraseData::Like:
        return QStringLiteral("LIKE");
    case PhraseData::NotLike:
        return QStringLiteral("NOT LIKE");

    case PhraseData::Add:
        return QStringLiteral("+");
    case PhraseData::Minus:
        return QStringLiteral("-");
    case PhraseData::Multiple:
        return QStringLiteral("*");
    case PhraseData::Divide:
        return QStringLiteral("/");

//    case PhraseData::Set:
//        return QStringLiteral("=");

//    case PhraseData::Append:
//        return QStringLiteral(",");

    case PhraseData::Between:
        return QStringLiteral("BETWEEN");

    case PhraseData::Mod:
        return QStringLiteral("MOD");

    default:
        qDebug() << "Unsupported" << cond;
        return QStringLiteral("<FAIL cond> %1").arg(cond);
    }
}

void AbstractSqlGenerator::appendSkipTake(QString &sql, int skip, int take)
{
    Q_UNUSED(sql)
    Q_UNUSED(skip)
    Q_UNUSED(take)
}

QString AbstractSqlGenerator::primaryKeyConstraint(const TableModel *table) const
{
    return  QStringLiteral("CONSTRAINT pk_%1 PRIMARY KEY (%2)")
            .arg(table->name(), table->primaryKey());
}

QString AbstractSqlGenerator::createConditionalPhrase(const PhraseData *d) const
{
    if (!d)
        return QString();

    QString ret = QString();

    PhraseData::Condition op = d->operatorCond;
    //apply not (!)
    if (d->isNot) {
        if (op < 20)
            op = static_cast<PhraseData::Condition>((op + 10) % 20);
    }
    switch (d->type) {
    case PhraseData::Field:
        ret = d->toString();
        break;

    case PhraseData::WithVariant:
       /* if (op == PhraseData::AddYears)
            ret = QString("DATEADD(year, %1, %2)")
                    .arg(d->operand.toString(), createConditionalPhrase(d->left));
        else if (op == PhraseData::AddMonths)
            ret = QString("DATEADD(month, %1, %2)")
                    .arg(d->operand.toString(), createConditionalPhrase(d->left));
        else if (op == PhraseData::AddDays)
            ret = QString("DATEADD(day, %1, %2)")
                    .arg(d->operand.toString(), createConditionalPhrase(d->left));
        else if (op == PhraseData::AddHours)
            ret = QString("DATEADD(hour, %1, %2)")
                    .arg(d->operand.toString(), createConditionalPhrase(d->left));
        else if (op == PhraseData::AddMinutes)
            ret = QString("DATEADD(minute, %1, %2)")
                    .arg(d->operand.toString(), createConditionalPhrase(d->left));
        else if (op == PhraseData::AddSeconds)
            ret = QString("DATEADD(second, %1, %2)")
                    .arg(d->operand.toString(), createConditionalPhrase(d->left));
        else */if (op == PhraseData::Between) {
            QVariantList list = d->operand.toList();
            ret = QStringLiteral("%1 BETWEEN %2 AND %3")
                    .arg(createConditionalPhrase(d->left), escapeValue(list.at(0)), escapeValue(list.at(1)));

        } else if (op == PhraseData::DatePartYear)
            ret = QStringLiteral("DATEPART(year, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartMonth)
            ret = QStringLiteral("DATEPART(month, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartDay)
            ret = QStringLiteral("DATEPART(day, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartHour)
            ret = QStringLiteral("DATEPART(hour, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartMinute)
            ret = QStringLiteral("DATEPART(minute, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartMilisecond)
            ret = QStringLiteral("DATEPART(milisecond, %1)")
                    .arg(d->operand.toString());
        else
            ret = createConditionalPhrase(d->left) + QStringLiteral(" ")
                  + operatorString(op) + QStringLiteral(" ")
                  + escapeValue(d->operand);
        break;

    case PhraseData::WithOther:
        ret = createConditionalPhrase(d->left) + QStringLiteral(" ")
              + operatorString(op) + QStringLiteral(" ")
              + createConditionalPhrase(d->right);
        break;

    case PhraseData::WithoutOperand:
        ret = createConditionalPhrase(d->left) + QStringLiteral(" ")
              + operatorString(op);
        break;
    }

    if (d->operatorCond == PhraseData::And || d->operatorCond == PhraseData::Or)
        ret = QStringLiteral("(") + ret + QStringLiteral(")");

    return ret;
}

QString AbstractSqlGenerator::createOrderPhrase(const PhraseList &ph)
{
    QString ret = QString();
    for (const auto &d: ph.data) {
        if (!ret.isEmpty())
            ret.append(QStringLiteral(", "));
        ret.append(d->toString());
        if (d->isNot)
            ret.append(QStringLiteral(" DESC"));
    }

    return ret;
}

QString AbstractSqlGenerator::createFieldPhrase(const PhraseList &ph)
{
    QString ret = QString();
    for (const auto &d: ph.data) {
        if (!ret.isEmpty())
            ret.append(QStringLiteral(", "));
        ret.append("`" + d->toString() + "`");
        if (d->isNot)
            qDebug() << "Operator ! is ignored in fields phrase";
    }
    return ret;
}

void AbstractSqlGenerator::createInsertPhrase(const AssignmentPhraseList &ph, QString &fields, QString &values)
{
    for (auto &d: ph.data) {
        if (!fields.isEmpty())
            fields.append(QStringLiteral(", "));

        if (!values.isEmpty())
            values.append(QStringLiteral(", "));

        switch (d->type) {
        case PhraseData::WithVariant:
            fields.append(d->left->toString());
            values.append(escapeValue(d->operand));
//            ret = createConditionalPhrase(d->left->toString()) + " " + operatorString(d->operatorCond) + " "
//                  + escapeValue(d->operand);
            break;

        case PhraseData::WithOther:
            fields.append(d->left->toString());
            values.append(d->right->toString());
            break;

        case PhraseData::Field:
        case PhraseData::WithoutOperand:
            qFatal("Invalid insert command");
        }
    }
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
