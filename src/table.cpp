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

#include <QMetaMethod>
#include <QVariant>
#include "table.h"
#include "database.h"
#include "databasemodel.h"
#include "generators/sqlgeneratorbase_p.h"

NUT_BEGIN_NAMESPACE

/*
 * FIXME:
 *  Qt can not access metaObject inside of constructor
 *  so, if we can't initalize myModel inside of ctor. in
 *  other side myModel inited in propertyChanged signal, so
 *  any method that uses myModel (like: primaryKey, ...) can't
 *  be accessed before any property set. So ugly, but there are
 *  no other way for now.
 *
 *  This should be fixed to v1.2
 */

Table::Table(QObject *parent) : QObject(parent), myModel(nullptr), _parentTableSet(nullptr)
{
    setStatus(NewCreated);
}

void Table::add(TableSetBase *t)
{
    this->childTableSets.insert(t);
}


QString Table::primaryKey() const
{
    return myModel->primaryKey();
}

bool Table::isPrimaryKeyAutoIncrement() const
{
    return myModel->field(myModel->primaryKey())->isAutoIncrement;
}


QVariant Table::primaryValue() const
{
    return property(primaryKey().toLatin1().data());
}

void Table::propertyChanged(QString propName)
{
    if (!myModel)
         myModel = TableModel::findByClassName(metaObject()->className());

    if (!myModel)
        qFatal ("model for this class not found");

    if (propName == primaryKey())
        return;

    _changedProperties.insert(propName);
    if (_status == FeatchedFromDB)
        _status = Modified;

    if (_status == NewCreated)
        _status = Added;
}

void Table::clear()
{
    _changedProperties.clear();
}

QSet<QString> Table::changedProperties() const
{
    return _changedProperties;
}

bool Table::setParentTable(Table *master)
{
    QString masterClassName = master->metaObject()->className();

    foreach (RelationModel *r, myModel->foregionKeys())
        if(r->masterClassName == masterClassName)
        {
            setProperty(QString(r->localColumn).toLatin1().data(),
                        master->primaryValue());
            _changedProperties.insert(r->localColumn);
            return true;
        }

    return false;
}

TableSetBase *Table::parentTableSet() const
{
    return _parentTableSet;
}

void Table::setParentTableSet(TableSetBase *parent)
{
    _parentTableSet = parent;
    _parentTableSet->add(this);
}

TableSetBase *Table::childTableSet(const QString &name) const
{
    foreach (TableSetBase *t, childTableSets)
        if (t->childClassName() == name)
            return t;
    return nullptr;
}

int Table::save(Database *db)
{
    QSqlQuery q = db->exec(db->sqlGenertor()->saveRecord(this, db->tableName(metaObject()->className())));

    if(status() == Added && isPrimaryKeyAutoIncrement())
        setProperty(primaryKey().toLatin1().data(), q.lastInsertId());

    foreach(TableSetBase *ts, childTableSets)
        ts->save(db);
    setStatus(FeatchedFromDB);

    return q.numRowsAffected();
}

Table::Status Table::status() const
{
    return _status;
}

void Table::setStatus(const Status &status)
{
    _status = status;
}

NUT_END_NAMESPACE
