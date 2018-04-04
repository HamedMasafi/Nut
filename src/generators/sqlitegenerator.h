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

#ifndef SQLITEGENERATOR_H
#define SQLITEGENERATOR_H

#include <QtCore/qglobal.h>
#include "sqlgeneratorbase_p.h"

NUT_BEGIN_NAMESPACE

class SqliteGenerator : public SqlGeneratorBase
{
public:
    explicit SqliteGenerator(Database *parent = nullptr);

    QString fieldType(FieldModel *field);

    void appendSkipTake(QString &sql, int skip, int take);
};

NUT_END_NAMESPACE

#endif // SQLITEGENERATOR_H
