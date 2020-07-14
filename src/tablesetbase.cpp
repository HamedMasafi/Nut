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

#include "table.h"
#include "database.h"
#include "tablesetbase_p.h"
#include "databasemodel.h"
#include "tablesetbasedata.h"

NUT_BEGIN_NAMESPACE

TableSetBase::TableSetBase(Database *parent) : QObject(parent),
    data(new TableSetBaseData(parent))
{
    parent->add(this);
}

TableSetBase::TableSetBase(Table *parent) : QObject(parent),
    data(new TableSetBaseData(parent))
{
    parent->add(this);
}

TableSetBase::~TableSetBase()
{
    foreach (Row<Table> t, data->childs)
        if (t)
            t->setParentTableSet(nullptr);
}

int TableSetBase::save(Database *db, bool cleanUp)
{
    int rowsAffected = 0;
    TableModel *masterModel = nullptr;
    if (data->table)
        masterModel = db->model().tableByClassName(data->table->metaObject()->className());

    foreach (Row<Table> t, data->childs) {
        if (data->table)
            t->setParentTable(data->table,
                              masterModel,
                              db->model().tableByClassName(t->metaObject()->className()));

        if (t->status() == Table::Added
            || t->status() == Table::Modified
            || t->status() == Table::Deleted) {
            rowsAffected += t->save(db);
            if (cleanUp)
#ifdef NUT_SHARED_POINTER
                remove(t);
#else
                t->deleteLater();
#endif
        }
    }

    if (cleanUp)
        data->childs.clear();

    return rowsAffected;
}

void TableSetBase::clearChilds()
{
#ifndef NUT_SHARED_POINTER
    foreach (Table *t, data->childs)
        t->deleteLater();
#endif
    data->childs.clear();
}

void TableSetBase::add(Row<Table> t)
{
    data.detach();
    data->childs.append(t);
    t->setParentTableSet(this);
}

void TableSetBase::remove(Row<Table> t)
{
    data.detach();
    data->childs.removeAll(t);
}

QString TableSetBase::childClassName() const
{
    return data->childClassName;
}

Database *TableSetBase::database() const
{
    return data->database;
}

void TableSetBase::setDatabase(Database *database)
{
    data.detach();
    data->database = database;
}

NUT_END_NAMESPACE
