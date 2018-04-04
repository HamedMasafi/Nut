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

#include "phrase.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif // QT_DEBUG

NUT_BEGIN_NAMESPACE

#ifdef QT_DEBUG
#define LOG(s) qDebug() << __func__ << s
#endif // QT_DEBUG

PhraseData::PhraseData() :
    className(""), fieldName(""),
    type(Field), operatorCond(NotAssign),
    left(nullptr), right(nullptr), operand(QVariant::Invalid), isNot(false), parents(1)
{ }

PhraseData::PhraseData(const char *className, const char *fieldName) :
    className(className), fieldName(fieldName),
    type(Field), operatorCond(NotAssign),
    left(nullptr), right(nullptr), operand(QVariant::Invalid), isNot(false), parents(1)
{ }

PhraseData::PhraseData(PhraseData *l, PhraseData::Condition o)
    : className(nullptr), fieldName(nullptr),
      type(WithoutOperand), operatorCond(o), left(l), right(nullptr),
      isNot(false), parents(1)
{
    l->parents++;
}

PhraseData::PhraseData(PhraseData *l, PhraseData::Condition o,
                       PhraseData *r)
    : className(nullptr), fieldName(nullptr),
      type(WithOther), operatorCond(o),
      left(l), right(r),
      isNot(false), parents(1)
{
    l->parents++;
    r->parents++;
}

PhraseData::PhraseData(PhraseData *l, PhraseData::Condition o, QVariant r)
    : className(nullptr), fieldName(nullptr),
      type(WithVariant), operatorCond(o), left(l),
      right(nullptr), operand(r), isNot(false), parents(1)
{ }

PhraseData *PhraseData::operator =(PhraseData *other)
{
#ifdef QT_DEBUG
    LOG("");
#endif // QT_DEBUG
    other->parents++;
    return other;
}

PhraseData &PhraseData::operator =(PhraseData &other)
{
    other.parents++;
    return other;
}

//PhraseData::PhraseData(const PhraseData &other) :
//    left(other.left), right(other.right), operand(other.operand),
//    operatorCond(other.operatorCond), className(other.className),
//    fieldName(other.fieldName), type(other.type), isNot(other.isNot),
//    parents(other.parents + 1)
//{ }

//PhraseData::PhraseData(const PhraseData *other) :
//    left(other->left), right(other->right), operand(other->operand),
//    operatorCond(other->operatorCond), className(other->className),
//    fieldName(other->fieldName), type(other->type), isNot(other->isNot),
//    parents(other->parents + 1)
//{ }

QString PhraseData::toString() const
{
    return QString("[%1].%2").arg(className).arg(fieldName);
}

PhraseData::~PhraseData()
{
//    if (type == WithOther) {
//        delete left;
//        delete right;
//    }
//    if (type == WithVariant) {
//        if (left)
//            delete left;
//    }

//    if (right && !--right->parents)
//        delete right;

//    if (left && !--left->parents)
//        delete left;
#ifdef QT_DEBUG
    LOG("");
#endif // QT_DEBUG
}

void PhraseData::cleanUp()
{
//    cleanUp(this);
}

void PhraseData::cleanUp(PhraseData *d)
{
    if (d->left)
        cleanUp(d->left);
    if (d->right)
        cleanUp(d->right);
}

AbstractFieldPhrase::AbstractFieldPhrase(PhraseData *d) : data(d)
{ }

AbstractFieldPhrase::AbstractFieldPhrase(const char *className,
                                         const char *fieldName)
    :data(new PhraseData(className, fieldName))
{
#ifdef QT_DEBUG
    qDebug() <<"AbstractFieldPhrase created"<<className<<fieldName;
#endif // QT_DEBUG
}

AbstractFieldPhrase::AbstractFieldPhrase(const AbstractFieldPhrase &other)
{
    data = other.data;
    data->parents++;
#ifdef QT_DEBUG
    qDebug() <<"Copy ctor"<<other.data->toString()<<other.data->parents;
#endif // QT_DEBUG
}

