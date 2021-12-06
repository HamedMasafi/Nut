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

#ifndef CHANGELOGTABLE_H
#define CHANGELOGTABLE_H

#include <QtCore/qglobal.h>
#include <QtNut/table.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class NUT_EXPORT ChangeLogTable : public Table
{
    Q_OBJECT

    NUT_PRIMARY_AUTO_INCREMENT(id)
    NUT_DECLARE_FIELD(int, id, id, setId)

    NUT_DECLARE_FIELD(QString, data, data, setData)

    NUT_DECLARE_FIELD(int, version, version, setVersion)

public:
    explicit ChangeLogTable(QObject *parentTableSet = Q_NULLPTR);
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

Q_DECLARE_METATYPE(NUT_WRAP_NAMESPACE(ChangeLogTable*))

#endif // CHANGELOGTABLE_H
