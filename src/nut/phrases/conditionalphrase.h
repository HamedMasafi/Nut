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

#ifndef CONDITIONALPHRASE_H
#define CONDITIONALPHRASE_H

#include <QtNut/phrasedata.h>

QT_BEGIN_NAMESPACE

class PhraseData;
class AbstractFieldPhrase;

#define SPECIALIZATION_NUMERIC_MEMBER(type, op, cond) \
ConditionalPhrase operator op(const QVariant &other) \
{ \
    return ConditionalPhrase(this, cond, other); \
}

class NUT_EXPORT ConditionalPhrase
{
public:
    PhraseData *data = nullptr;
//    QSharedPointer<PhraseData> leftDataPointer;
//    QSharedPointer<PhraseData> rightDataPointer;
    ConditionalPhrase();
    ConditionalPhrase(const ConditionalPhrase &other);
#ifdef Q_COMPILER_RVALUE_REFS
    ConditionalPhrase(ConditionalPhrase &&other);
#endif
    explicit ConditionalPhrase(const PhraseData *data);
    ConditionalPhrase(AbstractFieldPhrase *, PhraseData::Condition);
    ConditionalPhrase(AbstractFieldPhrase *, PhraseData::Condition, const QVariant &v);
    ConditionalPhrase(AbstractFieldPhrase *, PhraseData::Condition, const AbstractFieldPhrase &v);
    ConditionalPhrase(AbstractFieldPhrase *l, PhraseData::Condition cond, ConditionalPhrase &r);
    ConditionalPhrase(ConditionalPhrase *l, PhraseData::Condition cond, const AbstractFieldPhrase  &r);
    ConditionalPhrase(ConditionalPhrase *l, PhraseData::Condition cond, const QVariant &r);
    ConditionalPhrase(ConditionalPhrase *l, PhraseData::Condition cond, ConditionalPhrase &r);
    virtual ~ConditionalPhrase();

    ConditionalPhrase &operator =(const ConditionalPhrase &other);
    ConditionalPhrase operator ==(const QVariant &other);
//    ConditionalPhrase operator ==(const AbstractFieldPhrase &other);
//    ConditionalPhrase operator &&(const ConditionalPhrase &other);
//    ConditionalPhrase operator ||(const ConditionalPhrase &other);
    ConditionalPhrase operator !();

    SPECIALIZATION_NUMERIC_MEMBER(type, <,  PhraseData::Less)
    SPECIALIZATION_NUMERIC_MEMBER(type, <=, PhraseData::LessEqual)
    SPECIALIZATION_NUMERIC_MEMBER(type, >,  PhraseData::Greater)
    SPECIALIZATION_NUMERIC_MEMBER(type, >=, PhraseData::GreaterEqual)
};


#define DECLARE_CONDITIONALPHRASE_OPERATORS(op) \
    ConditionalPhrase NUT_EXPORT operator op(const ConditionalPhrase &l, const ConditionalPhrase &r); \
    ConditionalPhrase NUT_EXPORT operator op(const ConditionalPhrase &l, ConditionalPhrase &&r); \
    ConditionalPhrase NUT_EXPORT operator op(ConditionalPhrase &&l, const ConditionalPhrase &r); \
    ConditionalPhrase NUT_EXPORT operator op(ConditionalPhrase &&l, ConditionalPhrase &&r);

DECLARE_CONDITIONALPHRASE_OPERATORS(==)
DECLARE_CONDITIONALPHRASE_OPERATORS(&&)
DECLARE_CONDITIONALPHRASE_OPERATORS(||)

ConditionalPhrase NUT_EXPORT operator <(AbstractFieldPhrase &l, ConditionalPhrase &&r);
ConditionalPhrase NUT_EXPORT operator <=(AbstractFieldPhrase &l, ConditionalPhrase &&r);
ConditionalPhrase NUT_EXPORT operator >(AbstractFieldPhrase &l, ConditionalPhrase &&r);
ConditionalPhrase NUT_EXPORT operator >=(AbstractFieldPhrase &l, ConditionalPhrase &&r);

ConditionalPhrase NUT_EXPORT operator <(ConditionalPhrase &&l, ConditionalPhrase &&r);
ConditionalPhrase NUT_EXPORT operator <=(ConditionalPhrase &&l, ConditionalPhrase &&r);
ConditionalPhrase NUT_EXPORT operator >(ConditionalPhrase &&l, ConditionalPhrase &&r);
ConditionalPhrase NUT_EXPORT operator >=(ConditionalPhrase &&l, ConditionalPhrase &&r);


QT_END_NAMESPACE

#endif // CONDITIONALPHRASE_H
