#pragma once

#include "aadliface.h"

#include <QString>

namespace ivm {
class AADLConnection;
class AADLFunctionType;

namespace testutils {

AADLIface::CreationInfo init(
        AADLIface::IfaceType t, AADLFunctionType *fn, const QString &name = QString());

AADLIface *createIface(AADLFunctionType *fn,
        AADLIface::IfaceType t = AADLIface::IfaceType::Provided, const QString &name = QString());

AADLConnection *createConnection(
        AADLFunctionType *source, AADLFunctionType *target, const QString &name);
}
}
