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

#include "dbgeography.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

DbGeography::DbGeography() : m_longitude(0), m_latitude(0)
{

}

DbGeography::DbGeography(const DbGeography &other)
{
    setLongitude(other.longitude());
    setLatitude(other.latitude());
}

DbGeography::DbGeography(const QVariant &value)
{
    QStringList parts = value.toString().split(',');
    if (parts.count() == 2) {
        setLongitude(parts[0].toDouble());
        setLatitude(parts[1].toDouble());
    } else {
        qWarning("Input value for DbGeography is invalid: %s",
                 qPrintable(value.toString()));
        setLongitude(0);
        setLatitude(0);
    }
}

qreal DbGeography::longitude() const
{
    return m_longitude;
}

qreal DbGeography::latitude() const
{
    return m_latitude;
}

void DbGeography::setLongitude(qreal longitude)

{
    if (qFuzzyCompare(m_longitude, longitude))
        return;

    m_longitude = longitude;
}

void DbGeography::setLatitude(qreal latitude)
{
    if (qFuzzyCompare(m_latitude, latitude))
        return;

    m_latitude = latitude;
}
DbGeography::operator QVariant()
{
    return QVariant::fromValue(QStringLiteral("%1,%2").arg(longitude()).arg(latitude()));
}

NUT_END_NAMESPACE

QT_END_NAMESPACE
