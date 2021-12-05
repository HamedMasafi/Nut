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

#include "assignmentphraselist.h"
#include "phrasedata.h"
#include "assignmentphrase.h"

NUT_BEGIN_NAMESPACE


AssignmentPhraseList AssignmentPhrase::operator &(const AssignmentPhrase &other)
{
    return AssignmentPhraseList(this, &other);
}

AssignmentPhraseList::AssignmentPhraseList(const AssignmentPhrase &l)
{
    data.append(l.data);
    incAllDataParents();
}

AssignmentPhraseList::AssignmentPhraseList(AssignmentPhraseList *l,
                                           const AssignmentPhrase *r)
{
    data.append(l->data);
    data.append(r->data);
    incAllDataParents();
}

AssignmentPhraseList::AssignmentPhraseList(AssignmentPhrase *l,
                                           const AssignmentPhrase *r)
{
    data.append(l->data);
    data.append(r->data);
    incAllDataParents();
}

AssignmentPhraseList::AssignmentPhraseList(const AssignmentPhrase &r,
                                           const AssignmentPhrase &l)
{
    data.append(l.data);
    data.append(r.data);
    incAllDataParents();
}

AssignmentPhraseList AssignmentPhraseList::operator &(const AssignmentPhrase
                                                      &ph)
{
    return AssignmentPhraseList(this, &ph);
}

AssignmentPhraseList::~AssignmentPhraseList()
{
}

void AssignmentPhraseList::incAllDataParents()
{
    for (auto &d: data)
        d->ref.ref();
}


NUT_END_NAMESPACE
