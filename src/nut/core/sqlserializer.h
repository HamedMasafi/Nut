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

#ifndef SQLSERIALIZER_H
#define SQLSERIALIZER_H

#include <QVariant>
#include "nut_global.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

class SqlSerializer
{
public:
    SqlSerializer();
    virtual ~SqlSerializer();

    bool readString(QString &text, QString &out) const;

    QVariant fromString(const QString &value, const QMetaType::Type &type) const;
    QString toString(const QVariant &value) const;
    QList<int> toListInt(const QString &s) const;
    QList<int> toListInt(const QString &s, const QString &sep) const;
    QList<qreal> toListReal(const QString &s) const;
    QList<qreal> toListReal(const QString &s, const QString &sep) const;
    QList<float> toListFloat(const QString &s) const;

    QString fromList(const QList<int> &list) const;
    QString fromList(const QList<qreal> &list) const;
    QString fromList(const QList<float> &list) const;
    QString fromVariantList(const QVariantList &list) const;

    QVariant deserialize(const QString &value, const QMetaType::Type &type) const;
    QString serialize(const QVariant &value) const;
private:
    QString escapeString(const QString &str) const;
    QString unescapeString(const QString &str) const;
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // SQLSERIALIZER_H
