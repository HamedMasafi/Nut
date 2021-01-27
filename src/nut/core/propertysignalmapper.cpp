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

#include "propertysignalmapper.h"
#include "table.h"

NUT_BEGIN_NAMESPACE

QMap<QString, PropertySignalMapper::ClassData*> PropertySignalMapper::_data;

void PropertySignalMapper::map(Table *obj)
{
    if (obj->_is_signals_mapped)
        return;
    ClassData *d;
    const QMetaObject *mo = obj->metaObject();
    auto className = QString::fromUtf8(mo->className());

    if (_data.contains(className)) {
        d = _data.value(className);

        for (auto &p: d->properties)
            QObject::connect(obj, p.notifySignal(), obj, d->propertyChanged);

    } else {
        d = new ClassData;
        d->propertyChanged = mo->method(mo->indexOfSlot("propertyChanged()"));

        for (int i = 0; i < mo->propertyCount(); ++i) {
            QMetaProperty p = mo->property(i);
            if (!strcmp(p.name(), "objectName"))
                continue;

            if (p.hasNotifySignal()) {
                d->signalMaps.insert(QLatin1String(p.notifySignal().name()),
                                     QLatin1String(p.name()));
                d->properties.append(p);
                QObject::connect(obj, p.notifySignal(), obj, d->propertyChanged);
            }
        }

        _data.insert(className, d);
    }

    obj->_is_signals_mapped = true;
}

QString PropertySignalMapper::changedProperty(QObject *obj, int senderSignalIndex)
{
    return _data.value(QString::fromUtf8(obj->metaObject()->className()))
            ->signalMaps.value(
                    QString::fromUtf8(obj->metaObject()->method(senderSignalIndex).name())
                );
}

NUT_END_NAMESPACE
