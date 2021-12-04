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
