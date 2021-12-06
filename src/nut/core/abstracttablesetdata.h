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

#ifndef NUT_ABSTRACTTABLESETDATA_H
#define NUT_ABSTRACTTABLESETDATA_H

#include <QtCore/QSharedData>

#include <QtNut/nut_global.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class Table;
class Database;
class AbstractTableSetData : public QSharedData
{
public:
    AbstractTableSetData(Database *parent) :
        database(parent), table(nullptr)
    { }

    AbstractTableSetData(Table *parent) :
        database(nullptr), table(parent)
    { }

//    QSet<Table*> tables;
//    QList<Table*> childRows;
    RowList<Table> childs;

    Database *database;
    Table *table;
    QString childClassName;
};


NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // NUT_ABSTRACTTABLESETDATA_H
