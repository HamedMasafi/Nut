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

#ifndef PHRASEDATALIST_H
#define PHRASEDATALIST_H

#include <QtNut/phrasedata.h>

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class NUT_EXPORT PhraseDataList : public QList<PhraseData*>
{
public:
    PhraseDataList();
    PhraseDataList(const PhraseDataList &other);
    void append(PhraseData *d);
    void append(QList<PhraseData*> &dl);
    virtual ~PhraseDataList();
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // PHRASEDATALIST_H
