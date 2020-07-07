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

#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QPointF>
#include <QTime>
#include <QUuid>
#include <QVariant>

#include "sqlgeneratorbase_p.h"
#include "../database.h"
#include "../table.h"
#include "../databasemodel.h"
#include "../tablemodel.h"
#include "sqlserializer.h"

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
bool SqlGeneratorBase::isNumeric(const QMetaType::Type &type)
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

SqlGeneratorBase::SqlGeneratorBase(Database *parent)
    : QObject(parent)
{
    if (parent)
        _database = parent;

    _serializer = new SqlSerializer;
}

QString SqlGeneratorBase::masterDatabaseName(QString databaseName)
{
    Q_UNUSED(databaseName)
    return QString();
}

QString SqlGeneratorBase::createTable(TableModel *table)
{
    Q_UNUSED(table)
    return QString();
}

QString SqlGeneratorBase::saveRecord(Table *t, QString tableName)
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
    case Table::FeatchedFromDB:
        Q_UNREACHABLE();
    }

    return QString();
}

QString SqlGeneratorBase::recordsPhrase(TableModel *table)
{
    if (!table)
        return QString();

    QString ret = QString();
    foreach (FieldModel *f, table->fields()) {
        if (!ret.isEmpty())
            ret.append(", ");
        ret.append(QString("%1.%2 AS \"%1.%2\"").arg(table->name(), f->name));
    }
    return ret;
}

QString SqlGeneratorBase::insertBulk(const QString &tableName, const PhraseList &ph, const QList<QVariantList> &vars)
{
    QString sql;
    foreach (QVariantList list, vars) {
        QStringList values;
        foreach (QVariant v, list)
            values.append(escapeValue(v));

        if (!sql.isEmpty())
            sql.append(", ");
        sql.append("(" + values.join(", ") + ")");
    }
    sql = "INSERT INTO " + tableName + "(" + createFieldPhrase(ph)
            + ") VALUES" + sql;

    removeTableNames(sql);
    return sql;
}

QString SqlGeneratorBase::fieldDeclare(FieldModel *field)
{
    QString type = fieldType(field);
    if (type.isEmpty())
        return type;
    return field->name + " " + type
            + (field->notNull ? " NOT NULL" : "")
            + (field->isUnique ? " UNIQUE" : "");
}

QStringList SqlGeneratorBase::constraints(TableModel *table)
{
    Q_UNUSED(table)
    return QStringList();
}

QString SqlGeneratorBase::relationDeclare(const RelationModel *relation)
{
    return QString("FOREIGN KEY (FK_%1) REFERENCES %2(%1)")
            .arg(relation->localColumn, relation->slaveTable->name());
}

QStringList SqlGeneratorBase::diff(const DatabaseModel &lastModel,
                                   const DatabaseModel &newModel)
{
    QStringList ret;

    DatabaseModel unionModel = lastModel | newModel;
    DatabaseModel::iterator i;

    for (i = unionModel.begin(); i != unionModel.end(); ++i) {
        TableModel *oldTable = lastModel.tableByName((*i)->name());
        TableModel *newTable = newModel.tableByName((*i)->name());
        QStringList sql = diff(oldTable, newTable);
        if (!sql.isEmpty())
            ret << sql;

//        QString sqlRel = diffRelation(oldTable, newTable);
//        if (!sqlRel.isEmpty())
//            ret << sqlRel;
    }

    return ret;
}

QString SqlGeneratorBase::diff(FieldModel *oldField, FieldModel *newField)
{
    QString sql = QString();
    if (oldField && newField)
        if (*oldField == *newField)
            return sql;

    if (!newField) {
        sql = "DROP COLUMN " + oldField->name;
    } else {
        if (oldField)
            sql = "MODIFY COLUMN ";
        else
            sql = "ADD COLUMN ";
        sql.append(fieldDeclare(newField));
    }
    return sql;
}

