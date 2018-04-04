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
#include "defines.h"

NUT_BEGIN_NAMESPACE

class Database;
class TableSetBase;
class SqlModelPrivate;
class Table;
class TableModel;
class SqlModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SqlModel(Database *database, TableSetBase *tableSet, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    SqlModelPrivate *d_ptr;
    Q_DECLARE_PRIVATE(SqlModel)
};

NUT_END_NAMESPACE

#endif // SQLMODEL_H