AbstractFieldPhrase::AbstractFieldPhrase(AbstractFieldPhrase &&other)
{
    data = other.data;
    data->parents++;
    other.data = nullptr;
}

AbstractFieldPhrase::~AbstractFieldPhrase()
{
#ifdef QT_DEBUG
    if (data) {
        LOG(data->toString()) << data->parents;
    } else {
        LOG("");
    }
#endif // QT_DEBUG

    if (data) {
        --data->parents;
        if (data->parents <= 0)
            delete data;
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

AbstractFieldPhrase AbstractFieldPhrase::operator !()
{

    AbstractFieldPhrase f(data->className, data->fieldName);
    f.data->isNot = !data->isNot;
    return f;
}

AssignmentPhrase AbstractFieldPhrase::operator =(const QVariant &other)
{
    return AssignmentPhrase(this, other);
}

AssignmentPhrase AbstractFieldPhrase::operator <<(const QVariant &other)
{
    return AssignmentPhrase(this, other);
}

PhraseList::PhraseList() : isValid(false)
{

}

PhraseList::PhraseList(const PhraseList &other) : isValid(true)
{
#ifdef QT_DEBUG
    LOG("");
#endif // QT_DEBUG
    data = qMove(other.data);
    const_cast<PhraseList&>(other).data.clear();
}

PhraseList::PhraseList(PhraseList &&other)
{
#ifdef QT_DEBUG
    LOG("");
#endif // QT_DEBUG
    data = other.data;
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

PhraseList::~PhraseList()
{
#ifdef QT_DEBUG
    LOG("");
#endif // QT_DEBUG
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
//    foreach (PhraseData *d, data)
//        d->parents++;
}

PhraseList PhraseList::operator |(PhraseList &other) {
    return PhraseList(this, &other);
}

AssignmentPhrase::AssignmentPhrase(PhraseData *d) : data(d)
{
    d->parents++;
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
    if (data)
        if (!--data->parents)
            delete data;
}

//AssignmentPhrase::AssignmentPhrase(AssignmentPhrase *l, const AssignmentPhrase *r)
//{
////    data = new PhraseData(l->data, PhraseData::Append, r->data);
//    qFatal("SS");
//}

AssignmentPhraseList AssignmentPhrase::operator &(const AssignmentPhrase &other)
{
    return AssignmentPhraseList(this, &other);
}

AssignmentPhraseList::AssignmentPhraseList()
{

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
    foreach (PhraseData *d, data)
        if (!--d->parents)
            delete d;
//    qDeleteAll(data);
    //    data.clear();
}

void AssignmentPhraseList::incAllDataParents()
{
    foreach (PhraseData *d, data)
        d->parents++;
}

ConditionalPhrase::ConditionalPhrase() : data(nullptr)
{ }

ConditionalPhrase::ConditionalPhrase(const ConditionalPhrase &other)
{
#ifdef QT_DEBUG
    qDebug() << "************* ctor called:";
#endif // QT_DEBUG
    data = other.data;
    data->parents++;
//    const_cast<ConditionalPhrase&>(other).data = 0;
}

#ifdef Q_COMPILER_RVALUE_REFS
ConditionalPhrase::ConditionalPhrase(const ConditionalPhrase &&other)
{
#ifdef QT_DEBUG
    qDebug() << "************* ctor called:";
#endif // QT_DEBUG
    this->data = qMove(other.data);
}
#endif

ConditionalPhrase::ConditionalPhrase(const PhraseData *data)
{
    this->data = const_cast<PhraseData*>(data);
    this->data->parents++;
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
#ifdef QT_DEBUG
    LOG("");
#endif // QT_DEBUG
    if (data) {
        data->cleanUp();
        if (!--data->parents)
            delete data;
    }
}

ConditionalPhrase &ConditionalPhrase::operator =(const ConditionalPhrase &other)
{
    data = other.data;
    data->parents++;
    return *this;
}

ConditionalPhrase ConditionalPhrase::operator ==(const QVariant &other)
{
    return ConditionalPhrase(this, PhraseData::Equal, other);
}

//ConditionalPhrase ConditionalPhrase::operator ==(const AbstractFieldPhrase &other)
//{
//    return ConditionalPhrase(this, PhraseData::Equal, other);
//}

//ConditionalPhrase ConditionalPhrase::operator &&(const ConditionalPhrase &other)
//{
//    return ConditionalPhrase(this, PhraseData::And,
//                             const_cast<ConditionalPhrase&>(other));
//}

//ConditionalPhrase ConditionalPhrase::operator ||(const ConditionalPhrase &other)
//{
//    return ConditionalPhrase(this, PhraseData::Or,
//                             const_cast<ConditionalPhrase&>(other));
//}

#define DECLARE_CONDITIONALPHRASE_OPERATORS(op, cond) \
ConditionalPhrase operator op(const ConditionalPhrase &l, \
                              const ConditionalPhrase &r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data->parents++;  \
    r.data->parents++;  \
    return p; \
} \
ConditionalPhrase operator op(const ConditionalPhrase &l, \
                              ConditionalPhrase &&r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data->parents++;  \
    r.data->parents++;  \
    return p; \
} \
ConditionalPhrase operator op(ConditionalPhrase &&l, \
                              const ConditionalPhrase &r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data->parents++;  \
    r.data->parents++;  \
    return p; \
} \
ConditionalPhrase operator op(ConditionalPhrase &&l, ConditionalPhrase &&r) \
{ \
    ConditionalPhrase p; \
    p.data = new PhraseData; \
    p.data->operatorCond = cond; \
    p.data->left = l.data; \
    p.data->right = r.data; \
    l.data->parents++;  \
    r.data->parents++;  \
    return p; \
}

DECLARE_CONDITIONALPHRASE_OPERATORS(==, PhraseData::Equal)
DECLARE_CONDITIONALPHRASE_OPERATORS(||, PhraseData::Or)
DECLARE_CONDITIONALPHRASE_OPERATORS(&&, PhraseData::And)

ConditionalPhrase ConditionalPhrase::operator !()
{
    ConditionalPhrase f(data);
    f.data->isNot = !data->isNot;
    return f;
}

PhraseDataList::PhraseDataList() : QList<PhraseData*>()
{

}

PhraseDataList::PhraseDataList(const PhraseDataList &other) : QList<PhraseData*>()
{
    PhraseDataList &o = const_cast<PhraseDataList&>(other);
    PhraseDataList::iterator i;
    for (i = o.begin(); i != o.end(); ++i)
        append(*i);
}

void PhraseDataList::append(PhraseData *d)
{
    d->parents++;
    QList<PhraseData*>::append(d);
}

void PhraseDataList::append(QList<PhraseData *> &dl)
{
    foreach (PhraseData *d, dl)
        d->parents++;
    QList<PhraseData*>::append(dl);
}

PhraseDataList::~PhraseDataList()
{
    QList<PhraseData*>::iterator i;
    for (i = begin(); i != end(); ++i) {
        (*i)->cleanUp();
        if (!--(*i)->parents)
            delete *i;
    }
}

//AssignmentPhraseList operator &(const AssignmentPhrase &l, const AssignmentPhrase &r)
//{
//    return AssignmentPhraseList(l, r);
//}

//AssignmentPhraseList operator &(const AssignmentPhrase &l, AssignmentPhrase &&r)
//{
//    r.data = 0;
//    return AssignmentPhraseList(l, r);
//}

//AssignmentPhraseList operator &(AssignmentPhrase &&l, const AssignmentPhrase &r)
//{
//    l.data = 0;
//    return AssignmentPhraseList(l, r);
//}

//AssignmentPhraseList operator &(AssignmentPhrase &&l, AssignmentPhrase &&r)
//{
//    r.data = l.data = 0;
//    return AssignmentPhraseList(l, r);
//}

NUT_END_NAMESPACE
