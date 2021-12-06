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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Nut API.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef BULKINSERTER_P_H
#define BULKINSERTER_P_H

#include <QtNut/phraselist.h>

QT_BEGIN_NAMESPACE

class Database;
class BulkInserterPrivate
{
public:
    BulkInserterPrivate(Database *db);

    Database *database;
    QString className;
    PhraseList fields;
    QList<QVariantList> variants;
    size_t fieldCount;
};

QT_END_NAMESPACE

#endif // BULKINSERTER_P_H
