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

#include <QDebug>

#include "abstractfieldphrase.h"
#include "conditionalphrase.h"
#include "phrasedata.h"

QT_BEGIN_NAMESPACE

ConditionalPhrase::ConditionalPhrase() : data(nullptr)
{ }

ConditionalPhrase::ConditionalPhrase(const ConditionalPhrase &other)
{
    data = other.data;
    data->ref.ref();
}

#ifdef Q_COMPILER_RVALUE_REFS
ConditionalPhrase::ConditionalPhrase(ConditionalPhrase &&other)
{
    data = other.data;
    other.data = nullptr;
}
#endif

ConditionalPhrase::ConditionalPhrase(const PhraseData *data)
{
    data = const_cast<PhraseData*>(data);
    data->ref.ref();
}

ConditionalPhrase::ConditionalPhrase(AbstractFieldPhrase *l,
                                     PhraseData::Condition cond)
{
    data = new PhraseData(l->data, cond);
}

ConditionalPhrase::ConditionalPhrase(AbstractFieldPhrase *l,
                                     PhraseData::Condition cond,
                                     const QVariant &v)
{
    data = new PhraseData(l->data, cond, v);
}

ConditionalPhrase::ConditionalPhrase(AbstractFieldPhrase *l,
                                     PhraseData::Condition cond,
                                     const AbstractFieldPhrase &other)
{
    data = new PhraseData(l->data, cond, other.data);
}

ConditionalPhrase::ConditionalPhrase(AbstractFieldPhrase *l,
                                     PhraseData::Condition cond,
                                     ConditionalPhrase &r)
{
    data = new PhraseData(l->data, cond, r.data);
    r.data = nullptr;
}

ConditionalPhrase::ConditionalPhrase(ConditionalPhrase *l,
                                     PhraseData::Condition cond,
                                     const AbstractFieldPhrase &r)
{
    data = new PhraseData(l->data, cond, r.data);
    l->data = nullptr;
}

ConditionalPhrase::ConditionalPhrase(ConditionalPhrase *l,
                                     PhraseData::Condition cond,
                                     const QVariant &r)
{
    data = new PhraseData(l->data, cond, r);
    l->data = nullptr;
}

ConditionalPhrase::ConditionalPhrase(ConditionalPhrase *l,
                                     PhraseData::Condition cond,
                                     ConditionalPhrase &r)
{
    data = new PhraseData(l->data, cond, r.data);
    l->data = nullptr;
    r.data = nullptr;
}

ConditionalPhrase::~ConditionalPhrase()
{
    if (data) {
        data->cleanUp();
        if (!data->ref.deref()) {
//            qDebug() << "deleted for cond";
            delete data;
        }
    }
}

ConditionalPhrase &ConditionalPhrase::operator =(const ConditionalPhrase &other)
{
    data = other.data;
    if (data)
        data->ref.ref();
    return *this;
}

ConditionalPhrase ConditionalPhrase::operator ==(const QVariant &other)
{
    return ConditionalPhrase(this, PhraseData::Equal, other);
}

#define DECLARE_CONDITIONALPHRASE_OPERATORS_IMPL(op, cond) \
ConditionalPhrase operator op(const ConditionalPhrase &l, \
                              const ConditionalPhrase &r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->type = PhraseData::WithOther; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data->ref.ref();  \
    r.data->ref.ref();  \
    return p; \
} \
ConditionalPhrase operator op(const ConditionalPhrase &l, \
                              ConditionalPhrase &&r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->type = PhraseData::WithOther; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data->ref.ref();  \
    r.data = nullptr;  \
    return p; \
} \
ConditionalPhrase operator op(ConditionalPhrase &&l, \
                              const ConditionalPhrase &r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->type = PhraseData::WithOther; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    r.data->ref.ref();  \
    l.data = nullptr;  \
    return p; \
} \
ConditionalPhrase operator op(ConditionalPhrase &&l, ConditionalPhrase &&r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->type = PhraseData::WithOther; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data = nullptr; \
    r.data = nullptr; \
    return p; \
}

DECLARE_CONDITIONALPHRASE_OPERATORS_IMPL(==, PhraseData::Equal)
DECLARE_CONDITIONALPHRASE_OPERATORS_IMPL(||, PhraseData::Or)
DECLARE_CONDITIONALPHRASE_OPERATORS_IMPL(&&, PhraseData::And)

ConditionalPhrase ConditionalPhrase::operator !()
{
    ConditionalPhrase f;
    f.data = data->clone();
    f.data->isNot = !f.data->isNot;
    return f;
}

ConditionalPhrase operator <(AbstractFieldPhrase &l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::Less, r);
}

ConditionalPhrase operator <=(AbstractFieldPhrase &l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::LessEqual, r);
}

ConditionalPhrase operator >(AbstractFieldPhrase &l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::Greater, r);
}

ConditionalPhrase operator >=(AbstractFieldPhrase &l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::GreaterEqual, r);
}

ConditionalPhrase operator <(ConditionalPhrase &&l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::Less, r);
}

ConditionalPhrase operator <=(ConditionalPhrase &&l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::LessEqual, r);
}

ConditionalPhrase operator >(ConditionalPhrase &&l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::Greater, r);
}

ConditionalPhrase operator >=(ConditionalPhrase &&l, ConditionalPhrase &&r)
{
    return ConditionalPhrase(&l, PhraseData::GreaterEqual, r);
}

QT_END_NAMESPACE
