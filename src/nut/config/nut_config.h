#ifndef NUT_CONFIG_H
#define NUT_CONFIG_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE

#if defined(NUT_SHARED) || !defined(NUT_STATIC)
#  ifdef NUT_STATIC
#    error "Both NUT_SHARED and NUT_STATIC defined, please make up your mind"
#  endif
#  ifndef NUT_SHARED
#    define NUT_SHARED
#  endif
#  if defined(NUT_BUILD_LIB)
#    define NUT_EXPORT Q_DECL_EXPORT
#  else
#    define NUT_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define NUT_EXPORT
#endif

QT_END_NAMESPACE

#endif // NUT_CONFIG_H
