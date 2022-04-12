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

#ifndef NUT_CONSTS_H
#define NUT_CONSTS_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE

#define __NAME                  "name"
#define __TYPE                  "type"
#define __FIELDS                "fields"
#define __FOREIGN_KEYS          "foreign_keys"
#define __nut_FIELD             "field"

#define __nut_NAME_PERFIX       "nut_info::"
#define __nut_DB_VERSION        "database_version"
#define __nut_PRIMARY_KEY       "primary_key"
#define __nut_AUTO_INCREMENT    "auto_increment"
#define __nut_PRIMARY_KEY_AI    "primary_key_ai"
#define __nut_UNIQUE            "unique"
#define __nut_TABLE             "table"
#define __nut_TABLE_NAME        "table_name"

#define __nut_DISPLAY           "display"
#define __nut_LEN               "len"
#define __nut_DEFAULT_VALUE     "def"
#define __nut_NOT_NULL          "notnull"

#define __nut_FOREIGN_KEY       "foreign_key"
#define __nut_NEW               "new"
#define __nut_REMOVE            "remove"
#define __nut_CHANGE            "change"

#define NUT_NAMESPACE Nut
#ifdef NUT_NAMESPACE
#   define NUT_BEGIN_NAMESPACE      namespace NUT_NAMESPACE{
#   define NUT_END_NAMESPACE        }
#   define NUT_WRAP_NAMESPACE(x)    NUT_NAMESPACE::x
#else
#   define NUT_BEGIN_NAMESPACE
#   define NUT_END_NAMESPACE
#   define NUT_WRAP_NAMESPACE(x)    x
#endif

QT_END_NAMESPACE

#endif // NUT_CONSTS_H
