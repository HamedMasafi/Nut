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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Nut API.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef SQLMODEL_P_H
#define SQLMODEL_P_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include <functional>

#include <QtNut/nut_global.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class SqlModel;
class Table;
class TableModel;
class NUT_EXPORT SqlModelPrivate {
public:
    SqlModel *q_ptr;
    Q_DECLARE_PUBLIC(SqlModel);

    explicit SqlModelPrivate(SqlModel *parent);

    QString tableName;

    RowList<Table> rows;
    TableModel *model;
    std::function <QVariant(int, QVariant)> renderer;
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // SQLMODEL_P_H
