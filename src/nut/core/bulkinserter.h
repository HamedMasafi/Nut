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

#ifndef BULKINSERTER_H
#define BULKINSERTER_H

#include <initializer_list>
#include <QtCore/QDebug>
#include <QtNut/phraselist.h>
#include <QtNut/fieldphrase.h>

#include <QtNut/nut_global.h>

QT_BEGIN_NAMESPACE

class PhraseList;
class Database;
class BulkInserterPrivate;
class NUT_EXPORT BulkInserter
{
    Q_DECLARE_PRIVATE(BulkInserter)

public:
    BulkInserter(Database *db, QString &className);
    BulkInserter(const BulkInserter &other);
    BulkInserter(BulkInserter &&other);

    void setFields(const PhraseList &ph);

    void insert(std::initializer_list<QVariant> vars);
    template<typename... Args>
    void insert(Args... args) {
        insert({args...});
    }
    int apply();

private:
    BulkInserterPrivate *d_ptr;
};

QT_END_NAMESPACE

#endif // BULKINSERTER_H