QStringList SqlGeneratorBase::diff(TableModel *oldTable, TableModel *newTable)
{
    if (!newTable && !oldTable)
        return QStringList();

    if (oldTable && newTable)
        if (*oldTable == *newTable)
            return QStringList();

    if (!newTable)
        return QStringList() << ("DROP TABLE " + oldTable->name());

    QList<QString> fieldNames;
    QList<QString> relations;

    if (oldTable) {
        foreach (FieldModel *f, oldTable->fields())
            if (!fieldNames.contains(f->name))
                fieldNames.append(f->name);
        foreach (RelationModel *r, oldTable->foreignKeys())
            if (!relations.contains(r->localColumn))
                relations.append(r->localColumn);
    }

    foreach (FieldModel *f, newTable->fields())
        if (!fieldNames.contains(f->name))
            fieldNames.append(f->name);
    foreach (RelationModel *r, newTable->foreignKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QStringList columnSql;
    foreach (QString fieldName, fieldNames) {
        FieldModel *newField = newTable->field(fieldName);
        if (oldTable) {
            FieldModel *oldField = oldTable->field(fieldName);

            QString buffer = diff(oldField, newField);
            if (!buffer.isEmpty())
                columnSql << buffer;
        } else {
            QString declare = fieldDeclare(newField);
            if (declare.isEmpty())
                return QStringList() << declare;
            columnSql << declare;
        }
    }
//    foreach (QString fieldName, relations) {
//        RelationModel *newRelation = newTable->foregionKeyByField(fieldName);
//        if (oldTable) {
//            RelationModel *oldRelation = oldTable->foregionKeyByField(fieldName);

//            QString buffer = diff(oldRelation, newRelation);
//            if (!buffer.isNull())
//                columnSql << buffer;
//        } else {
//            columnSql << relationDeclare(newRelation);
//        }
//    }
    QString sql;
    if (oldTable) {
        sql = QString("ALTER TABLE %1 \n%2")
                .arg(newTable->name(), columnSql.join(",\n"));
    } else {
        if (!newTable->primaryKey().isNull()) {
            QString pkCon = primaryKeyConstraint(newTable);
            if (!pkCon.isEmpty())
                columnSql << pkCon;
            columnSql << constraints(newTable);
        }

        sql = QString("CREATE TABLE %1 \n(%2)")
                .arg(newTable->name(), columnSql.join(",\n"));

    }
    return QStringList() << sql;
}

QStringList SqlGeneratorBase::diffRelation(TableModel *oldTable, TableModel *newTable)
{
    QStringList ret;
    if (!newTable)
        return ret;

    QList<QString> relations;

    if (oldTable)
        foreach (RelationModel *r, oldTable->foreignKeys())
            if (!relations.contains(r->localColumn))
                relations.append(r->localColumn);

    foreach (RelationModel *r, newTable->foreignKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QStringList columnSql;
    foreach (QString fieldName, relations) {
        RelationModel *newRelation = newTable->foreignKeyByField(fieldName);
        RelationModel *oldRelation = nullptr;
        if (oldTable)
            oldRelation = oldTable->foreignKeyByField(fieldName);

        QStringList buffer = diff(oldRelation, newRelation);
        if (!buffer.isEmpty())
            columnSql << buffer.at(0);
    }

    if (columnSql.count())
        ret.append("ALTER TABLE " + newTable->name() + "\n"
                + columnSql.join(",\n"));

    return ret;
}

QStringList SqlGeneratorBase::diff(RelationModel *oldRel, RelationModel *newRel)
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
        ret.append(QString("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                       "REFERENCES %2(%3)")
                .arg(newRel->localColumn, newRel->masterTable->name(),
                     newRel->foreignColumn));

    if (!newRel)
        ret.append(QString("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                       "REFERENCES %2(%3)")
                .arg(oldRel->localColumn, oldRel->masterTable->name(),
                     oldRel->foreignColumn));

//    if (*oldRel == *newRel)
    return ret;
}

QString SqlGeneratorBase::join(const QString &mainTable,
                               const QList<RelationModel*> &list,
                               QStringList *order)
{
    QString ret = mainTable;
    QList<RelationModel*>::const_iterator i;
    for (i = list.begin(); i != list.end(); ++i) {
        if ((*i)->masterTable->name() == mainTable) {
            ret.append(QString(" INNER JOIN %3 ON %1.%2 = %3.%4")
                       .arg((*i)->masterTable->name(),
                            (*i)->masterTable->primaryKey(),
                            (*i)->slaveTable->name(),
                            (*i)->localColumn));

            if (order != Q_NULLPTR)
                order->append((*i)->slaveTable->name() + "." + (*i)->slaveTable->primaryKey());
        } else {
            ret.append(QString(" INNER JOIN %3 ON %1.%2 = %3.%4")
                       .arg(mainTable,
                            (*i)->localColumn,
                            (*i)->masterTable->name(),
                            (*i)->masterTable->primaryKey()));

            if (order != Q_NULLPTR)
                order->append((*i)->masterTable->name() + "." + (*i)->masterTable->primaryKey());
        }
    }
    return ret;
}

QString SqlGeneratorBase::join(const QStringList &list, QStringList *order)
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
        return "[" + list.first() + "]";

    DatabaseModel model = _database->model();
    QStringList clone = list;
    QString mainTable = clone.takeFirst();
    QString ret = "[" + mainTable + "]";

    do {
        if (!clone.count())
            break;

        QString table = clone.first();// model.tableByClassName(clone.first())->name();
        RelationModel *rel = model.relationByClassNames(mainTable, clone.first());
        if (rel) {
            //mainTable is master of table
            ret.append(QString(" INNER JOIN [%1] ON %4.%2 = %1.%3")
                       .arg(table, rel->masterTable->primaryKey(),
                            rel->localColumn, mainTable));

            if (order != Q_NULLPTR)
                order->append(mainTable + "." + rel->masterTable->primaryKey());

        } else{
            rel = model.relationByClassNames(clone.first(), mainTable);
            if (rel) {
                // table is master of mainTable
                ret.append(QString(" INNER JOIN [%1] ON %4.%2 = %1.%3")
                           .arg(table, rel->localColumn,
                           rel->masterTable->primaryKey(), mainTable));

                if (order != Q_NULLPTR)
                    order->append(mainTable + "." + rel->localColumn);

            } else {
//                qInfo("Relation for %s and %s not exists",
//                      qPrintable(table), qPrintable(mainTable));
            }
        }

        clone.takeFirst();
    } while (clone.count());

    return ret;
}

QString SqlGeneratorBase::insertRecord(Table *t, QString tableName)
{
    QString sql = QString();
    auto model = _database->model().tableByName(tableName);

    QString key = model->isPrimaryKeyAutoIncrement() ? model->primaryKey() : QString();

    QStringList values;

    QSet<QString> props = t->changedProperties();
    QString changedPropertiesText = QString();
    foreach (QString f, props) {
        if (f == key)
            continue;

        values.append(escapeValue(t->property(f.toLatin1().data())));

        if (changedPropertiesText != "")
            changedPropertiesText.append(", ");
        changedPropertiesText.append(f);
    }
    sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
              .arg(tableName, changedPropertiesText, values.join(", "));

    removeTableNames(sql);

    return sql;
}

QString SqlGeneratorBase::updateRecord(Table *t, QString tableName)
{
    QString sql = QString();
    auto model = _database->model().tableByName(tableName);
    QString key = model->primaryKey();
    QStringList values;

    foreach (QString f, t->changedProperties())
        if (f != key)
            values.append(f + "='" + t->property(f.toLatin1().data()).toString()
                          + "'");
    sql = QString("UPDATE %1 SET %2 WHERE %3=%4")
              .arg(tableName, values.join(", "),
                   key, t->property(key.toUtf8().data()).toString());

    removeTableNames(sql);

    return sql;
}

QString SqlGeneratorBase::deleteRecord(Table *t, QString tableName)
{
    auto model = _database->model().tableByName(tableName);
    QString key = model->primaryKey();
    QString sql = QString("DELETE FROM %1 WHERE %2='%3'")
        .arg(tableName, key, t->property(key.toUtf8().data()).toString());
    replaceTableNames(sql);
    return sql;
}

QString SqlGeneratorBase::agregateText(const AgregateType &t,
                                       const QString &arg) const
{
    switch (t) {
    case Min:
        return "MIN(" + arg + ")";

    case Max:
        return "MAX(" + arg + ")";

    case Average:
        return "AVG(" + arg + ")";

    case Count:
        return "COUNT(" + arg + ")";

    case Sum:
        return "SUM(" + arg + ")";

    case SingleField:
        return arg;
    }
    return QString(); // never reach
}

QString SqlGeneratorBase::fromTableText(const QString &tableName,
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
            tableNameText = QString("%1 INNER JOIN %2 ON (%1.%3 = %2.%4)")
                                .arg(tableName, joinTableName,
                                     pk, rel->localColumn);
            orderBy = tableName + "." + pk;
        } else {
            qWarning("Relation between table %s and class %s (%s) not exists!",
                     qPrintable(tableName),
                     qPrintable(joinClassName),
                     qPrintable(joinTableName.isNull() ? "NULL" : joinTableName));
            joinClassName = QString();
        }
    }

    return tableNameText;
}

