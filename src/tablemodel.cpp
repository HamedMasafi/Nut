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

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>

#include <QJsonArray>
#include <QJsonObject>

#include "tablemodel.h"
#include "defines.h"

NUT_BEGIN_NAMESPACE

QString TableModel::name() const
{
    return _name;
}

void TableModel::setName(const QString &name)
{
    _name = name;
}

QString TableModel::className() const
{
    return _className;
}

void TableModel::setClassName(const QString &className)
{
    _className = className;
}

int TableModel::typeId() const
{
    return _typeId;
}

void TableModel::setTypeId(const int &typeId)
{
    _typeId = typeId;
}

FieldModel *TableModel::field(int n) const
{
    if (n < 0 || n >= _fields.count())
        return nullptr;

    return _fields.at(n);
}

FieldModel *TableModel::field(const QString &name) const
{
    foreach (FieldModel *f, _fields)
        if(f->name == name)
            return f;

    return nullptr;
}

QList<FieldModel *> TableModel::fields() const
{
    return _fields;
}

QList<RelationModel *> TableModel::foreignKeys() const
{
    return _foreignKeys;
}

QStringList TableModel::fieldsNames() const
{
    QStringList ret;
    foreach (FieldModel *f, _fields)
        ret.append(f->name);
    return ret;
}

bool TableModel::operator ==(const TableModel &t) const{
    if(_name != t.name())
        return false;

    if(fields().count() != t.fields().count())
        return false;

    foreach (FieldModel *f, _fields) {
        FieldModel *tf = t.field(f->name);
        if(!tf)
            return false;

        if(*f != *tf)
            return false;
    }

    return true;
}

bool TableModel::operator !=(const TableModel &t) const
{
    return !(*this == t);
}

TableModel::TableModel(int typeId, const QString &tableName)
{
    //TODO: check that
//    if  (findByTypeId(typeId))
//        return;

    const QMetaObject *tableMetaObject = QMetaType::metaObjectForType(typeId);

    _typeId = typeId;
    _name = tableName;
    _className = tableMetaObject->className();

//#ifdef NUT_NAMESPACE
//    if(_className.startsWith(QT_STRINGIFY(NUT_NAMESPACE) "::"))
//        _className = _className.replace(QT_STRINGIFY(NUT_NAMESPACE) "::", "");
//#endif

    // get fields names
    for(int j = 0; j < tableMetaObject->classInfoCount(); j++){
        QString type;
        QString name;
        QString value;

        if (!nutClassInfoString(tableMetaObject->classInfo(j),
                            type, name, value)) {
            continue;
        }

        if(type == __nut_FIELD){
            auto *f = new FieldModel;
            f->name = f->displayName = name;
            _fields.append(f);
        }
    }
    // Browse all fields
    for(int j = 1; j < tableMetaObject->propertyCount(); j++){
        QMetaProperty fieldProperty = tableMetaObject->property(j);

        FieldModel *fieldObj = field(fieldProperty.name());
        foreach (FieldModel *f, _fields)
            if(f->name == fieldProperty.name())
                f = fieldObj;
        if(!fieldObj)
            continue;
        fieldObj->type = static_cast<QMetaType::Type>(fieldProperty.type());
        fieldObj->typeName = QString(fieldProperty.typeName());
    }

    // Browse class infos
    for(int j = 0; j < tableMetaObject->classInfoCount(); j++){
        QString type;
        QString name;
        QString value;

        if (!nutClassInfoString(tableMetaObject->classInfo(j),
                            type, name, value)) {
            continue;
        }

        if(type == __nut_FOREIGN_KEY){
            auto *fk = new RelationModel;
            fk->slaveTable = this;
            fk->localColumn = name + "Id";
            fk->localProperty = name;
            fk->foreignColumn = value;
            fk->masterClassName = value;
            _foreignKeys.append(fk);
        }

        if(type == __nut_FIELD){

        }


        FieldModel *f = field(name);
        if (!f)
            continue;

        if (type == __nut_LEN)
            f->length = value.toInt();
        else if (type == __nut_NOT_NULL)
            f->notNull = true;
        else if (type == __nut_DEFAULT_VALUE)
            f->defaultValue = value;
        else if (type == __nut_PRIMARY_KEY)
            f->isPrimaryKey = true;
        else if (type == __nut_AUTO_INCREMENT)
            f->isAutoIncrement = true;
        else if (type == __nut_UNIQUE)
            f->isUnique = true;
        else if (type == __nut_DISPLAY)
            f->displayName = value.mid(1, value.length() - 2);
        else if (type == __nut_PRIMARY_KEY_AI) {
            f->isPrimaryKey = true;
            f->isAutoIncrement = true;
        }
    }
}

/*
 "comment": {
        "auto_increment": "id",
        "fields": {
            "id": {
                "name": "id",
                "type": "int"
            },
            "userId": {
                "name": "userId",
                "type": "int"
            }
        },
        "primary_key": "id"
    },
*/
TableModel::TableModel(const QJsonObject &json, const QString &tableName) : _typeId(0)
{
    _name = tableName;

    QJsonObject fields = json.value(__FIELDS).toObject();
    QJsonObject relations = json.value(__FOREIGN_KEYS).toObject();
    foreach (QString key, fields.keys()) {
        QJsonObject fieldObject = fields.value(key).toObject();
        //TODO: use FieldModel(QJsonObject) ctor
        auto *f = new FieldModel;
        f->name = fieldObject.value(__NAME).toString();
        f->type = static_cast<QMetaType::Type>(QMetaType::type(fieldObject.value(__TYPE).toString().toLatin1().data()));
        f->typeName = QMetaType::typeName(f->type);

        if(fieldObject.contains(__nut_NOT_NULL))
            f->notNull = fieldObject.value(__nut_NOT_NULL).toBool();

        if(fieldObject.contains(__nut_UNIQUE))
            f->isUnique = fieldObject.value(__nut_UNIQUE).toBool();

        if(fieldObject.contains(__nut_LEN))
            f->length = fieldObject.value(__nut_LEN).toInt();

        if(fieldObject.contains(__nut_DEFAULT_VALUE))
            f->defaultValue = fieldObject.value(__nut_DEFAULT_VALUE).toString();
        _fields.append(f);
    }

    foreach (QString key, relations.keys()) {
        QJsonObject relObject = fields.value(key).toObject();
        _foreignKeys.append(new RelationModel(relObject));
    }
}

