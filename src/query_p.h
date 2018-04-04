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

#ifndef QUERY_P_H
#define QUERY_P_H

#include "phrase.h"

#include <QList>
#include <QString>

NUT_BEGIN_NAMESPACE

class Database;
class TableSetBase;
class QueryBase;
struct RelationModel;
class QueryPrivate{
    QueryBase *q_ptr;
    Q_DECLARE_PUBLIC(QueryBase)

public:
    explicit QueryPrivate(QueryBase *parent = nullptr);
    ~QueryPrivate();

    QString sql;
    QString className;
    QString tableName;
    QString select;
    Database *database;
    TableSetBase *tableSet;
    QStringList joins;
    QList<RelationModel*> relations;
    int skip;
    int take;
    PhraseList orderPhrase, fieldPhrase;
    ConditionalPhrase wherePhrase;

//    QList<WherePhrase> wheres;
//    QList<WherePhrase> orderPhrases;
//    QList<WherePhrase> fields;
//    QHash<QString, QString> orders;
};

NUT_END_NAMESPACE

#endif // QUERY_P_H
