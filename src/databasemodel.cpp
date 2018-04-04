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

#include "databasemodel.h"
#include "tablemodel.h"

#include <QJsonArray>
#include <QJsonObject>

NUT_BEGIN_NAMESPACE

QMap<QString, DatabaseModel*> DatabaseModel::_models;

#define NODE_VERSION "version"
#define NODE_TABLES  "tables"
DatabaseModel::DatabaseModel(const QString &name) :
    QList<TableModel*>(), _databaseClassName(name), _version(QString())
{
    _models.insert(name, this);
}

DatabaseModel::DatabaseModel(const DatabaseModel &other) :
    QList<TableModel*>(other), _version(QString())
{

}

DatabaseModel::DatabaseModel(const QJsonObject &json) :
    QList<TableModel*>()
{
    setVersion(json.value(NODE_VERSION).toString());

    QJsonObject tables = json.value(NODE_TABLES).toObject();
    foreach (QString key, tables.keys()) {
        if(!tables.value(key).isObject())
            continue;

        TableModel *sch = new TableModel(tables.value(key).toObject(), key);
        append(sch);
    }
}

DatabaseModel::~DatabaseModel()
{
}

TableModel *DatabaseModel::tableByName(QString tableName) const
{
    for(int i = 0; i < size(); i++){
        TableModel *s = at(i);

        if(s->name() == tableName)
            return s;
    }

//    qWarning("Table with name '%s' not found in model",
//             qUtf8Printable(tableName));
    return 0;
}

TableModel *DatabaseModel::tableByClassName(QString className) const
{
    QStringList l;
    for(int i = 0; i < size(); i++){
        TableModel *s = at(i);

        l.append(s->className());
        if(s->className() == className)
            return s;
    }

    return 0;
}

bool DatabaseModel::operator ==(const DatabaseModel &other) const
{
    if(size() != other.size())
        return false;

    for(int i = 0; i < size(); i++){
        TableModel *mine = at(i);
        TableModel *others = other.tableByName(mine->name());

        if(!others)
            return false;

        if(*mine != *others)
            return false;
    }

    return true;
}

DatabaseModel DatabaseModel::operator +(const DatabaseModel &other)
{
    DatabaseModel model;
    DatabaseModel::const_iterator i;

    for (i = constBegin(); i != constEnd(); ++i)
        model.append(*i);

    for (i = other.constBegin(); i != other.constEnd(); ++i)
        model.append(*i);

    return model;
}

QJsonObject DatabaseModel::toJson() const
{
    QJsonObject obj;

    obj.insert(NODE_VERSION, QJsonValue(_version));
    QJsonObject tables;
    for(int i = 0; i < size(); i++){
        TableModel *s = at(i);
        tables.insert(s->name(), s->toJson());
    }

    obj.insert(NODE_TABLES, tables);

    return obj;
}

DatabaseModel::operator QJsonObject()
{
    return toJson();
}

RelationModel *DatabaseModel::relationByClassNames(const QString &masterClassName, const QString &childClassName)
{
    TableModel *childTable = tableByClassName(childClassName);

    if(!childTable)
        return 0;

    foreach (RelationModel *rel, childTable->foregionKeys())
        if(rel->masterClassName == masterClassName)
            return rel;

    return 0;
}

RelationModel *DatabaseModel::relationByTableNames(const QString &masterTableName, const QString &childTableName)
{
    TableModel *childTable = tableByName(childTableName);

    if(!childTable)
        return 0;

    foreach (RelationModel *rel, childTable->foregionKeys())
        if(rel->masterTable->name() == masterTableName)
            return rel;

    return 0;
}

DatabaseModel DatabaseModel::fromJson(QJsonObject &json)
{
    DatabaseModel model;

    model.setVersion(json.value(NODE_VERSION).toString());

    QJsonObject tables = json.value(NODE_TABLES).toObject();
    foreach (QString key, tables.keys()) {
        if(!json.value(key).isObject())
            continue;

        TableModel *sch = new TableModel(json.value(key).toObject(), key);
        model.append(sch);
    }
    return model;
}

QString DatabaseModel::version() const
{
    return _version;
}

void DatabaseModel::setVersion(QString version)
{
    _version = version;
}

bool DatabaseModel::remove(const QString &tableName)
{
    for(int i = 0; i < size(); i++){
        TableModel *s = at(i);
        if(s->name() == tableName){
            removeAt(i);
            return true;
        }
    }
    return false;
}

void DatabaseModel::fixRelations()
{
    /*TODO: fixme
    foreach (TableModel *table, currentModel)
        foreach (RelationModel *fk, table->foregionKeys())
            fk->masterTable = currentModel.tableByClassName(fk->masterClassName);
            */
}

DatabaseModel *DatabaseModel::modelByName(const QString &name)
{
    if (_models.contains(name))
        return _models[name];

    return nullptr;
}

void DatabaseModel::deleteAllModels()
{
    QMapIterator<QString, DatabaseModel*> i(_models);
     while (i.hasNext()) {
         i.next();
//         cout << i.key() << ": " << i.value() << endl;
         delete i.value();
     }
//    qDeleteAll(_models.values());
    _models.clear();
}

NUT_END_NAMESPACE
