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

#ifndef PHRASELIST_H
#define PHRASELIST_H

#include <QtNut/nut_global.h>
#include <QtNut/phrasedatalist.h>

QT_BEGIN_NAMESPACE

class AbstractFieldPhrase;
class NUT_EXPORT PhraseList
{
public:
    bool isValid;
    PhraseDataList data;
    explicit PhraseList();
    PhraseList(const PhraseList &other);
    PhraseList(PhraseList &&other);
    PhraseList(const AbstractFieldPhrase &other);
    PhraseList(const AbstractFieldPhrase *left, const AbstractFieldPhrase &right);
    PhraseList(PhraseList *left, PhraseList *right);
    PhraseList(PhraseList *left, const AbstractFieldPhrase *right);
    virtual ~PhraseList() = default;

    PhraseList &operator =(const PhraseList &other);
    PhraseList operator |(PhraseList &other);
    PhraseList operator |(const AbstractFieldPhrase &other);

private:
    void incAllDataParents();
};

QT_END_NAMESPACE

#endif // PHRASELIST_H