QString SqlGeneratorBase::deleteRecords(const QString &tableName, const QString &where)
{
    QString sql = QString();
    if (where.isEmpty() || where.isNull())
        sql = "DELETE FROM " + tableName;
    else
        sql = "DELETE FROM " + tableName + " WHERE " + where;

    replaceTableNames(sql);

    return sql;
}

QString SqlGeneratorBase::selectCommand(const QString &tableName,
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
        foreach (RelationModel *rel, joins)
            tables << rel->masterTable << rel->slaveTable;

        selectText = QString();
        foreach (TableModel *t, tables) {
            if (!selectText.isEmpty())
                selectText.append(", ");
            selectText.append(recordsPhrase(t));
        }
    } else {
        selectText = createFieldPhrase(fields);
    }

    QStringList joinedOrders;
    QString orderText = createOrderPhrase(order);
    QString whereText = createConditionalPhrase(where.data);
    QString fromText = join(tableName, joins, &joinedOrders);

    QString sql = "SELECT " + selectText + " FROM " + fromText;

    if (whereText != "")
        sql.append(" WHERE " + whereText);

    if (orderText != "")
        sql.append(" ORDER BY " + orderText);

//    for (int i = 0; i < _database->model().count(); i++)
//        sql = sql.replace(_database->model().at(i)->className() + ".",
//                          _database->model().at(i)->name() + ".");

    appendSkipTake(sql, skip, take);
    replaceTableNames(sql);

    return sql + " ";
}

