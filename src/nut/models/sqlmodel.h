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

#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QExplicitlySharedDataPointer>
#include <QtCore/QList>

#include <QtNut/nut_global.h>

NUT_BEGIN_NAMESPACE

class Database;
class AbstractTableSet;
class Table;
class TableModel;
class SqlModelPrivate;
class NUT_EXPORT SqlModel : public QAbstractTableModel
{
    Q_OBJECT
    SqlModelPrivate *d_ptr;
    Q_DECLARE_PRIVATE(SqlModel)

public:
//    explicit SqlModel(Query *q);
    explicit SqlModel(Database *database, AbstractTableSet *tableSet, QObject *parent = Q_NULLPTR);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    template<class T>
    void setTable(RowList<T> rows);

    void setRows(RowList<Table> rows);
    void append(Row<Table> table);
//    void append(Table *table);
    Row<Table> at(const int &i) const;

    void setRenderer(const std::function<QVariant (int, QVariant)> &renderer);

Q_SIGNALS:
    void beforeShowText(int col, QVariant &value);
};

template<class T>
Q_OUTOFLINE_TEMPLATE void SqlModel::setTable(RowList<T> rows)
{
    RowList<Table> tab;
    Q_FOREACH (auto t, rows)
        tab.append(t);
    setRows(tab);
}


NUT_END_NAMESPACE

#endif // SQLMODEL_H
