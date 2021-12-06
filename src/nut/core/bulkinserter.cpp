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

#include "bulkinserter.h"
#include "bulkinserter_p.h"

#include "phrases/phraselist.h"
#include "database.h"
#include "abstractsqlgenerator.h"
#include "databasemodel.h"

#include <QtCore/QDebug>

QT_BEGIN_NAMESPACE

BulkInserterPrivate::BulkInserterPrivate(Database *db)
    : database(db), fieldCount(0)
{

}

BulkInserter::BulkInserter(Database *db, QString &className)
    : d_ptr(new BulkInserterPrivate(db))
{
    Q_D(BulkInserter);

    for (auto &m: db->model())
        if (m->className() == className)
            d->className = m->name();
}

BulkInserter::BulkInserter(const BulkInserter &other)
{
    d_ptr = other.d_ptr;
}

BulkInserter::BulkInserter(BulkInserter &&other)
{
    d_ptr = other.d_ptr;
    other.d_ptr = nullptr;
}

void BulkInserter::setFields(const PhraseList &ph)
{
    Q_D(BulkInserter);
    d->fields = ph;
    d->fieldCount = static_cast<size_t>(ph.data.count());
}

void BulkInserter::insert(std::initializer_list<QVariant> vars)
{
    Q_D(BulkInserter);

    if (vars.size() != d->fieldCount) {
        qInfo("Number of rows mistake");
        return;
    }

    QVariantList list;
    std::initializer_list<QVariant>::iterator it;
    for (it = vars.begin(); it != vars.end(); ++it)
        list.append(*it);
    d->variants.append(list);
}

int BulkInserter::apply()
{
    Q_D(BulkInserter);
    auto sql = d->database->sqlGenerator()->insertBulk(d->className,
                                                       d->fields,
                                                       d->variants);
    QSqlQuery q = d->database->exec(sql);
    return q.numRowsAffected();
}

QT_END_NAMESPACE
