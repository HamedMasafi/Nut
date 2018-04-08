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

#include "postgresqlgenerator.h"
#include "../table.h"
#include "../tablemodel.h"

NUT_BEGIN_NAMESPACE

PostgreSqlGenerator::PostgreSqlGenerator(Database *parent) : SqlGeneratorBase (parent)
{

}

QString PostgreSqlGenerator::fieldType(FieldModel *field)
{
    QString dbType;

    switch (field->type) {
    case QVariant::Bool:
        dbType = "BOOLEAN";
        break;
    case QVariant::ByteArray:
        dbType = "BYTEA";
        break;
    case QVariant::Date:
        dbType = "DATE";
        break;
    case QVariant::DateTime:
        dbType = "TIMESTAMP";
        break;
    case QVariant::Time:
        dbType = "TIME";
        break;

    case QVariant::Int:
    case QVariant::UInt:
        if(field->isAutoIncrement)
            dbType = "SERIAL";
        else
            dbType = "INTEGER";
        break;

    case QVariant::ULongLong:
    case QVariant::LongLong:
        if(field->isAutoIncrement)
            dbType = "BIGSERIAL";
        else
            dbType = "BIGINT";
        break;

    case QVariant::Double:
        dbType = "REAL";
        break;
    case QVariant::String:
        if(field->length)
            dbType = QString("VARCHAR(%1)").arg(field->length);
        else
            dbType = "TEXT";
        break;

    case QVariant::Point:
    case QVariant::PointF:
        dbType="POINT";
        break;

    case QVariant::Uuid:
        dbType = "UUID";
        break;

    case QVariant::Polygon:
    case QVariant::PolygonF:
        dbType = "POLYGON";
        break;

    default:
        qDebug() << "Type for " << (int)field->type << field->type << "(" << QMetaType::typeName(field->type) << ")" << "nut supported";
        dbType = QString();
    }

    if(field->type == (unsigned)QMetaType::type("Nut::DbGeography"))
        dbType = "GEOGRAPHY";

    return dbType;
}

QString PostgreSqlGenerator::diff(FieldModel *oldField, FieldModel *newField)
{
    QString sql = QString();
    if(oldField && newField)
        if(*oldField == *newField)
            return QString();

    if(!newField){
        sql = "DROP COLUMN " + oldField->name;
    }else{
        if(oldField){
            sql = "ALTER COLUMN ";
            sql.append(newField->name + " TYPE " + fieldType(newField));
        } else {
            sql = "ADD COLUMN ";
            sql.append(fieldDeclare(newField));
        }
    }
    return sql;
}


NUT_END_NAMESPACE
