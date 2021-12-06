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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Nut API.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef NUT_P_H
#define NUT_P_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#   define VARIANT_TYPE_COMPARE(v, t)  v.typeId() == QMetaType::Q##t
#   define VARIANT_TYPE_COMPARE_X(v, vt, mt)  v.typeId() == QMetaType::mt
#   define METATYPE_TO_NAME(type) QMetaType(type).name()
#   define METATYPE_ID(v) static_cast<QMetaType::Type>(v.typeId())
#else
#   define VARIANT_TYPE_COMPARE(v, t)  v.type() == QVariant::t
#   define VARIANT_TYPE_COMPARE_X(v, vt, mt)  v.type() == QVariant::vt
#   define METATYPE_TO_NAME(type) QMetaType::typeName(type)
#   define METATYPE_ID(v) static_cast<QMetaType::Type>(v.type())
#endif

#endif