QString SqlGeneratorBase::selectCommand(const QString &tableName,
                                        const SqlGeneratorBase::AgregateType &t,
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

    QString sql = "SELECT " + selectText + " FROM " + fromText;

    if (whereText != "")
        sql.append(" WHERE " + whereText);

    for (int i = 0; i < _database->model().count(); i++)
        sql = sql.replace(_database->model().at(i)->className() + ".",
                          _database->model().at(i)->name() + ".");

    appendSkipTake(sql, skip, take);
    replaceTableNames(sql);

    return sql + " ";
}

QString SqlGeneratorBase::deleteCommand(const QString &tableName,
                                        const ConditionalPhrase &where)
{
    QString command = "DELETE FROM " + tableName;
    QString whereText = createConditionalPhrase(where.data);

    if (whereText != "")
        command.append(" WHERE " + whereText);

    for (int i = 0; i < _database->model().count(); i++)
        command = command.replace(_database->model().at(i)->className() + ".",
                                  _database->model().at(i)->name() + ".");

    replaceTableNames(command);

    return command;
}

QString SqlGeneratorBase::updateCommand(const QString &tableName,
                                        const AssignmentPhraseList &assigments,
                                        const ConditionalPhrase &where)
{
    QString assigmentTexts = QString();
    foreach (PhraseData *d, assigments.data) {
        if (assigmentTexts != "")
            assigmentTexts.append(", ");

        assigmentTexts.append(createConditionalPhrase(d));
    }
    QString whereText = createConditionalPhrase(where.data);

    QString sql = "UPDATE " + tableName + " SET " + assigmentTexts;

    if (whereText != "")
        sql.append(" WHERE " + whereText);

    for (int i = 0; i < _database->model().count(); i++)
        sql = sql.replace(_database->model().at(i)->className() + ".",
                          _database->model().at(i)->name() + ".");

    removeTableNames(sql);

    return sql;
}

QString SqlGeneratorBase::insertCommand(const QString &tableName, const AssignmentPhraseList &assigments)
{

    QString fieldNames;
    QString values;
    foreach (PhraseData *d, assigments.data) {
        if (fieldNames != "")
            fieldNames.append(", ");

        if (values != "")
            values.append(", ");

        fieldNames.append(d->left->fieldName);
        values.append(escapeValue(d->operand));
    }
    return QString("INSERT INTO %1 (%2) VALUES (%3);")
              .arg(tableName, fieldNames, values);
}

//QString SqlGeneratorBase::selectCommand(SqlGeneratorBase::AgregateType t,
//                                        QString agregateArg,
//                                        QString tableName,
//                                        QList<WherePhrase> &wheres,
//                                        QList<WherePhrase> &orders,
//                                        QList<RelationModel*> joins,
//                                        int skip, int take)
//{
//    Q_UNUSED(take)
//    Q_UNUSED(skip)

