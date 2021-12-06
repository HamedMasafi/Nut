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

#ifndef ASSIGNMENTPHRASELIST_H
#define ASSIGNMENTPHRASELIST_H

#include <QtNut/nut_global.h>

QT_BEGIN_NAMESPACE

class PhraseData;
class AssignmentPhrase;
class NUT_EXPORT AssignmentPhraseList
{
public:
    QList<PhraseData*> data;
    explicit AssignmentPhraseList() = default;
    AssignmentPhraseList(const AssignmentPhrase &l);
    AssignmentPhraseList(AssignmentPhraseList *l, const AssignmentPhrase *r);
    AssignmentPhraseList(AssignmentPhrase *l, const AssignmentPhrase *r);
    AssignmentPhraseList(const AssignmentPhrase &r, const AssignmentPhrase &l);

    AssignmentPhraseList operator &(const AssignmentPhrase &ph);

    ~AssignmentPhraseList();

private:
    void incAllDataParents();
};

QT_END_NAMESPACE

#endif // ASSIGNMENTPHRASELIST_H
