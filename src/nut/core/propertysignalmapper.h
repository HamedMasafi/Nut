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

#ifndef PROPERTYSIGNALMAPPER_H
#define PROPERTYSIGNALMAPPER_H

#include <QtNut/nut_global.h>

#include <QMetaMethod>
#include <QMetaProperty>

QT_BEGIN_NAMESPACE

class Table;
class PropertySignalMapper
{
    struct ClassData
    {
        QMetaMethod propertyChanged;
        QList<QMetaProperty> properties;
        QMap<QString, QString> signalMaps;
    };
    static QMap<QString, ClassData*> _data;

public:
    static void map(Table *obj);
    static QString changedProperty(QObject *obj, int senderSignalIndex);
};

QT_END_NAMESPACE

#endif // PROPERTYSIGNALMAPPER_H
