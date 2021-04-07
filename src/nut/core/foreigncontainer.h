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

#ifndef FOREIGNCONTAINER_H
#define FOREIGNCONTAINER_H

#include "table.h"
#include <QtNut/nut_global.h>

NUT_BEGIN_NAMESPACE

class AbstractForeignContainer
{
    Nut::Row<Table*> _t;
protected:
    AbstractForeignContainer(Table *parent, const QString &name) {
        parent->addForeignKey(name, this);
    }
    virtual void setObject(Table *table) { Q_UNUSED(table) }
};

template<class _OBJECT, typename _KEY>
class NUT_EXPORT ForeignContainer : public AbstractForeignContainer
{
    _OBJECT *_object{nullptr};
    _KEY _key;
    _KEY *_member;
    QString _name;
    enum StorageType {
        Key,
        ClassValue
    };
    StorageType storageType;

public:
    ForeignContainer(Table *parent, const QString &name, _KEY &keyMember)
        : AbstractForeignContainer(parent, name), _member(&keyMember), _name(name)
    {
    }

    ForeignContainer<_OBJECT, _KEY> operator =(const _KEY &key)
    {
        this->_key = key;
        this->_object = nullptr;
        this->storageType = Key;
        return *this;
    }

    ForeignContainer<_OBJECT, _KEY> operator =(const _OBJECT *value)
    {
        this->_object = value;
        this->storageType = ClassValue;
        return *this;
    }

    _KEY key()
    {
        if (this->storageType == Key)
            return _key;
        else
            return _object->primaryValue().template value<_KEY>();
    }

    _OBJECT *object()
    {
        return _object;
    }

    operator _KEY()
    {
        return key();
    }

    operator _OBJECT()
    {
        return object();
    }

    void setObject(Table *table) override
    {
        auto p = Nut::cast<_OBJECT>(table);
        if (!p)
            return;

        this->_object = p;
        this->storageType = ClassValue;
    }
};

NUT_END_NAMESPACE

#endif // FOREIGNCONTAINER_H
