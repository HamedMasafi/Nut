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

#ifndef FIELDPHRASE_H
#define FIELDPHRASE_H

#include <QtNut/nut_global.h>
#include <QtNut/abstractfieldphrase.h>

QT_BEGIN_NAMESPACE

template<typename T>
class FieldPhrase : public AbstractFieldPhrase
{
public:
    FieldPhrase(const char *className, const char *s) :
        AbstractFieldPhrase(className, s)
    {}

    virtual ~FieldPhrase() {}

    AssignmentPhrase operator =(const QVariant &other);
    ConditionalPhrase operator ==(const QVariant &other);
};

template<typename T>
Q_OUTOFLINE_TEMPLATE AssignmentPhrase FieldPhrase<T>::operator =(const QVariant &other) {
    return AssignmentPhrase(this, other);
}

template<typename T>
Q_OUTOFLINE_TEMPLATE ConditionalPhrase FieldPhrase<T>::operator ==(const QVariant &other) {
    return ConditionalPhrase(this, PhraseData::Equal, other);
}

//Date and time
#define CONDITIONAL_VARIANT_METHOD(name, cond) \
    ConditionalPhrase name(int val) \
    { \
        return ConditionalPhrase(this, cond, val); \
    }


QT_END_NAMESPACE

#endif // FIELDPHRASE_H
