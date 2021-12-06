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
#include "abstracttableset.h"
#include "databasemodel.h"
#include "tablemodel.h"
#include "table.h"
#include "sqlmodel_p.h"
#include "sqlmodel.h"
#include "query.h"

QT_BEGIN_NAMESPACE

SqlModelPrivate::SqlModelPrivate(SqlModel *parent) : q_ptr(parent)
      , renderer(nullptr)
{
    Q_UNUSED(parent)
}

void SqlModel::setRenderer(const std::function<QVariant (int, QVariant)> &renderer)
{
    Q_D(SqlModel);
    d->renderer = renderer;
}

SqlModel::SqlModel(Database *database, AbstractTableSet *tableSet, QObject *parent)
    : QAbstractTableModel(parent)
      , d_ptr(new SqlModelPrivate(this))
{
    Q_D(SqlModel);
    d->model = database->model()
            .tableByClassName(tableSet->childClassName());
    d->tableName = d->model->name();
}

int SqlModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const SqlModel);
    Q_UNUSED(parent)
    return d->rows.count();
}

int SqlModel::columnCount(const QModelIndex &parent) const
{
    Q_D(const SqlModel);
    Q_UNUSED(parent)
    return d->model->fields().count();
}

QVariant SqlModel::data(const QModelIndex &index, int role) const
{
    Q_D(const SqlModel);

    if (!index.isValid())
        return QVariant();

    if (index.row() >= d->rows.count() || index.row() < 0)
        return QVariant::fromValue(QStringLiteral("-"));

    if (role == Qt::DisplayRole) {
        Row<Table> t = d->rows.at(index.row());
        QVariant v = t->property(d->model->field(index.column())->name.toLocal8Bit().data());

        if (d->renderer != nullptr)
            v = d->renderer(index.column(), v);
        return v;
    }
    return QVariant();
}

void SqlModel::setRows(RowList<Table> rows)
{
    Q_D(SqlModel);

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
    Q_D(SqlModel);
    beginInsertRows(QModelIndex(), d->rows.count(), d->rows.count());
    d->rows.append(table);
    endInsertRows();
}

QVariant SqlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const SqlModel);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return d->model->field(section)->displayName;
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

Row<Table> SqlModel::at(const int &i) const
{
    Q_D(const SqlModel);
    return d->rows.at(i);
}

QT_END_NAMESPACE
