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

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

QT_BEGIN_NAMESPACE

QMap<QString, DatabaseModel*> DatabaseModel::_models;

#define NODE_VERSION QStringLiteral("version")
#define NODE_TABLES  QStringLiteral("tables")
DatabaseModel::DatabaseModel(const QString &name) :
    QList<TableModel*>(), _databaseClassName(name), _version(0)
{
    _models.insert(name, this);
}

DatabaseModel::DatabaseModel(const DatabaseModel &other) :
    QList<TableModel*>(other), _version(0)
{

}

DatabaseModel::DatabaseModel(const QJsonObject &json) :
    QList<TableModel*>()
{
    setVersion(json.value(NODE_VERSION).toInt());

    QJsonObject tables = json.value(NODE_TABLES).toObject();
    for (auto &key: tables.keys()) {
        if(!tables.value(key).isObject())
            continue;

        TableModel *sch = new TableModel(tables.value(key).toObject(), key);
        append(sch);
    }
}

TableModel *DatabaseModel::tableByName(const QString &tableName) const
{
    for(int i = 0; i < size(); i++){
        TableModel *s = at(i);

        if(s->name() == tableName)
            return s;
    }

//    qWarning("Table with name '%s' not found in model",
//             qUtf8Printable(tableName));
    return nullptr;
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

    return nullptr;
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

//DatabaseModel DatabaseModel::operator +(const DatabaseModel &other)
//{
//    DatabaseModel model;
//    DatabaseModel::const_iterator i;

//    for (i = constBegin(); i != constEnd(); ++i)
//        model.append(*i);

//    for (i = other.constBegin(); i != other.constEnd(); ++i)
//        model.append(*i);

//    return model;
//}

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
        return nullptr;

    for (auto &rel: childTable->foreignKeys())
        if(rel->masterClassName == masterClassName)
            return rel;

    return nullptr;
}

RelationModel *DatabaseModel::relationByTableNames(const QString &masterTableName, const QString &childTableName)
{
    TableModel *childTable = tableByName(childTableName);

    if(!childTable)
        return nullptr;

    for (auto &rel: childTable->foreignKeys())
        if(rel->masterTable->name() == masterTableName)
            return rel;

    return nullptr;
}

DatabaseModel DatabaseModel::fromJson(QJsonObject &json)
{
    DatabaseModel model;

    model.setVersion(json.value(NODE_VERSION).toInt());

    QJsonObject tables = json.value(NODE_TABLES).toObject();
    for (auto &key: tables.keys()) {
        if(!json.value(key).isObject())
            continue;

        TableModel *sch = new TableModel(json.value(key).toObject(), key);
        model.append(sch);
    }
    return model;
}

int DatabaseModel::version() const
{
    return _version;
}

void DatabaseModel::setVersion(int version)
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
    for (auto &table: currentModel)
        for (auto &fk: table->foreignKeys())
            fk->masterTable = currentModel.tableByClassName(fk->masterClassName);
            */
}

DatabaseModel *DatabaseModel::modelByName(const QString &name)
{
    if (_models.contains(name))
        return _models[name];

    return Q_NULLPTR;
}

void DatabaseModel::deleteAllModels()
{
    QMapIterator<QString, DatabaseModel*> i(_models);
     while (i.hasNext()) {
         i.next();
//         cout << i.key() << ": " << i.value() << endl;
//         qDeleteAll(i.value());
     }
//    qDeleteAll(_models.values());
     _models.clear();
}

DatabaseModel operator +(const DatabaseModel &l, const DatabaseModel &r)
{
    DatabaseModel model;
    DatabaseModel::const_iterator i;

    for (i = r.constBegin(); i != r.constEnd(); ++i)
        model.append(*i);

    for (i = l.constBegin(); i != l.constEnd(); ++i)
        model.append(*i);

    return model;
}

DatabaseModel operator |(const DatabaseModel &l, const DatabaseModel &r)
{
    DatabaseModel ret;
    DatabaseModel::const_iterator i;
    QSet<QString> tables;

    for (i = r.constBegin(); i != r.constEnd(); ++i) {
        if (tables.contains((*i)->name()))
            continue;
        ret.append(*i);
        tables.insert((*i)->name());
    }

    for (i = l.constBegin(); i != l.constEnd(); ++i) {
        if (tables.contains((*i)->name()))
            continue;
        ret.append(*i);
        tables.insert((*i)->name());
    }

    return ret;
}

QT_END_NAMESPACE
