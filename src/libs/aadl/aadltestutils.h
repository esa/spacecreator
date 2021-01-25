#pragma once

#include "aadlobjectiface.h"

#include <QString>

namespace ivm {
class AADLObjectConnection;
class AADLObjectFunctionType;

namespace testutils {

AADLObjectIface::CreationInfo init(
        AADLObjectIface::IfaceType t, AADLObjectFunctionType *fn, const QString &name = QString());

AADLObjectIface *createIface(AADLObjectFunctionType *fn,
        AADLObjectIface::IfaceType t = AADLObjectIface::IfaceType::Provided, const QString &name = QString());

AADLObjectConnection *createConnection(
        AADLObjectFunctionType *source, AADLObjectFunctionType *target, const QString &name);
}
}
