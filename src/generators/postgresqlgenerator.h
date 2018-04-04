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

#ifndef POSTGRESQLGENERATOR_H
#define POSTGRESQLGENERATOR_H

#include <QtCore/qglobal.h>
#include "sqlgeneratorbase_p.h"

NUT_BEGIN_NAMESPACE

class PostgreSqlGenerator : public SqlGeneratorBase
{
public:
    explicit PostgreSqlGenerator(Database *parent = nullptr);

    QString fieldType(FieldModel *field);

    QString diff(FieldModel *oldField, FieldModel *newField);
};

NUT_END_NAMESPACE

#endif // POSTGRESQLGENERATOR_H
