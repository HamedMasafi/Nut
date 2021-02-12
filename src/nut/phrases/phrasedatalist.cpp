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

#include "phrasedatalist.h"

NUT_BEGIN_NAMESPACE

PhraseDataList::PhraseDataList() : QList<PhraseData*>()
{

}

PhraseDataList::PhraseDataList(const PhraseDataList &other) : QList<PhraseData*>()
{
//    auto &o = const_cast<PhraseDataList&>(other);
    PhraseDataList::const_iterator i;
    for (i = other.constBegin(); i != other.constEnd(); ++i)
        append(*i);
}

void PhraseDataList::append(PhraseData *d)
{
    d->ref.ref();
    QList<PhraseData*>::append(d);
}

void PhraseDataList::append(QList<PhraseData *> &dl)
{
    for (PhraseData *&d: dl)
        d->ref.ref();
    QList<PhraseData*>::append(dl);
}

PhraseDataList::~PhraseDataList()
{
    QList<PhraseData*>::iterator i;
    for (i = begin(); i != end(); ++i) {
        (*i)->cleanUp();
        if (!(*i)->ref.deref())
            delete *i;
    }
}

NUT_END_NAMESPACE
