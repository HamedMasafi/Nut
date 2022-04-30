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

#include <QWeakPointer>

#include "table.h"
#include "database.h"
#include "abstracttableset.h"
#include "databasemodel.h"
#include "abstracttablesetdata.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

AbstractTableSet::AbstractTableSet(Database *parent) : QObject(parent),
    data(new AbstractTableSetData(parent))
{
    parent->add(this);
}

AbstractTableSet::AbstractTableSet(Table *parent) : QObject(parent),
    data(new AbstractTableSetData(parent))
{
    parent->add(this);
}

AbstractTableSet::~AbstractTableSet()
{
    for (auto &t: data->children)
        if (t)
            t->setParentTableSet(nullptr);
}

int AbstractTableSet::save(Database *db)
{
    int rowsAffected = 0;
    TableModel *masterModel = nullptr;
    if (data->table)
        masterModel = db->model().tableByClassName(
            QString::fromUtf8(data->table->metaObject()->className()));

    for (auto i = data->weakChildren.begin(); i != data->weakChildren.end(); ) {
        auto &row = *i;

        if (!row) {
            i = data->weakChildren.erase(i);
            continue;
        }
        auto t = row.lock();
        if (t.isNull()) {
            i = data->weakChildren.erase(i);
            continue;
        }

        if (data->table)
            t->setParentTable(data->table,
                              masterModel,
                              db->model().tableByClassName(
                                  QString::fromUtf8(t->metaObject()->className())));

        if (t->status() == Table::Added || t->status() == Table::Modified
            || t->status() == Table::Deleted) {
            rowsAffected += t->save(db);
        }
        i++;
    }

    for (auto i = data->children.begin(); i != data->children.end(); ) {
        auto &t = *i;
        if (!t) {
            i = data->children.erase(i);
            continue;
        }

        if (data->table)
            t->setParentTable(data->table,
                              masterModel,
                              db->model().tableByClassName(
                                  QString::fromUtf8(t->metaObject()->className())));

        if (t->status() == Table::Added || t->status() == Table::Modified
            || t->status() == Table::Deleted) {
            rowsAffected += t->save(db);
            data->weakChildren.append(t.toWeakRef());

            i = data->children.erase(i);
            continue;
        }
        i++;
    }

//    data->children.clear();

    return rowsAffected;
}

    void AbstractTableSet::clearChildren()
    {
#ifdef NUT_RAW_POINTER
    for (auto &t: data->children)
        t->deleteLater();
#endif
    data->children.clear();
}

void AbstractTableSet::add(Row<Table> t)
{
    data.detach();
    data->children.append(t);
    t->setParentTableSet(this);
}

void AbstractTableSet::add(WeakRow<Table> t)
{
    data.detach();
    data->weakChildren.append(t);
    t.toStrongRef()->setParentTableSet(this);
}

void AbstractTableSet::remove(Row<Table> t)
{
    data.detach();
    data->children.removeAll(t);

#ifdef NUT_RAW_POINTER
    t->deleteLater();
#endif

}

void AbstractTableSet::remove(WeakRow<Table> t)
{
    data.detach();
    data->weakChildren.removeAll(t);
}

QString AbstractTableSet::childClassName() const
{
    return data->childClassName;
}

Database *AbstractTableSet::database() const
{
    return data->database;
}

void AbstractTableSet::setDatabase(Database *database)
{
    data.detach();
    data->database = database;
}

int AbstractTableSet::size() const
{
    return data->children.size();
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
