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

#ifndef TABLEPRIVATE_H
#define TABLEPRIVATE_H

#include <QtCore/QSet>
#include <QtCore/QSharedData>

#include <QtNut/nut_global.h>
#include <QtNut/table.h>

QT_BEGIN_NAMESPACE

class TableModel;
class Table;
class AbstractTableSet;
class TablePrivate : public QSharedData {
    Table *q_ptr;
    Q_DECLARE_PUBLIC(Table)

public:
    TablePrivate();


    TableModel *model;
    Table::Status status;
    QSet<QString> changedProperties;
    AbstractTableSet *parentTableSet;
    QSet<AbstractTableSet*> childTableSets;

    void refreshModel();
};

QT_END_NAMESPACE

#endif // TABLEPRIVATE_H
