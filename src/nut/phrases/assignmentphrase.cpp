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
#include "assignmentphrase.h"
#include "phrasedata.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

AssignmentPhrase::AssignmentPhrase(PhraseData *d) : data(d)
{
    d->ref.ref();
}

AssignmentPhrase::AssignmentPhrase(AbstractFieldPhrase *l, const QVariant r)
{

    data = new PhraseData(l->data, PhraseData::Equal, r);
//    l->data = 0;
}

AssignmentPhrase::AssignmentPhrase(AbstractFieldPhrase *l,
                                   const AssignmentPhrase *r)
{
    data = new PhraseData(l->data, PhraseData::Equal, r->data);
    //    l->data = 0;
}

AssignmentPhrase::AssignmentPhrase(AssignmentPhrase *ph, const QVariant &v)
{
    data = new PhraseData(ph->data, PhraseData::Equal, v);
}

//AssignmentPhrase::AssignmentPhrase(AssignmentPhrase &other)
//{
//    data = other.data;
//    other.data = 0;
//}

AssignmentPhrase::~AssignmentPhrase()
{
    if (data && data->ref.deref())
        delete data;
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
