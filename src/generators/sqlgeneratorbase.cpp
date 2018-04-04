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
SqlGeneratorBase::SqlGeneratorBase(Database *parent)
    : QObject((QObject *)parent)
{
    if (parent)
        _database = parent;
}

SqlGeneratorBase::~SqlGeneratorBase()
{
}

QString SqlGeneratorBase::masterDatabaseName(QString databaseName)
{
    Q_UNUSED(databaseName);
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
        ret.append(QString("%1.%2 AS [%1.%2]").arg(table->name()).arg(f->name));
    }
    return ret;
}

QString SqlGeneratorBase::fieldDeclare(FieldModel *field)
{
    return field->name + " " + fieldType(field) + (field->notNull ? " NOT NULL" : "");
}

QString SqlGeneratorBase::relationDeclare(const RelationModel *relation)
{
    return QString("FOREIGN KEY (FK_%1) REFERENCES %2(%1)")
            .arg(relation->localColumn)
            .arg(relation->slaveTable->name());
}

QStringList SqlGeneratorBase::diff(DatabaseModel lastModel,
                                   DatabaseModel newModel)
{
    QStringList ret;

    DatabaseModel unionModel = lastModel + newModel;

    foreach (TableModel *table, unionModel) {
        TableModel *oldTable = lastModel.tableByName(table->name());
        TableModel *newTable = newModel.tableByName(table->name());
        QString sql = diff(oldTable, newTable);
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
            return QString();

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

QString SqlGeneratorBase::diff(TableModel *oldTable, TableModel *newTable)
{
    if (oldTable && newTable)
        if (*oldTable == *newTable)
            return QString();

    if (!newTable)
        return "DROP TABLE " + oldTable->name();

    QList<QString> fieldNames;
    QList<QString> relations;

    if (oldTable) {
        foreach (FieldModel *f, oldTable->fields())
            if (!fieldNames.contains(f->name))
                fieldNames.append(f->name);
        foreach (RelationModel *r, oldTable->foregionKeys())
            if (!relations.contains(r->localColumn))
                relations.append(r->localColumn);
    }

    foreach (FieldModel *f, newTable->fields())
        if (!fieldNames.contains(f->name))
            fieldNames.append(f->name);
    foreach (RelationModel *r, newTable->foregionKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QStringList columnSql;
    foreach (QString fieldName, fieldNames) {
        FieldModel *newField = newTable->field(fieldName);
        if (oldTable) {
            FieldModel *oldField = oldTable->field(fieldName);

            QString buffer = diff(oldField, newField);
            if (!buffer.isNull())
                columnSql << buffer;
        } else {
            columnSql << fieldDeclare(newField);
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
                .arg(newTable->name())
                .arg(columnSql.join(",\n"));
    } else {
        if (!newTable->primaryKey().isNull())
            columnSql << QString("CONSTRAINT pk_%1 PRIMARY KEY (%2)")
                             .arg(newTable->name())
                             .arg(newTable->primaryKey());

        sql = QString("CREATE TABLE %1 \n(%2)")
                .arg(newTable->name())
                .arg(columnSql.join(",\n"));
    }
    return sql;
}

QString SqlGeneratorBase::diffRelation(TableModel *oldTable, TableModel *newTable)
{
    if (!newTable)
        return QString();

    QList<QString> relations;

    if (oldTable)
        foreach (RelationModel *r, oldTable->foregionKeys())
            if (!relations.contains(r->localColumn))
                relations.append(r->localColumn);

    foreach (RelationModel *r, newTable->foregionKeys())
        if (!relations.contains(r->localColumn))
            relations.append(r->localColumn);

    QStringList columnSql;
    foreach (QString fieldName, relations) {
        RelationModel *newRelation = newTable->foregionKeyByField(fieldName);
        RelationModel *oldRelation = 0;
        if (oldTable)
            oldRelation = oldTable->foregionKeyByField(fieldName);

        QString buffer = diff(oldRelation, newRelation);
        if (!buffer.isNull())
            columnSql << buffer;
    }

    if (columnSql.count())
        return "ALTER TABLE " + newTable->name() + "\n"
                + columnSql.join(",\n");
    else
        return QString();

}

QString SqlGeneratorBase::diff(RelationModel *oldRel, RelationModel *newRel)
{
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
        return QString("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                       "REFERENCES %2(%3)")
                .arg(newRel->localColumn)
                .arg(newRel->masterTable->name())
                .arg(newRel->foreignColumn);

    if (!newRel)
        return QString("ADD CONSTRAINT FK_%1 FOREIGN KEY (%1) "
                       "REFERENCES %2(%3)")
                .arg(oldRel->localColumn)
                .arg(oldRel->masterTable->name())
                .arg(oldRel->foreignColumn);

//    if (*oldRel == *newRel)
        return QString();
}

QString SqlGeneratorBase::join(const QString &mainTable,
                               const QList<RelationModel*> list,
                               QStringList *order)
{
    QString ret = mainTable;
    QList<RelationModel*>::const_iterator i;
    for (i = list.begin(); i != list.end(); ++i) {
        if ((*i)->masterTable->name() == mainTable) {
            ret.append(QString(" INNER JOIN %3 ON %1.%2 = %3.%4")
                       .arg((*i)->masterTable->name())
                       .arg((*i)->masterTable->primaryKey())
                       .arg((*i)->slaveTable->name())
                       .arg((*i)->localColumn));

            if (order != nullptr)
                order->append((*i)->slaveTable->name() + "." + (*i)->slaveTable->primaryKey());
        } else {
            ret.append(QString(" INNER JOIN %3 ON %1.%2 = %3.%4")
                       .arg(mainTable)
                       .arg((*i)->localColumn)
                       .arg((*i)->masterTable->name())
                       .arg((*i)->masterTable->primaryKey()));

            if (order != nullptr)
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
                       .arg(table)
                       .arg(rel->masterTable->primaryKey())
                       .arg(rel->localColumn)
                       .arg(mainTable));

            if (order != nullptr)
                order->append(mainTable + "." + rel->masterTable->primaryKey());

        } else{
            rel = model.relationByClassNames(clone.first(), mainTable);
            if (rel) {
                // table is master of mainTable
                ret.append(QString(" INNER JOIN [%1] ON %4.%2 = %1.%3")
                           .arg(table)
                           .arg(rel->localColumn)
                           .arg(rel->masterTable->primaryKey())
                           .arg(mainTable));

                if (order != nullptr)
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
    QString key = t->primaryKey();
    QStringList values;

    foreach (QString f, t->changedProperties())
        if (f != key)
            values.append("'" + t->property(f.toLatin1().data()).toString()
                          + "'");

    QString changedPropertiesText = QString();
    QSet<QString> props = t->changedProperties();
    foreach (QString s, props) {
        if (changedPropertiesText != "")
            changedPropertiesText.append(", ");
        changedPropertiesText.append(s);
    }
    sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
              .arg(tableName)
              .arg(changedPropertiesText)
              .arg(values.join(", "));

    removeTableNames(sql);

    return sql;
}

QString SqlGeneratorBase::updateRecord(Table *t, QString tableName)
{
    QString sql = QString();
    QString key = t->primaryKey();
    QStringList values;

    foreach (QString f, t->changedProperties())
        if (f != key)
            values.append(f + "='" + t->property(f.toLatin1().data()).toString()
                          + "'");
    sql = QString("UPDATE %1 SET %2 WHERE %3=%4")
              .arg(tableName)
              .arg(values.join(", "))
              .arg(key)
              .arg(t->primaryValue().toString());

    removeTableNames(sql);

    return sql;
}

QString SqlGeneratorBase::deleteRecord(Table *t, QString tableName)
{
    QString sql = QString("DELETE FROM %1 WHERE %2='%3'")
        .arg(tableName)
        .arg(t->primaryKey())
        .arg(t->primaryValue().toString());
    replaceTableNames(sql);
    return sql;
}

QString SqlGeneratorBase::agregateText(const AgregateType &t,
                                       const QString &arg) const
{
    switch (t) {
    case Min:
        return "MIN(" + arg + ")";
        break;

    case Max:
        return "MAX(" + arg + ")";
        break;

    case Average:
        return "AVERAGE(" + arg + ")";
        break;

    case Count:
        return "COUNT(" + arg + ")";
        break;

    case SignleField:
        return arg;
        break;

    default:
        return QString();
    }
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
                                .arg(tableName)
                                .arg(joinTableName)
                                .arg(pk)
                                .arg(rel->localColumn);
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

QString SqlGeneratorBase::deleteRecords(QString tableName, QString where)
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
                                        const QList<RelationModel*> joins,
                                        const int skip,
                                        const int take)
{
    Q_UNUSED(skip);
    Q_UNUSED(take);
    QString selectText;

    if (!fields.isValid) {
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

    for (int i = 0; i < _database->model().count(); i++)
        sql = sql.replace(_database->model().at(i)->className() + ".",
                          _database->model().at(i)->name() + ".");

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
    Q_UNUSED(skip);
    Q_UNUSED(take);
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

//QString SqlGeneratorBase::selectCommand(SqlGeneratorBase::AgregateType t,
//                                        QString agregateArg,
//                                        QString tableName,
//                                        QList<WherePhrase> &wheres,
//                                        QList<WherePhrase> &orders,
//                                        QList<RelationModel*> joins,
//                                        int skip, int take)
//{
//    Q_UNUSED(take);
//    Q_UNUSED(skip);

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
    foreach (TableModel *m, TableModel::allModels())
        command = command
                .replace("[" + m->className() + "]", "`" + m->name() + "`");
}

void SqlGeneratorBase::removeTableNames(QString &command)
{
    foreach (TableModel *m, TableModel::allModels())
        command = command.replace("[" + m->className() + "].", "");
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
    switch (v.type()) {
    case QVariant::Bool:
        return v.toBool() ? "1" : "0";
        break;

    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::ULongLong:
    case QVariant::LongLong:
    case QVariant::Double:
        return v.toString();
        break;

    case QVariant::Uuid:
        return v.toUuid().toString();
        break;

    case QVariant::Char:
    case QVariant::String:
        return "'" + v.toString() + "'";

    case QVariant::DateTime:
        return "'" + v.toDateTime().toString(Qt::ISODate) + "'";

    case QVariant::Date:
        return "'" + v.toDate().toString(Qt::ISODate) + "'";

    case QVariant::Time:
        return "'" + v.toTime().toString(Qt::ISODate) + "'";

    case QVariant::StringList:
    case QVariant::List:
        return "('" + v.toStringList().join("', '") + "')";

    case QVariant::Point: {
        QPoint pt = v.toPoint();
        return QString("POINT(%1 %2)").arg(pt.x()).arg(pt.y());
    }

    case QVariant::PointF: {
        QPointF pt = v.toPointF();
        return QString("POINT(%1 %2)").arg(pt.x()).arg(pt.y());
    }

    case QVariant::Invalid:
        qFatal("Invalud field value");
        return "<FAIL>";

    default:
        Q_UNREACHABLE();
        return QString();
    }
}

QVariant SqlGeneratorBase::readValue(const QVariant::Type &type,
                                     const QVariant &dbValue)
{
    Q_UNUSED(type);
    return dbValue;
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

    default:
        ret = "<FAIL>";
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
        return QString("<FAIL cond> %1").arg(cond);
    }
}

void SqlGeneratorBase::appendSkipTake(QString &sql, int skip, int take)
{
    Q_UNUSED(sql);
    Q_UNUSED(skip);
    Q_UNUSED(take);
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
            op = (PhraseData::Condition)((op + 10) % 20);
    }
    switch (d->type) {
    case PhraseData::Field:
        ret = d->toString();
        break;

    case PhraseData::WithVariant:
        if (op == PhraseData::AddYears)
            ret = QString("DATEADD(year, %1, %2)")
                    .arg(d->operand.toString()).arg(createConditionalPhrase(d->left));
        else if (op == PhraseData::AddMonths)
            ret = QString("DATEADD(month, %1, %2)")
                    .arg(d->operand.toString()).arg(createConditionalPhrase(d->left));
        else if (op == PhraseData::AddDays)
            ret = QString("DATEADD(day, %1, %2)")
                    .arg(d->operand.toString()).arg(createConditionalPhrase(d->left));
        else if (op == PhraseData::AddHours)
            ret = QString("DATEADD(hour, %1, %2)")
                    .arg(d->operand.toString()).arg(createConditionalPhrase(d->left));
        else if (op == PhraseData::AddMinutes)
            ret = QString("DATEADD(minute, %1, %2)")
                    .arg(d->operand.toString()).arg(createConditionalPhrase(d->left));
        else if (op == PhraseData::AddSeconds)
            ret = QString("DATEADD(second, %1, %2)")
                    .arg(d->operand.toString()).arg(createConditionalPhrase(d->left));
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

    default:
        ret = "<FAIL phrase>";
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
        default:
            qFatal("Invalid insert command");
        }
    }
}

NUT_END_NAMESPACE
