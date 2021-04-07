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
#include <QtCore/QSharedPointer>

#include <QtSql/QSqlQuery>

#include <QtNut/abstracttableset.h>
#include <QtNut/table.h>
#include <QtNut/bulkinserter.h>
#include <QtNut/databasemodel.h>
#include <QtNut/abstracttablesetdata.h>

NUT_BEGIN_NAMESPACE

template<class T>
class Query;

class BulkInserter;
class Database;

template<class T>
class TableSet : public AbstractTableSet
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

    Query<T> query();
    BulkInserter bulkInserter();
};

template<class T>
Q_OUTOFLINE_TEMPLATE TableSet<T>::TableSet(Database *parent) : AbstractTableSet(parent)
{
    data->childClassName = QString::fromUtf8(T::staticMetaObject.className());
}

template<class T>
Q_OUTOFLINE_TEMPLATE TableSet<T>::TableSet(Table *parent) : AbstractTableSet(parent)
{
    data->childClassName = QString::fromUtf8(T::staticMetaObject.className());
}

template<class T>
Q_OUTOFLINE_TEMPLATE Query<T> TableSet<T>::query()
{
    return Query<T>(data->database, this);
}

template<class T>
Q_OUTOFLINE_TEMPLATE BulkInserter TableSet<T>::bulkInserter()
{
    return BulkInserter(data->database, data->childClassName);
}

template<class T>
Q_OUTOFLINE_TEMPLATE int TableSet<T>::length() const
{
    return data->childs.count();
}

template<class T>
Q_OUTOFLINE_TEMPLATE Row<T> TableSet<T>::at(int i) const
{
#ifdef NUT_RAW_POINTER
    return reinterpret_cast<T*>(data->childs.at(i));
#else
    return data->childs.at(i).template objectCast<T>();
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
    Q_FOREACH (Row<T> i, t)
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
    Q_FOREACH (Row<T> i, t)
        remove(i);
}

NUT_END_NAMESPACE

#endif // TABLESET_H
