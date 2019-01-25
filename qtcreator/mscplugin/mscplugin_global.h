#pragma once

#include <QtGlobal>

#if defined(MSCPLUGIN_LIBRARY)
#  define MSCPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MSCPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif
