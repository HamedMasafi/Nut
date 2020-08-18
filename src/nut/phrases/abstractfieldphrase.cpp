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
#include <QDebug>

NUT_BEGIN_NAMESPACE

AbstractFieldPhrase::AbstractFieldPhrase(PhraseData *d) : data(d)
{ }

AbstractFieldPhrase::AbstractFieldPhrase(const char *className,
                                         const char *fieldName)
    :data(new PhraseData(className, fieldName))
{
}

AbstractFieldPhrase::AbstractFieldPhrase(const AbstractFieldPhrase &other)
{
    data = other.data;
    data->ref.ref();
}

AbstractFieldPhrase::AbstractFieldPhrase(AbstractFieldPhrase &&other)
{
    data = other.data;
    other.data = nullptr;
}

AbstractFieldPhrase::~AbstractFieldPhrase()
{
    if (data) {
        if (!data->ref.deref()) {
            qDebug() << "deleted" << data->className
                     << data->fieldName;
            delete data;
        }
        else
            qDebug() << "more parents for" << data->className
                     << data->fieldName;
    }
}

PhraseList AbstractFieldPhrase::operator |(const AbstractFieldPhrase &other)
{
    return PhraseList(this, other);
}

ConditionalPhrase AbstractFieldPhrase::isNull()
{
    return ConditionalPhrase(this, PhraseData::Null);
}


ConditionalPhrase AbstractFieldPhrase::operator ==(const ConditionalPhrase
                                                   &other)
{
    return ConditionalPhrase(this, PhraseData::Equal,
                             const_cast<ConditionalPhrase&>(other));
}


AssignmentPhrase AbstractFieldPhrase::operator =(const QVariant &other)
{
    return AssignmentPhrase(this, other);
}

AssignmentPhrase AbstractFieldPhrase::operator =(const ConditionalPhrase &other)
{
    return AssignmentPhrase(new PhraseData(data, PhraseData::Equal, other.data));
}

AssignmentPhrase AbstractFieldPhrase::operator <<(const QVariant &other)
{
    return AssignmentPhrase(this, other);
}

void AbstractFieldPhrase::detach()
{
    auto clone = data->clone();
    if (!data->ref.deref())
        delete data;
    data = clone;
}

#define AbstractFieldPhraseOperatorVariant(class, op, cond) \
ConditionalPhrase class::operator op(const QVariant &other) \
{ \
    return ConditionalPhrase(this, cond, other); \
}

AbstractFieldPhraseOperatorVariant(AbstractFieldPhrase, ==, PhraseData::Equal)
AbstractFieldPhraseOperatorVariant(AbstractFieldPhrase, !=, PhraseData::NotEqual)

#define AbstractFieldPhraseOperatorField(op, cond) \
ConditionalPhrase AbstractFieldPhrase::operator op(const AbstractFieldPhrase &other) \
{ \
    return ConditionalPhrase(this, cond, other); \
}

AbstractFieldPhraseOperatorField(==, PhraseData::Equal)
AbstractFieldPhraseOperatorField(!=, PhraseData::NotEqual)
AbstractFieldPhraseOperatorField(< , PhraseData::Less)
AbstractFieldPhraseOperatorField(<=, PhraseData::LessEqual)
AbstractFieldPhraseOperatorField(> , PhraseData::Greater)
AbstractFieldPhraseOperatorField(>=, PhraseData::GreaterEqual)

AbstractFieldPhrase AbstractFieldPhrase::operator ~()
{
    AbstractFieldPhrase f(data->className, data->fieldName);
    f.data->isNot = !data->isNot;
    return f;
}

AbstractFieldPhrase AbstractFieldPhrase::operator !()
{
    AbstractFieldPhrase f(data->clone());
    f.data->isNot = !data->isNot;
    return f;
}

NUT_END_NAMESPACE
