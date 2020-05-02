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

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMetaClassInfo>

#ifdef NUT_COMPILE_STATIC
#   define NUT_EXPORT
#else
#   define NUT_EXPORT Q_DECL_EXPORT
#endif

#define NUT_INFO(type, name, value)                                            \
    Q_CLASSINFO(__nut_NAME_PERFIX type #name #value,                           \
                type "\n" #name "\n" #value)

#define NUT_INFO_STRING(type, name, value)                                     \
    Q_CLASSINFO(__nut_NAME_PERFIX type #name #value,                           \
                type "\n" #name "\n" value)

#define NUT_FIELD_PERFIX
#define NUT_FIELD_POSTFIX Field

// Database
#define NUT_DB_VERSION(version)  \
    NUT_INFO(__nut_DB_VERSION, version, 0)

#define NUT_DECLARE_TABLE(type, name)                                          \
    NUT_INFO(__nut_TABLE, type, name)                                          \
    Q_PROPERTY(NUT_WRAP_NAMESPACE(TableSet<type>) name READ name)              \
    NUT_WRAP_NAMESPACE(TableSet<type>) *m_##name;                              \
    public:                                                                    \
    static const type *_##name;                                                \
    NUT_WRAP_NAMESPACE(TableSet<type>) *name() const                           \
    { return m_##name; }                                                       \
    private:

//Table
#define NUT_DECLARE_FIELD(type, name, read, write)                             \
    Q_PROPERTY(type name READ read WRITE write)                                \
    NUT_INFO(__nut_FIELD, name, 0)                                             \
    type m_##name;                                                             \
public:                                                                        \
    static NUT_WRAP_NAMESPACE(FieldPhrase<type>)& name ## Field(){             \
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

#define NUT_FOREIGN_KEY_DECLARE(type, keytype, name, read, write)                     \
    NUT_INFO(__nut_FIELD, name##Id, 0)                                             \
    NUT_INFO(__nut_FOREIGN_KEY, name, type)                                   \
    Nut::Row<type> m_##name; \
    keytype m_##name##Id; \
    Q_PROPERTY(Nut::Row<type> name READ read WRITE write)                                \
    Q_PROPERTY(keytype name##Id READ read##Id WRITE write##Id)                                \
    Q_PROPERTY(Nut::Row<Table> _##name READ _##read WRITE _##write)                \
public:                                                                        \
    Nut::Row<type> read() const;                          \
    keytype read##Id() const;                                                   \
    static NUT_WRAP_NAMESPACE(FieldPhrase<keytype>)& name##Id ## Field(){             \
        static NUT_WRAP_NAMESPACE(FieldPhrase<keytype>) f =                       \
                NUT_WRAP_NAMESPACE(FieldPhrase<keytype>)                          \
                        (staticMetaObject.className(), #name);                 \
        return f;                                                              \
    }                                                                          \
    void write(Nut::Row<type> name); \
    void write##Id(keytype name##Id); \
private: \
    Nut::Row<Table> _##read() const;                                               \
    void _##write(Nut::Row<Table> name);

#define NUT_FOREIGN_KEY_IMPLEMENT(class, type, keytype, name, read, write)                     \
    \
    Nut::Row<type> class::read() const { return m_##name ; }                          \
    void class::write(Nut::Row<type> name){                                           \
        propertyChanged(QT_STRINGIFY2(name##Id));                                                \
        m_##name = name;                                                       \
        m_##name##Id = name->primaryValue().value<keytype>(); \
    } \
    \
    keytype class::read##Id() const{                                                         \
        if (m_##name) \
            return m_##name->primaryValue().value<keytype>(); \
        return m_##name##Id;                                                       \
    }                                                                          \
    void class::write##Id(keytype name##Id){                                                     \
        propertyChanged(QT_STRINGIFY2(name##Id));                                                \
        m_##name##Id = name##Id;                                                       \
        m_##name = nullptr; \
        propertyChanged(QT_STRINGIFY2(name##Id));                                                \
    } \
    Nut::Row<Table> class::_##read() const { return m_##name ; }                           \
    \
    void class::_##write(Nut::Row<Table> name) {                                           \
        propertyChanged(QT_STRINGIFY2(keyname));                                                \
        m_##name = qSharedPointerCast< type >( name );\
        m_##name##Id = m_##name->primaryValue().value<keytype>(); \
    }


#define NUT_DECLARE_CHILD_TABLE(type, n)                                       \
    private:                                                                   \
        NUT_WRAP_NAMESPACE(TableSet)<type> *m_##n;                             \
    public:                                                                    \
        static type *n##Table();                                               \
        NUT_WRAP_NAMESPACE(TableSet)<type> *n();

#define NUT_IMPLEMENT_CHILD_TABLE(class, type, n)                              \
    type *class::n##Table(){                                                   \
        static auto f = new type();                                           \
        return f;                                                              \
    }                                                                          \
    NUT_WRAP_NAMESPACE(TableSet)<type> *class::n(){                            \
        return m_##n;                                                          \
    }

#define NUT_FIELD(name)                     NUT_INFO(__nut_FIELD, name, 0)
#define NUT_PRIMARY_KEY(x)                  NUT_INFO(__nut_PRIMARY_KEY,  x, 0)  \
    public:                                                                     \
    QVariant primaryValue() const override {                                    \
        return property(#x);                                                    \
    }                                                                           \
    void setPrimaryValue(const QVariant &value) override {                      \
        setProperty(#x, value);                                                 \
    }                                                                           \
    private:


#define NUT_AUTO_INCREMENT(x)               NUT_INFO(__nut_AUTO_INCREMENT, x, 0)
#define NUT_PRIMARY_AUTO_INCREMENT(x)       NUT_INFO(__nut_PRIMARY_KEY_AI, x, 0)\
            NUT_PRIMARY_KEY(X) NUT_AUTO_INCREMENT(X)
#define NUT_DISPLAY_NAME(field, name)       NUT_INFO(__nut_DISPLAY, field, name)
#define NUT_UNIQUE(x)                       NUT_INFO(__nut_UNIQUE, x, 0)
#define NUT_LEN(field, len)                 NUT_INFO(__nut_LEN, field, len)
#define NUT_DEFAULT_VALUE(x, n)             NUT_INFO(__nut_DEFAULT_VALUE, x, n)
#define NUT_NOT_NULL(x)                     NUT_INFO(__nut_NOT_NULL, x, 1)
#define NUT_INDEX(name, field, order)

NUT_BEGIN_NAMESPACE

inline bool nutClassInfo(const QMetaClassInfo &classInfo,
                         QString &type, QString &name, QVariant &value)
{
    if (!QString(classInfo.name()).startsWith(__nut_NAME_PERFIX))
        return false;

    QStringList parts = QString(classInfo.value()).split("\n");
    if (parts.count() != 3)
        return false;

    type = parts[0];
    name = parts[1];
    value = qVariantFromValue(parts[2]);
    return true;
}

inline bool nutClassInfoString(const QMetaClassInfo &classInfo,
                              QString &type, QString &name, QString &value)
{
    if (!QString(classInfo.name()).startsWith(__nut_NAME_PERFIX))
        return false;

    QStringList parts = QString(classInfo.value()).split("\n");
    if (parts.count() != 3)
        return false;

    type = parts[0];
    name = parts[1];
    value = parts[2];
    return true;
}

inline bool nutClassInfoBool(const QMetaClassInfo &classInfo,
                              QString &type, QString &name, bool &value)
{
    if (!QString(classInfo.name()).startsWith(__nut_NAME_PERFIX))
        return false;

    QStringList parts = QString(classInfo.value()).split("\n");
    if (parts.count() != 3)
        return false;

    QString buffer = parts[2].toLower();
    if (buffer != "true" && buffer != "false")
        return false;

    type = parts[0];
    name = parts[1];
    value = (buffer == "true");
    return true;
}

inline bool nutClassInfoInt(const QMetaClassInfo &classInfo,
                            QString &type, QString &name, bool &value)
{
    if (!QString(classInfo.name()).startsWith(__nut_NAME_PERFIX))
        return false;

    QStringList parts = QString(classInfo.value()).split("\n");
    if (parts.count() != 3)
        return false;
    bool ok;
    type = parts[0];
    name = parts[1];
    value = parts[2].toInt(&ok);
    return ok;
}

#ifdef NUT_SHARED_POINTER
template <class T>
using RowList = QList<QSharedPointer<T>>;

template <class T>
using RowSet = QSet<QSharedPointer<T>>;

template <typename T>
using Row = QSharedPointer<T>;

template<class T>
inline Row<T> create() {
    return QSharedPointer<T>(new T);
}

template<class T>
inline Row<T> create(QObject *parent) {
    return QSharedPointer<T>(new T(parent));
}

template<class T>
inline Row<T> createFrom(T *row) {
    return QSharedPointer<T>(row);
}
template<class T>
inline Row<T> createFrom(const QSharedPointer<T> row) {
    return row;
}

#else
template <typename T>
using RowList = QList<T*>;

template <typename T>
using RowSet = QSet<T*>;

template <typename T>
using Row = T*;

template<class T>
inline Row<T> create() {
    return new T;
}

template<class T>
inline T *get(const Row<T> row) {
    return row;
}

template<class T>
inline T *get(const QSharedPointer<T> row) {
    return row.data();
}

#endif

NUT_END_NAMESPACE

#endif // SYNTAX_DEFINES_H
