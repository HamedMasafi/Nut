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

#include "fieldphrase_date.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

FieldPhrase<QDate>::FieldPhrase(const char *className, const char *s) :
    AbstractFieldPhrase(className, s)
{}

ConditionalPhrase FieldPhrase<QDate>::addYears(int years) {
    return ConditionalPhrase(this, PhraseData::AddYears, years);
}

ConditionalPhrase FieldPhrase<QDate>::addMonths(int months) {
    return ConditionalPhrase(this, PhraseData::AddMonths, months);
}

ConditionalPhrase FieldPhrase<QDate>::addDays(int days) {
    return ConditionalPhrase(this, PhraseData::AddDays, days);
}

ConditionalPhrase FieldPhrase<QDate>::year() {
    return ConditionalPhrase(this, PhraseData::DatePartYear);
}

ConditionalPhrase FieldPhrase<QDate>::month() {
    return ConditionalPhrase(this, PhraseData::DatePartMonth);
}

ConditionalPhrase FieldPhrase<QDate>::day() {
    return ConditionalPhrase(this, PhraseData::DatePartDay);
}

FieldPhrase<QTime>::FieldPhrase(const char *className, const char *s) :
    AbstractFieldPhrase(className, s)
{}


ConditionalPhrase FieldPhrase<QTime>::addHours(int hours) {
    return ConditionalPhrase(this, PhraseData::AddHours, hours);
}

ConditionalPhrase FieldPhrase<QTime>::addMinutes(int minutes) {
    return ConditionalPhrase(this, PhraseData::AddMinutes, minutes);
}

ConditionalPhrase FieldPhrase<QTime>::addSeconds(int seconds) {
    return ConditionalPhrase(this, PhraseData::AddSeconds, seconds);
}

ConditionalPhrase FieldPhrase<QTime>::hour() {
    return ConditionalPhrase(this, PhraseData::DatePartHour);
}

ConditionalPhrase FieldPhrase<QTime>::minute() {
    return ConditionalPhrase(this, PhraseData::DatePartMinute);
}

ConditionalPhrase FieldPhrase<QTime>::second() {
    return ConditionalPhrase(this, PhraseData::DatePartSecond);
}

ConditionalPhrase FieldPhrase<QTime>::msec() {
    return ConditionalPhrase(this, PhraseData::DatePartMilisecond);
}

FieldPhrase<QDateTime>::FieldPhrase(const char *className, const char *s) :
    AbstractFieldPhrase(className, s)
{}

ConditionalPhrase FieldPhrase<QDateTime>::addYears(int years) {
    return ConditionalPhrase(this, PhraseData::AddYearsDateTime, years);
}

ConditionalPhrase FieldPhrase<QDateTime>::addMonths(int months) {
    return ConditionalPhrase(this, PhraseData::AddMonthsDateTime, months);
}

ConditionalPhrase FieldPhrase<QDateTime>::addDays(int days) {
    return ConditionalPhrase(this, PhraseData::AddDaysDateTime, days);
}

ConditionalPhrase FieldPhrase<QDateTime>::addHours(int hours) {
    return ConditionalPhrase(this, PhraseData::AddHoursDateTime, hours);
}

ConditionalPhrase FieldPhrase<QDateTime>::addMinutes(int minutes) {
    return ConditionalPhrase(this, PhraseData::AddMinutesDateTime, minutes);
}

ConditionalPhrase FieldPhrase<QDateTime>::addSeconds(int seconds) {
    return ConditionalPhrase(this, PhraseData::AddSecondsDateTime, seconds);
}

ConditionalPhrase FieldPhrase<QDateTime>::year() {
    return ConditionalPhrase(this, PhraseData::DatePartYear);
}

ConditionalPhrase FieldPhrase<QDateTime>::month() {
    return ConditionalPhrase(this, PhraseData::DatePartMonth);
}

ConditionalPhrase FieldPhrase<QDateTime>::day() {
    return ConditionalPhrase(this, PhraseData::DatePartDay);
}

ConditionalPhrase FieldPhrase<QDateTime>::hour() {
    return ConditionalPhrase(this, PhraseData::DatePartHour);
}

ConditionalPhrase FieldPhrase<QDateTime>::minute() {
    return ConditionalPhrase(this, PhraseData::DatePartMinute);
}

ConditionalPhrase FieldPhrase<QDateTime>::second() {
    return ConditionalPhrase(this, PhraseData::DatePartSecond);
}

ConditionalPhrase FieldPhrase<QDateTime>::msec() {
    return ConditionalPhrase(this, PhraseData::DatePartMilisecond);
}


COMMON_OPERATORS_IMPL(QDate)
COMMON_OPERATORS_IMPL(QTime)
COMMON_OPERATORS_IMPL(QDateTime)

NUT_END_NAMESPACE

QT_END_NAMESPACE
