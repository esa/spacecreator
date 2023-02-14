#pragma once

#include "ivconnectionlayertype.h"
#include "ivinterface.h"

#include <QString>

namespace ivm {
class IVConnection;
class IVFunctionType;
class IVComment;
class IVArchetypeReference;
class IVArchetypeLibraryReference;

namespace testutils {

IVInterface::CreationInfo init(IVInterface::InterfaceType t, IVFunctionType *fn, const QString &name = QString(),
        const shared::Id &id = shared::InvalidId);

IVInterfaceRequired *createRequiredIface(
        IVFunctionType *fn, const QString &name = QString(), const shared::Id &id = shared::InvalidId);

IVInterfaceProvided *createProvidedIface(
        IVFunctionType *fn, const QString &name = QString(), const shared::Id &id = shared::InvalidId);

IVInterface *createIface(IVFunctionType *fn, IVInterface::InterfaceType t = IVInterface::InterfaceType::Provided,
        const QString &name = QString(), const shared::Id &id = shared::InvalidId);

IVConnection *createConnection(
        IVFunctionType *source, IVFunctionType *target, const QString &name, const shared::Id &id = shared::InvalidId);

IVFunction *createFunction(
        const QString &name = QString(), QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);

IVFunctionType *createFunctionType(
        const QString &name = QString(), QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);

IVComment *createComment(
        const QString &name = QString(), QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);

IVConnectionLayerType *createConnectionLayer(QObject *parent = nullptr);

IVArchetypeReference *createArchetypeReference(
        const QString &archetypeLibraryName, const QString &archetypeFunctionName, QObject *parent = nullptr);

IVArchetypeLibraryReference *createArchetypeLibraryReference(
        const QString &archetypeLibraryName, const QString &archetypeLibraryPath, QObject *parent = nullptr);

}
}
