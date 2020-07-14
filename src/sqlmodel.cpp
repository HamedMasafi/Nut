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

#include "database.h"
#include "tablesetbase_p.h"
#include "databasemodel.h"
#include "tablemodel.h"
#include "table.h"
#include "sqlmodel_p.h"
#include "sqlmodel.h"
#include "query.h"

NUT_BEGIN_NAMESPACE

//SqlModel::SqlModel(Query *q) : QAbstractItemModel(q.)
//{

//}

void SqlModel::setRenderer(const std::function<QVariant (int, QVariant)> &renderer)
{
    _renderer = renderer;
}

SqlModel::SqlModel(Database *database, TableSetBase *tableSet, QObject *parent)
    : QAbstractTableModel(parent)
    , _renderer(nullptr)
    , d(new SqlModelPrivate(this))
{
    d->model = database->model()
            .tableByClassName(tableSet->childClassName());
    d->tableName = d->model->name();


    //     setQuery("SELECT * FROM " + d->tableName, database->databaseName());
}

int SqlModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return d->rows.count();
}

int SqlModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return d->model->fields().count();
}

QVariant SqlModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= d->rows.count() || index.row() < 0)
        return QVariant("-");

    if (role == Qt::DisplayRole) {
        Row<Table> t = d->rows.at(index.row());
        QVariant v = t->property(d->model->field(index.column())->name.toLocal8Bit().data());

        if (_renderer != nullptr)
            v = _renderer(index.column(), v);
        return v;
    }
    return QVariant();
}

void SqlModel::setRows(RowList<Table> rows)
{
    d.detach();
    if (d->rows.count()) {
        beginRemoveRows(QModelIndex(), 0, d->rows.count());
        d->rows.clear();
        endRemoveRows();
    }
    beginInsertRows(QModelIndex(), 0, rows.count());
    d->rows = rows;
    endInsertRows();
}

void SqlModel::append(Row<Table> table)
{
    d.detach();
    beginInsertRows(QModelIndex(), d->rows.count(), d->rows.count());
    d->rows.append(table);
    endInsertRows();
}

//void SqlModel::append(Table *table)
//{
//    append(TableType<Table>::Row(table));
//}

QVariant SqlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return d->model->field(section)->displayName;
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

Row<Table> SqlModel::at(const int &i) const
{
    return d->rows.at(i);
}

SqlModelPrivate::SqlModelPrivate(SqlModel *parent)
{

}


NUT_END_NAMESPACE
