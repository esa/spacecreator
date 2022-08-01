#pragma once

#include "ivconnectionlayertype.h"
#include "ivinterface.h"

#include <QString>

namespace ivm {
class IVConnection;
class IVFunctionType;
class IVComment;
class IVArchetypeReference;

namespace testutils {

IVInterface::CreationInfo init(IVInterface::InterfaceType t, IVFunctionType *fn, const QString &name = QString());

IVInterfaceRequired *createRequiredIface(IVFunctionType *fn, const QString &name = QString());

IVInterfaceProvided *createProvidedIface(IVFunctionType *fn, const QString &name = QString());

IVInterface *createIface(IVFunctionType *fn, IVInterface::InterfaceType t = IVInterface::InterfaceType::Provided,
        const QString &name = QString());

IVConnection *createConnection(IVFunctionType *source, IVFunctionType *target, const QString &name);

IVFunction *createFunction(const QString &name = QString(), QObject *parent = nullptr);

IVFunctionType *createFunctionType(const QString &name = QString(), QObject *parent = nullptr);

IVComment *createComment(const QString &name = QString(), QObject *parent = nullptr);

IVConnectionLayerType *createConnectionLayer(QObject *parent = nullptr);

IVArchetypeReference *createArchetypeReference(
        const QString &archetypeLibraryName, const QString &archetypeFunctionName, QObject *parent = nullptr);

}
}
