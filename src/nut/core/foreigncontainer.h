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

#include <QtGlobal>
#include <QtNut/NutGlobal>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

template<class _OBJECT, typename _KEY>
class ForeignContainer
{
    _OBJECT *_object{nullptr};
    _KEY _key;
    enum StorageType {
        Key,
        ClassValue
    };
    StorageType storageType;

public:
    ForeignContainer()
    {}

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
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // FOREIGNCONTAINER_H