//    QStringList joinedOrders;
//    QString select = agregateText(t, agregateArg);

//    //TODO: temporatory disabled
//    if (t == SelectAll) {
//        QSet<TableModel*> tables;
//        tables.insert(_database->model().tableByName(tableName));
//        foreach (RelationModel *rel, joins)
//            tables << rel->masterTable << rel->slaveTable;

//        select = "";
//        foreach (TableModel *t, tables) {
//            if (!select.isEmpty())
//                select.append(", ");
//            select.append(recordsPhrase(t));
//        }
//    }
//    QString from = join(tableName, joins, &joinedOrders);
//    QString where = createWhere(wheres);
//    QString orderText = joinedOrders.join(", ");

//    foreach (WherePhrase p, orders) {
//        if (orderText != "")
//            orderText.append(", ");
//        orderText.append(phraseOrder(p.data()));
//    }

//    QString sql = "SELECT " + select + " FROM " + from;

//    if (where != "")
//        sql.append(" WHERE " + where);

//    if (orderText != "")
//        sql.append(" ORDER BY " + orderText);

//    for (int i = 0; i < _database->model().count(); i++)
//        sql = sql.replace(_database->model().at(i)->className() + ".",
//                          _database->model().at(i)->name() + ".");

//    replaceTableNames(sql);

//    return sql + " ";
//}

//QString SqlGeneratorBase::createWhere(QList<WherePhrase> &wheres)
//{
//    QString whereText = "";

//    foreach (WherePhrase w, wheres) {
//        if (whereText != "")
//            whereText.append(" AND ");

//        whereText.append(phrase(w.data()));
//    }

//    return whereText;
//}

void SqlGeneratorBase::replaceTableNames(QString &command)
{
    foreach (TableModel *m, _database->model())
        command = command
                .replace("[" + m->className() + "]", m->name());
}

void SqlGeneratorBase::removeTableNames(QString &command)
{
    foreach (TableModel *m, _database->model())
        command = command.replace("[" + m->className() + "].", "");
}

