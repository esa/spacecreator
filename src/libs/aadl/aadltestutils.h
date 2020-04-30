#pragma once

#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"

#include <QString>

namespace aadl {
namespace testutils {

AADLObjectIface::CreationInfo init(AADLObjectIface::IfaceType t, AADLObjectFunctionType* fn, const QString& name = QString());

AADLObjectIface* createIface(AADLObjectFunctionType* fn, AADLObjectIface::IfaceType t = AADLObjectIface::IfaceType::Provided, const QString& name = QString());

}
}
