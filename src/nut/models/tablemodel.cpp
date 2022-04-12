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
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

#include "tablemodel.h"
#include "nut_global.h"
#include "nut_p.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

#define REL_LOCAL_COLUMN        QStringLiteral("localColumn")
#define REL_LOCAL_PROPERTY      QStringLiteral("localProperty")
#define REL_MASTER_CLASS_NAME   QStringLiteral("masterClassName")
#define REL_FOREIGIN_COLUMN     QStringLiteral("foreignColumn")

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
    for (auto &f: _fields)
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
    for (auto &f: _fields)
        ret.append(f->name);
    return ret;
}

bool TableModel::operator ==(const TableModel &t) const{
    if(_name != t.name())
        return false;

    if(fields().count() != t.fields().count())
        return false;

    for (auto &f: _fields) {
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

    const QMetaObject *tableMetaObject = QMetaType(typeId).metaObject();

    _typeId = typeId;
    _name = tableName;
    _className = QString::fromUtf8(tableMetaObject->className());

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

        if(type == QStringLiteral(__nut_FIELD)) {
            auto *f = new FieldModel;
            f->name = f->displayName = name;
            _fields.append(f);
        }
    }
    // Browse all fields
    for(int j = 1; j < tableMetaObject->propertyCount(); j++){
        QMetaProperty fieldProperty = tableMetaObject->property(j);
        auto name = QString::fromUtf8(fieldProperty.name());
        FieldModel *fieldObj = field(name);
        for (auto &f: _fields)
            if(f->name == name)
                fieldObj = f;
        if(!fieldObj)
            continue;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        fieldObj->type = static_cast<QMetaType::Type>(fieldProperty.metaType().id());
#else
        fieldObj->type = static_cast<QMetaType::Type>(fieldProperty.type());
#endif
        fieldObj->typeName = QString::fromUtf8(fieldProperty.typeName());
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

        if(type == QStringLiteral(__nut_FOREIGN_KEY)) {
            auto *fk = new RelationModel;
            fk->slaveTable = this;
            fk->localColumn = name + QStringLiteral("Id");
            fk->localProperty = name;
            fk->foreignColumn = value;
            fk->masterClassName = value;
            _foreignKeys.append(fk);
        }

        if(type == QStringLiteral(__nut_FIELD)) {

        }


        FieldModel *f = field(name);
        if (!f)
            continue;

        if (type == QStringLiteral(__nut_LEN))
            f->length = value.toInt();
        else if (type == QStringLiteral(__nut_NOT_NULL))
            f->notNull = true;
        else if (type == QStringLiteral(__nut_DEFAULT_VALUE))
            f->defaultValue = value;
        else if (type == QStringLiteral(__nut_PRIMARY_KEY))
            f->isPrimaryKey = true;
        else if (type == QStringLiteral(__nut_AUTO_INCREMENT))
            f->isAutoIncrement = true;
        else if (type == QStringLiteral(__nut_UNIQUE))
            f->isUnique = true;
        else if (type == QStringLiteral(__nut_DISPLAY))
            f->displayName = value.mid(1, value.length() - 2);
        else if (type == QStringLiteral(__nut_PRIMARY_KEY_AI)) {
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

    QJsonObject fields = json.value(QStringLiteral(__FIELDS)).toObject();
    QJsonObject relations = json.value(QStringLiteral(__FOREIGN_KEYS)).toObject();
    for (auto &key: fields.keys()) {
        QJsonObject fieldObject = fields.value(key).toObject();
        //TODO: use FieldModel(QJsonObject) ctor
        auto *f = new FieldModel;
        f->name = fieldObject.value(QStringLiteral(__NAME)).toString();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        f->type = static_cast<QMetaType::Type>(QMetaType::fromName(fieldObject.value(QStringLiteral(__TYPE)).toString().toLatin1()).id());
#else
        f->type = static_cast<QMetaType::Type>(QMetaType::type(fieldObject.value(QStringLiteral(__TYPE)).toString().toLatin1().data()));
#endif
        f->typeName = QString::fromUtf8(METATYPE_TO_NAME(f->type));

        if(fieldObject.contains(QStringLiteral(__nut_NOT_NULL)))
            f->notNull = fieldObject.value(QStringLiteral(__nut_NOT_NULL)).toBool();

        if(fieldObject.contains(QStringLiteral(__nut_UNIQUE)))
            f->isUnique = fieldObject.value(QStringLiteral(__nut_UNIQUE)).toBool();

        if(fieldObject.contains(QStringLiteral(__nut_LEN)))
            f->length = fieldObject.value(QStringLiteral(__nut_LEN)).toInt();

        if(fieldObject.contains(QStringLiteral(__nut_DEFAULT_VALUE)))
            f->defaultValue = fieldObject.value(QStringLiteral(__nut_DEFAULT_VALUE)).toString();
        _fields.append(f);
    }

    for (auto &key: relations.keys()) {
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

    for (auto &f: _fields) {
        QJsonObject fieldObj;
        fieldObj.insert(QStringLiteral(__NAME), f->name);
        fieldObj.insert(QStringLiteral(__TYPE), QString::fromUtf8(METATYPE_TO_NAME(f->type)));

        if(f->length)
            fieldObj.insert(QStringLiteral(__nut_LEN), f->length);

        if(f->notNull)
            fieldObj.insert(QStringLiteral(__nut_NOT_NULL), f->notNull);

        if(f->isUnique)
            fieldObj.insert(QStringLiteral(__nut_UNIQUE), f->isUnique);

        if(!f->defaultValue.isNull())
            fieldObj.insert(QStringLiteral(__nut_DEFAULT_VALUE), f->defaultValue);

        if(f->isAutoIncrement)
            obj.insert(QStringLiteral(__nut_AUTO_INCREMENT), f->name);

        if(f->isPrimaryKey)
            obj.insert(QStringLiteral(__nut_PRIMARY_KEY), f->name);

        fieldsObj.insert(f->name, fieldObj);
    }
    for (auto &rel: _foreignKeys)
        foreignKeysObj.insert(rel->localColumn, rel->toJson());

    obj.insert(QStringLiteral(__FIELDS), fieldsObj);
    obj.insert(QStringLiteral(__FOREIGN_KEYS), foreignKeysObj);

    return obj;
}

RelationModel *TableModel::foreignKey(const QString &otherTable) const
{
    for (auto &fk: _foreignKeys)
        if(fk->masterClassName == otherTable)
            return fk;

    return nullptr;
}

RelationModel *TableModel::foreignKeyByField(const QString &fieldName) const
{
    for (auto &fk: _foreignKeys)
        if(fk->localColumn == fieldName)
            return fk;

    return nullptr;
}

QString TableModel::toString() const
{
    QStringList sl;
    for (auto &f: _fields)
        sl.append(f->name + QStringLiteral(" ")
                  + QString::fromUtf8(METATYPE_TO_NAME(f->type)));

    QString ret = QStringLiteral("%1 (%2)")
                      .arg(_name, sl.join(QStringLiteral(", ")));
    return ret;
}

QString TableModel::primaryKey() const
{
    for (auto &f: _fields)
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
    name = json.value(QStringLiteral(__NAME)).toString();
    type = static_cast<QMetaType::Type>(json.value(QStringLiteral(__TYPE)).toInt());
    length = json.value(QStringLiteral(__nut_LEN)).toInt();
    notNull = json.value(QStringLiteral(__nut_NOT_NULL)).toBool();
    isUnique = json.value(QStringLiteral(__nut_UNIQUE)).toBool();
    isAutoIncrement = json.value(QStringLiteral(__nut_AUTO_INCREMENT)).toBool();
    isPrimaryKey = json.value(QStringLiteral(__nut_PRIMARY_KEY)).toBool();
    defaultValue = json.value(QStringLiteral(__nut_DEFAULT_VALUE)).toString();
    isUnique = json.value(QStringLiteral(__nut_UNIQUE)).toBool();
}

QJsonObject FieldModel::toJson() const
{
    QJsonObject fieldObj;
    fieldObj.insert(QStringLiteral(__NAME), name);
    fieldObj.insert(QStringLiteral(__TYPE), QString::fromUtf8(METATYPE_TO_NAME(type)));
    fieldObj.insert(QStringLiteral(__nut_LEN), length);
    fieldObj.insert(QStringLiteral(__nut_NOT_NULL), notNull);
    fieldObj.insert(QStringLiteral(__nut_UNIQUE), isUnique);
    fieldObj.insert(QStringLiteral(__nut_AUTO_INCREMENT), isAutoIncrement);
    fieldObj.insert(QStringLiteral(__nut_PRIMARY_KEY), isPrimaryKey);
    fieldObj.insert(QStringLiteral(__nut_DEFAULT_VALUE), defaultValue);
    return fieldObj;
}

RelationModel::RelationModel(const QJsonObject &obj)
{
    localColumn = obj.value(REL_LOCAL_COLUMN).toString();
    localProperty = obj.value(REL_LOCAL_PROPERTY).toString();
    masterClassName = obj.value(REL_MASTER_CLASS_NAME).toString();
    foreignColumn = obj.value(REL_FOREIGIN_COLUMN).toString();
    slaveTable = masterTable = nullptr;
}

QJsonObject RelationModel::toJson() const
{
    QJsonObject o;
    o.insert(REL_LOCAL_COLUMN, localColumn);
    o.insert(REL_LOCAL_PROPERTY, localProperty);
    o.insert(REL_MASTER_CLASS_NAME, masterClassName);
    o.insert(REL_FOREIGIN_COLUMN, foreignColumn);
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

QT_END_NAMESPACE
