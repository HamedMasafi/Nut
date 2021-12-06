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

#ifndef DBGEOGRAPHY_H
#define DBGEOGRAPHY_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QtCore/QVariant>

#include <QtNut/nut_global.h>

QT_BEGIN_NAMESPACE

class NUT_EXPORT DbGeography
{
    qreal m_longitude;
    qreal m_latitude;

public:
    explicit DbGeography();
    explicit DbGeography(const DbGeography &other);
    explicit DbGeography(const QVariant &value);

    qreal longitude() const;
    qreal latitude() const;

    void setLongitude(qreal longitude);
    void setLatitude(qreal latitude);

    operator QVariant();
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(NUT_WRAP_NAMESPACE(DbGeography))

#endif // DBGEOGRAPHY_H
