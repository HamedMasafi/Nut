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

#ifndef DATEPHRASE_H
#define DATEPHRASE_H

#include <QtCore/QDateTime>
#include <type_traits>

#include <QtNut/fieldphrase.h>

QT_BEGIN_NAMESPACE

#define COMMON_OPERATORS_DECL(T) \
    AssignmentPhrase operator =(const T &other); \
    ConditionalPhrase operator <(const QVariant &other);                   \
    ConditionalPhrase operator <=(const QVariant &other);  \
    ConditionalPhrase operator >(const QVariant &other);   \
    ConditionalPhrase operator >=(const QVariant &other);  \
    ConditionalPhrase between(const QVariant &min, const QVariant &max);

#define COMMON_OPERATORS_IMPL(T) \
    AssignmentPhrase FieldPhrase<T>::operator =(const T &other) { \
        return AssignmentPhrase(this, other); \
    } \
    ConditionalPhrase FieldPhrase<T>::operator <(const QVariant &other) {                   \
        return ConditionalPhrase(this, PhraseData::Less, other);    \
    }   \
    ConditionalPhrase FieldPhrase<T>::operator <=(const QVariant &other) {  \
        return ConditionalPhrase(this, PhraseData::LessEqual, other);   \
    }   \
    ConditionalPhrase FieldPhrase<T>::operator >(const QVariant &other) {   \
        return ConditionalPhrase(this, PhraseData::Greater, other); \
    }   \
    ConditionalPhrase FieldPhrase<T>::operator >=(const QVariant &other) {  \
        return ConditionalPhrase(this, PhraseData::GreaterEqual, other);    \
    }   \
    ConditionalPhrase FieldPhrase<T>::between(const QVariant &min, const QVariant &max) \
    {   \
        return ConditionalPhrase(this, PhraseData::Between, \
                                 QVariantList() << min << max); \
    }

template<>
class NUT_EXPORT FieldPhrase<QDate> : public AbstractFieldPhrase
{
public:
    FieldPhrase(const char *className, const char *s);

    COMMON_OPERATORS_DECL(QDate)

    ConditionalPhrase addYears(int years);
    ConditionalPhrase addMonths(int months);
    ConditionalPhrase addDays(int days);

    ConditionalPhrase year();
    ConditionalPhrase month();
    ConditionalPhrase day();
};

template<>
class NUT_EXPORT FieldPhrase<QTime> : public AbstractFieldPhrase
{
public:
    FieldPhrase(const char *className, const char *s);

    COMMON_OPERATORS_DECL(QTime)

    ConditionalPhrase addHours(int hours);
    ConditionalPhrase addMinutes(int minutes);
    ConditionalPhrase addSeconds(int seconds);

    ConditionalPhrase hour();
    ConditionalPhrase minute();
    ConditionalPhrase second();
    ConditionalPhrase msec();
};

template<>
class NUT_EXPORT FieldPhrase<QDateTime> : public AbstractFieldPhrase
{
public:
    FieldPhrase(const char *className, const char *s);

    COMMON_OPERATORS_DECL(QDateTime)

    ConditionalPhrase addYears(int year);
    ConditionalPhrase addMonths(int months);
    ConditionalPhrase addDays(int days);

    ConditionalPhrase addHours(int hours);
    ConditionalPhrase addMinutes(int minutes);
    ConditionalPhrase addSeconds(int seconds);

    ConditionalPhrase year();
    ConditionalPhrase month();
    ConditionalPhrase day();

    ConditionalPhrase hour();
    ConditionalPhrase minute();
    ConditionalPhrase second();
    ConditionalPhrase msec();
};

QT_END_NAMESPACE

#endif // DATEPHRASE_H
