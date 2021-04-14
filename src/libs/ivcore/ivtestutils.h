#pragma once

#include "ivinterface.h"

#include <QString>

namespace ivm {
class IVConnection;
class IVFunctionType;

namespace testutils {

IVInterface::CreationInfo init(IVInterface::InterfaceType t, IVFunctionType *fn, const QString &name = QString());

IVInterfaceRequired *createRequiredIface(IVFunctionType *fn, const QString &name = QString());

IVInterfaceProvided *createProvidedIface(IVFunctionType *fn, const QString &name = QString());

IVInterface *createIface(
        IVFunctionType *fn, IVInterface::InterfaceType t = IVInterface::InterfaceType::Provided, const QString &name = QString());

IVConnection *createConnection(IVFunctionType *source, IVFunctionType *target, const QString &name);
}
}
