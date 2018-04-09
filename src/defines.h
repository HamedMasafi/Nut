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

#ifndef SYNTAX_DEFINES_H
#define SYNTAX_DEFINES_H

#define NUT_NAMESPACE Nut

#include "defines_p.h"
#include "qglobal.h"

#ifdef NUT_COMPILE_STATIC
#   define NUT_EXPORT
#else
#   define NUT_EXPORT Q_DECL_EXPORT
#endif

#define NUT_INFO(type, name, value)                                            \
    Q_CLASSINFO(__nut_NAME_PERFIX type #name #value,                           \
                type "\n" #name "\n" #value)

// Database
#define NUT_DB_VERSION(version)  \
    NUT_INFO(__nut_DB_VERSION, version, 0)

#define NUT_DECLARE_TABLE(type, name)                                          \
    NUT_INFO(__nut_TABLE, type, name)                                          \
    Q_PROPERTY(type* name READ name)                                           \
    Q_PROPERTY(NUT_WRAP_NAMESPACE(TableSet<type>) name##Table READ name##Table)\
    type* m_##name;                                                            \
    NUT_WRAP_NAMESPACE(TableSet<type>) *m_##name##Table;                       \
public:                                                                        \
    static const type _##name;                                                 \
    type* name() const{ return m_##name; }                                     \
    NUT_WRAP_NAMESPACE(TableSet<type>) *name##Table() const                    \
            { return m_##name##Table; }

//Table
#define NUT_DECLARE_FIELD(type, name, read, write)                             \
    Q_PROPERTY(type name READ read WRITE write)                                \
    NUT_INFO(__nut_FIELD, name, 0)                                             \
    type m_##name;                                                             \
public:                                                                        \
    static NUT_WRAP_NAMESPACE(FieldPhrase<type>) name ## Field(){              \
        static NUT_WRAP_NAMESPACE(FieldPhrase<type>) f =                       \
                NUT_WRAP_NAMESPACE(FieldPhrase<type>)                          \
                        (staticMetaObject.className(), #name);                 \
        return f;                                                              \
    }                                                                          \
    type read() const{                                                         \
        return m_##name;                                                       \
    }                                                                          \
    void write(type name){                                                     \
        m_##name = name;                                                       \
        propertyChanged(#name);                                                \
    }

#define NUT_FOREGION_KEY(type, keytype, name, read, write)                     \
    Q_PROPERTY(type* name READ read WRITE write)                               \
    NUT_DECLARE_FIELD(keytype, id##name, id##read, id##write)                  \
    NUT_INFO(__nut_FOREGION_KEY, name, type)                                   \
    type *m_##name;                                                            \
public:                                                                        \
    type *read() const { return m_##name ; }                                   \
    void write(type *name){                                                    \
        m_##name = name;                                                       \
    }

#define NUT_DECLARE_CHILD_TABLE(type, n)                                       \
    private:                                                                   \
        NUT_WRAP_NAMESPACE(TableSet)<type> *m_##n;                             \
    public:                                                                    \
        static type *n##Table();                                               \
        NUT_WRAP_NAMESPACE(TableSet)<type> *n();

#define NUT_IMPLEMENT_CHILD_TABLE(class, type, n)                              \
    type *class::n##Table(){                                                   \
        static type *f = new type();                                           \
        return f;                                                              \
    }                                                                          \
    NUT_WRAP_NAMESPACE(TableSet)<type> *class::n(){                            \
        return m_##n;                                                          \
    }


#define NUT_PRIMARY_KEY(x)                  NUT_INFO(__nut_PRIMARY_KEY,  x, 0)
#define NUT_AUTO_INCREMENT(x)               NUT_INFO(__nut_AUTO_INCREMENT, x, 0)
#define NUT_PRIMARY_AUTO_INCREMENT(x)           NUT_PRIMARY_KEY(x)             \
                                                NUT_AUTO_INCREMENT(x)
#define NUT_DISPLAY_NAME(field, name)       NUT_INFO(__nut_DISPLAY, field, name)
#define NUT_UNIQUE(x)                       NUT_INFO(__nut_UNIQUE, x, 0)
#define NUT_LEN(field, len)                 NUT_INFO(__nut_LEN, field, len)
#define NUT_DEFAULT_VALUE(x, n)             NUT_INFO(__nut_DEFAULT_VALUE, x, n)
#define NUT_NOT_NULL(x)                     NUT_INFO(__nut_NOT_NULL, x, 1)
#define NUT_INDEX(name, field, order)

#endif // SYNTAX_DEFINES_H
