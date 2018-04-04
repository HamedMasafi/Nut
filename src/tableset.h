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

#ifndef TABLESET_H
#define TABLESET_H

#include <QtCore/qglobal.h>
#include <QtCore/QVariant>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtSql/QSqlQuery>

#include "tablesetbase_p.h"
#include "table.h"

NUT_BEGIN_NAMESPACE

template<class T>
class Query;

template<class T>
class NUT_EXPORT TableSet : public TableSetBase
{
public:
    explicit TableSet(Database *parent);
    explicit TableSet(Table *parent);

    void append(T *t);
    void append(QList<T *> t);
    void remove(T *t);
    void remove(QList<T *> t);

    inline T *type() const {}

    int length() const;
    T *at(int i) const;
    const T &operator[](int i) const;
    Query<T> *query();
    Query<T> *query(bool autoDelete);
};

template<class T>
Q_OUTOFLINE_TEMPLATE TableSet<T>::TableSet(Database *parent) : TableSetBase(parent)
{
    _childClassName = T::staticMetaObject.className();
}

template<class T>
Q_OUTOFLINE_TEMPLATE TableSet<T>::TableSet(Table *parent) : TableSetBase(parent)
{
    _childClassName = T::staticMetaObject.className();
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> *TableSet<T>::query()
{
    Query<T> *q = new Query<T>(_database, this, true);

    return q;
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> *TableSet<T>::query(bool autoDelete)
{
    Query<T> *q = new Query<T>(_database, this, autoDelete);

    return q;
}

template<class T>
Q_OUTOFLINE_TEMPLATE int TableSet<T>::length() const
{
    return _tables.count();
}

template<class T>
Q_OUTOFLINE_TEMPLATE T *TableSet<T >::at(int i) const
{
    return (T*)_tablesList.at(i);
}

template<class T>
Q_OUTOFLINE_TEMPLATE const T &TableSet<T>::operator[](int i) const
{
    return _tablesList[i];
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::append(T *t)
{
    _tables.insert(t);
    _tablesList.append(t);
//    rows.append(t);
    t->setParentTableSet(this);
    if(t->status() != Table::FeatchedFromDB)
        t->setStatus(Table::Added);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::append(QList<T *> t)
{
    foreach (T* i, t)
        append(i);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::remove(T *t)
{
    _tables.remove(t);
    t->setStatus(Table::Deleted);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::remove(QList<T *> t)
{
    foreach (T* i, t)
        remove(i);
}

NUT_END_NAMESPACE

#endif // TABLESET_H
