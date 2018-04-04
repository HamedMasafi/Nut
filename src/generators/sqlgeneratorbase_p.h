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

#ifndef SQLGENERATORBASE_H
#define SQLGENERATORBASE_H

#include <QtCore/qglobal.h>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "../phrase.h"
//#include "../wherephrase.h"

NUT_BEGIN_NAMESPACE

class Table;
struct FieldModel;
class DatabaseModel;
class TableModel;
class Database;
struct RelationModel;
class SqlGeneratorBase : public QObject
{
//    Q_OBJECT

    Database *_database;
public:
    //TODO: remove this enum
    enum CommandType{
        Select,
        Insert,
        Update,
        Delete
    };
    enum AgregateType{
//        SelectAll,
        Count,
        Min,
        Max,
        Average,
        SignleField
    };

    explicit SqlGeneratorBase(Database *parent = nullptr);
    virtual ~SqlGeneratorBase();

    virtual QString masterDatabaseName(QString databaseName);

    virtual QString createTable(TableModel *table);

    virtual QString fieldType(FieldModel *field) = 0;
    virtual QString fieldDeclare(FieldModel *field);
    virtual QString relationDeclare(const RelationModel *relation);

    virtual QStringList diff(DatabaseModel lastModel, DatabaseModel newModel);
    virtual QString diff(FieldModel *oldField, FieldModel *newField);
    virtual QString diff(TableModel *oldTable, TableModel *newTable);
    virtual QString diffRelation(TableModel *oldTable, TableModel *newTable);
    virtual QString diff(RelationModel *oldRel, RelationModel *newRel);

    virtual QString join(const QString &mainTable,
                         const QList<RelationModel*> list,
                         QStringList *order = nullptr);
    virtual QString join(const QStringList &list, QStringList *order = nullptr);

    virtual QString saveRecord(Table *t, QString tableName);

    virtual QString recordsPhrase(TableModel *table);

    virtual QString insertRecord(Table *t, QString tableName);
    virtual QString updateRecord(Table *t, QString tableName);
    virtual QString deleteRecord(Table *t, QString tableName);
    virtual QString deleteRecords(QString tableName, QString where);

    virtual QString selectCommand(const QString &tableName,
                                  const PhraseList &fields,
                                  const ConditionalPhrase &where,
                                  const PhraseList &order,
                                  const QList<RelationModel*> joins,
                                  const int skip = -1,
                                  const int take = -1);

    virtual QString selectCommand(const QString &tableName,
                                  const AgregateType &t,
                                  const QString &agregateArg,
                                  const ConditionalPhrase &where,
                                  const QList<RelationModel *> &joins,
                                  const int skip = -1,
                                  const int take = -1);

    virtual QString deleteCommand(const QString &tableName,
                                  const ConditionalPhrase &where);
    virtual QString updateCommand(const QString &tableName,
                                  const AssignmentPhraseList &assigments,
                                  const ConditionalPhrase &where);
//    virtual QString selectCommand(AgregateType t,
//                                  QString agregateArg, QString tableName,
//                                  QList<WherePhrase> &wheres,
//                                  QList<WherePhrase> &orders,
//                                  QList<RelationModel*> joins,
//                                  int skip = -1, int take = -1);

//    virtual QString deleteCommand(QList<WherePhrase> &wheres, QString tableName);

//    virtual QString updateCommand(WherePhrase &phrase, QList<WherePhrase> &wheres, QString tableName);

    virtual QString escapeValue(const QVariant &v) const;
    virtual QVariant readValue(const QVariant::Type &type, const QVariant &dbValue);
    virtual QString phrase(const PhraseData *d) const;
    virtual QString operatorString(const PhraseData::Condition &cond) const;
    virtual void appendSkipTake(QString &sql, int skip = -1, int take = -1);
protected:
    QString createConditionalPhrase(const PhraseData *d) const;
    QString createFieldPhrase(const PhraseList &ph);
    QString createOrderPhrase(const PhraseList &ph);
    void createInsertPhrase(const AssignmentPhraseList &ph, QString &fields, QString &values);

    QString agregateText(const AgregateType &t, const QString &arg = QString()) const;
    QString fromTableText(const QString &tableName, QString &joinClassName, QString &orderBy) const;
//    QString createWhere(QList<WherePhrase> &wheres);
    void replaceTableNames(QString &command);
    void removeTableNames(QString &command);
};

NUT_END_NAMESPACE

#endif // SQLGENERATORBASE_H