QString SqlGeneratorBase::dateTimePartName(const PhraseData::Condition &op) const
{
    switch (op) {
    case PhraseData::DatePartYear:
    case PhraseData::AddYears:
    case PhraseData::AddYearsDateTime:
        return "YEAR";
    case PhraseData::DatePartMonth:
    case PhraseData::AddMonths:
    case PhraseData::AddMonthsDateTime:
        return "MONTH";
    case PhraseData::DatePartDay:
    case PhraseData::AddDays:
    case PhraseData::AddDaysDateTime:
        return "DAY";
    case PhraseData::DatePartHour:
    case PhraseData::AddHours:
    case PhraseData::AddHoursDateTime:
        return "HOUR";
    case PhraseData::DatePartMinute:
    case PhraseData::AddMinutes:
    case PhraseData::AddMinutesDateTime:
        return "MINUTE";
    case PhraseData::DatePartSecond:
    case PhraseData::AddSeconds:
    case PhraseData::AddSecondsDateTime:
        return "SECOND";

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

QString SqlGeneratorBase::escapeValue(const QVariant &v) const
{
    if (v.type() == QVariant::String && v.toString().isEmpty())
        return "''";

    QString serialized = _serializer->serialize(v);
    if (serialized.isEmpty()) {
         qWarning("No field escape rule for: %s", v.typeName());
         return QString();
    }

    if (v.type() == QVariant::List)
        return serialized;

    return "'" + serialized + "'";
}

QVariant SqlGeneratorBase::unescapeValue(const QMetaType::Type &type,
                                     const QVariant &dbValue)
{
    return _serializer->deserialize(dbValue.toString(), type);
}

QString SqlGeneratorBase::phrase(const PhraseData *d) const
{
    QString ret = QString();

    switch (d->type) {
    case PhraseData::Field:
        ret = d->toString();
        break;

    case PhraseData::WithVariant:
        ret = phrase(d->left) + " " + operatorString(d->operatorCond) + " "
              + escapeValue(d->operand);
        break;

    case PhraseData::WithOther:
        ret = phrase(d->left) + " " + operatorString(d->operatorCond) + " "
              + phrase(d->right);
        break;

    case PhraseData::WithoutOperand:
        ret = phrase(d->left) + " " + operatorString(d->operatorCond);
        break;
    }

    if (d->operatorCond == PhraseData::And || d->operatorCond == PhraseData::Or)
        ret = "(" + ret + ")";

    return ret;
}

QString
SqlGeneratorBase::operatorString(const PhraseData::Condition &cond) const
{
    switch (cond) {
    case PhraseData::Equal:
        return "=";
    case PhraseData::NotEqual:
        return "<>";
    case PhraseData::Less:
        return "<";
    case PhraseData::Greater:
        return ">";
    case PhraseData::LessEqual:
        return "<=";
    case PhraseData::GreaterEqual:
        return ">=";
    case PhraseData::Null:
        return "IS NULL";

    case PhraseData::NotNull:
        return "IS NOT NULL";

    case PhraseData::In:
        return "IN";

    case PhraseData::NotIn:
        return "NOT IN";

    case PhraseData::And:
        return "AND";
    case PhraseData::Or:
        return "OR";

    case PhraseData::Like:
        return "LIKE";
    case PhraseData::NotLike:
        return "NOT LIKE";

    case PhraseData::Add:
        return "+";
    case PhraseData::Minus:
        return "-";
    case PhraseData::Multiple:
        return "*";
    case PhraseData::Divide:
        return "/";

//    case PhraseData::Set:
//        return "=";

//    case PhraseData::Append:
//        return ",";

    case PhraseData::Between:
        return "BETWEEN";

    case PhraseData::Mod:
        return "MOD";

    default:
        qDebug() << "Unsupported" << cond;
        return QString("<FAIL cond> %1").arg(cond);
    }
}

void SqlGeneratorBase::appendSkipTake(QString &sql, int skip, int take)
{
    Q_UNUSED(sql)
    Q_UNUSED(skip)
    Q_UNUSED(take)
}

QString SqlGeneratorBase::primaryKeyConstraint(const TableModel *table) const
{
    return  QString("CONSTRAINT pk_%1 PRIMARY KEY (%2)")
            .arg(table->name(), table->primaryKey());
}

QString SqlGeneratorBase::createConditionalPhrase(const PhraseData *d) const
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
            ret = QString("%1 BETWEEN %2 AND %3")
                    .arg(createConditionalPhrase(d->left), escapeValue(list.at(0)), escapeValue(list.at(1)));

        } else if (op == PhraseData::DatePartYear)
            ret = QString("DATEPART(year, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartMonth)
            ret = QString("DATEPART(month, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartDay)
            ret = QString("DATEPART(day, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartHour)
            ret = QString("DATEPART(hour, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartMinute)
            ret = QString("DATEPART(minute, %1)")
                    .arg(d->operand.toString());
        else if (op == PhraseData::DatePartMilisecond)
            ret = QString("DATEPART(milisecond, %1)")
                    .arg(d->operand.toString());
        else
            ret = createConditionalPhrase(d->left) + " " + operatorString(op) + " "
              + escapeValue(d->operand);
        break;

    case PhraseData::WithOther:
        ret = createConditionalPhrase(d->left) + " " + operatorString(op) + " "
              + createConditionalPhrase(d->right);
        break;

    case PhraseData::WithoutOperand:
        ret = createConditionalPhrase(d->left) + " " + operatorString(op);
        break;
    }

    if (d->operatorCond == PhraseData::And || d->operatorCond == PhraseData::Or)
        ret = "(" + ret + ")";

    return ret;
}

QString SqlGeneratorBase::createOrderPhrase(const PhraseList &ph)
{
    QString ret = QString();
    foreach (const PhraseData *d, ph.data) {
        if (ret != "")
            ret.append(", ");
        ret.append(d->toString());
        if (d->isNot)
            ret.append(" DESC");
    }

    return ret;
}

QString SqlGeneratorBase::createFieldPhrase(const PhraseList &ph)
{
    QString ret = QString();
    foreach (const PhraseData *d, ph.data) {
        if (ret != "")
            ret.append(", ");
        ret.append(d->toString());
        if (d->isNot)
            qDebug() << "Operator ! is ignored in fields phrase";
    }
    return ret;
}

void SqlGeneratorBase::createInsertPhrase(const AssignmentPhraseList &ph, QString &fields, QString &values)
{
    foreach (PhraseData *d, ph.data) {
        if (fields != "")
            fields.append(", ");

        if (values != "")
            values.append(", ");

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
