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

#include "abstractfieldphrase.h"
#include "phraselist.h"

NUT_BEGIN_NAMESPACE

PhraseList::PhraseList() : isValid(false)
{

}

PhraseList::PhraseList(const PhraseList &other) : isValid(true)
{
    data = other.data;
}

PhraseList::PhraseList(PhraseList &&other)
{
    data = qMove(other.data);
    const_cast<PhraseList&>(other).data.clear();
}

PhraseList::PhraseList(const AbstractFieldPhrase &other) : isValid(true)
{
    data.append(other.data);
    incAllDataParents();
}

PhraseList::PhraseList(const AbstractFieldPhrase *left,
                       const AbstractFieldPhrase &right)
    : isValid(true)
{
    data.append(left->data);
    data.append(right.data);
    incAllDataParents();
}

PhraseList::PhraseList(PhraseList *left, PhraseList *right) : isValid(true)
{
//    data = qMove(left->data + right->data);
    data.append(left->data);
    data.append(right->data);
//    left->data.clear();
//    right->data.clear();
}

PhraseList::PhraseList(PhraseList *left, const AbstractFieldPhrase *right)
    : isValid(true)
{
    data.append(left->data);
    data.append(right->data);
    incAllDataParents();
}

PhraseList &PhraseList::operator =(const PhraseList &other)
{
    data.append(const_cast<PhraseList&>(other).data);
    return *this;
}

PhraseList PhraseList::operator |(const AbstractFieldPhrase &other) {
    return PhraseList(this, &other);
}

void PhraseList::incAllDataParents()
{
//    for (auto &d: data)
//        d->parents++;
}

PhraseList PhraseList::operator |(PhraseList &other) {
    return PhraseList(this, &other);
}

NUT_END_NAMESPACE
