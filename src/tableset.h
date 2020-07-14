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

#include <QtCore/QtGlobal>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QSharedPointer>

#include "tablesetbase_p.h"
#include "table.h"
#include "bulkinserter.h"
#include "databasemodel.h"
#include "tablesetbasedata.h"

NUT_BEGIN_NAMESPACE

template<class T>
class Query;

class BulkInserter;
class Database;

template<class T>
class NUT_EXPORT TableSet : public TableSetBase
{
public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;

    explicit TableSet(Database *parent);
    explicit TableSet(Table *parent);

    void append(Row<T> t);
    void append(RowList<T> t);
    void remove(Row<T> t);
    void remove(RowList<T> t);

    int length() const;
    Row<T> at(int i) const;
    Row<T> operator[](int i) const;

    Query<T> *query(bool autoDelete = true);
    BulkInserter *bulkInserter();
};

template<class T>
Q_OUTOFLINE_TEMPLATE TableSet<T>::TableSet(Database *parent) : TableSetBase(parent)
{
    data->childClassName = T::staticMetaObject.className();
}

template<class T>
Q_OUTOFLINE_TEMPLATE TableSet<T>::TableSet(Table *parent) : TableSetBase(parent)
{
    data->childClassName = T::staticMetaObject.className();
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> *TableSet<T>::query(bool autoDelete)
{
    Query<T> *q = new Query<T>(data->database, this, autoDelete);

    return q;
}

template<class T>
Q_OUTOFLINE_TEMPLATE BulkInserter *TableSet<T>::bulkInserter()
{
    BulkInserter *bi = new BulkInserter(data->database, data->childClassName);
    return bi;
}

template<class T>
Q_OUTOFLINE_TEMPLATE int TableSet<T>::length() const
{
    return data->childs.count();
}

template<class T>
Q_OUTOFLINE_TEMPLATE Row<T> TableSet<T>::at(int i) const
{
#ifdef NUT_SHARED_POINTER
    return data->childs.at(i).template objectCast<T>();
#else
    return reinterpret_cast<T*>(data->childs.at(i));
#endif
}

template<class T>
Q_OUTOFLINE_TEMPLATE Row<T> TableSet<T>::operator[](int i) const
{
    return at(i);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::append(Row<T> t)
{
    data.detach();
    data->childs.append(t);
//    data->tables.insert(t.data());
//    data->childRows.append(t.data());

//    if (_database)
//        t->setModel(_database->model().tableByClassName(t->metaObject()->className()));

    t->setParentTableSet(this);
    if(t->status() != Table::FetchedFromDB)
        t->setStatus(Table::Added);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::append(RowList<T> t)
{
    foreach (Row<T> i, t)
        append(i);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::remove(Row<T> t)
{
    data.detach();
//    data->childs.removeOne(t.data());
//    data->tables.remove(t.data());
    data->childs.removeOne(t);
    t->setStatus(Table::Deleted);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void TableSet<T>::remove(RowList<T> t)
{
    foreach (Row<T> i, t)
        remove(i);
}

NUT_END_NAMESPACE

#endif // TABLESET_H