TableModel::~TableModel()
{
    qDeleteAll(_fields);
    qDeleteAll(_foreignKeys);
}

QJsonObject TableModel::toJson() const
{
    QJsonObject obj;
    QJsonObject fieldsObj;
    QJsonObject foreignKeysObj;

    foreach (FieldModel *f, _fields) {
        QJsonObject fieldObj;
        fieldObj.insert(__NAME, f->name);
        fieldObj.insert(__TYPE, QString(QVariant::typeToName(f->type)));

        if(f->length)
            fieldObj.insert(__nut_LEN, f->length);

        if(f->notNull)
            fieldObj.insert(__nut_NOT_NULL, f->notNull);

        if(f->isUnique)
            fieldObj.insert(__nut_UNIQUE, f->isUnique);

        if(!f->defaultValue.isNull())
            fieldObj.insert(__nut_DEFAULT_VALUE, f->defaultValue);

        if(f->isAutoIncrement)
            obj.insert(__nut_AUTO_INCREMENT, f->name);

        if(f->isPrimaryKey)
            obj.insert(__nut_PRIMARY_KEY, f->name);

        fieldsObj.insert(f->name, fieldObj);
    }
    foreach (RelationModel *rel, _foreignKeys)
        foreignKeysObj.insert(rel->localColumn, rel->toJson());

    obj.insert(__FIELDS, fieldsObj);
    obj.insert(__FOREIGN_KEYS, foreignKeysObj);

    return obj;
}

RelationModel *TableModel::foreignKey(const QString &otherTable) const
{
    foreach (RelationModel *fk, _foreignKeys)
        if(fk->masterClassName == otherTable)
            return fk;

    return nullptr;
}

RelationModel *TableModel::foreignKeyByField(const QString &fieldName) const
{
    foreach (RelationModel *fk, _foreignKeys)
        if(fk->localColumn == fieldName)
            return fk;

    return nullptr;
}

QString TableModel::toString() const
{
    QStringList sl;
    foreach (FieldModel *f, _fields)
        sl.append(f->name + " " + QVariant::typeToName(f->type));

    QString ret = QString("%1 (%2)")
            .arg(_name, sl.join(", "));
    return ret;
}

QString TableModel::primaryKey() const
{
    foreach (FieldModel *f, _fields)
        if(f->isPrimaryKey)
            return f->name;
    return QString();
}

bool TableModel::isPrimaryKeyAutoIncrement() const
{
    FieldModel *pk = field(primaryKey());
    if (!pk)
        return false;
    return pk->isAutoIncrement;
}

FieldModel::FieldModel(const QJsonObject &json)
{
    name = json.value(__NAME).toString();
    type = static_cast<QMetaType::Type>(json.value(__TYPE).toInt());
    length = json.value(__nut_LEN).toInt();
    notNull = json.value(__nut_NOT_NULL).toBool();
    isUnique = json.value(__nut_UNIQUE).toBool();
    isAutoIncrement = json.value(__nut_AUTO_INCREMENT).toBool();
    isPrimaryKey = json.value(__nut_PRIMARY_KEY).toBool();
    defaultValue = json.value(__nut_DEFAULT_VALUE).toString();
    isUnique = json.value(__nut_UNIQUE).toBool();
}

QJsonObject FieldModel::toJson() const
{
    QJsonObject fieldObj;
    fieldObj.insert(__NAME, name);
    fieldObj.insert(__TYPE, QString(QVariant::typeToName(type)));
    fieldObj.insert(__nut_LEN, length);
    fieldObj.insert(__nut_NOT_NULL, notNull);
    fieldObj.insert(__nut_UNIQUE, isUnique);
    fieldObj.insert(__nut_AUTO_INCREMENT, isAutoIncrement);
    fieldObj.insert(__nut_PRIMARY_KEY, isPrimaryKey);
    fieldObj.insert(__nut_DEFAULT_VALUE, defaultValue);
    return fieldObj;
}

RelationModel::RelationModel(const QJsonObject &obj)
{
    localColumn = obj.value("localColumn").toString();
    localProperty = obj.value("localProperty").toString();
    masterClassName = obj.value("masterClassName").toString();
    foreignColumn = obj.value("foreignColumn").toString();
    slaveTable = masterTable = nullptr;
}

QJsonObject RelationModel::toJson() const
{
    QJsonObject o;
    o.insert("localColumn", localColumn);
    o.insert("localProperty", localProperty);
    o.insert("masterClassName", masterClassName);
    o.insert("foreignColumn", foreignColumn);
    return o;
}

bool operator ==(const RelationModel &l, const RelationModel &r)
{
    return r.foreignColumn == l.foreignColumn
            && r.localColumn == l.localColumn
            && r.localProperty == l.localProperty
            && r.masterClassName == l.masterClassName;
}

bool operator !=(const RelationModel &l, const RelationModel &r)
{
    return !(l == r);
}

NUT_END_NAMESPACE
