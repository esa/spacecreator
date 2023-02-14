#include "ivtestutils.h"

#include "ivarchetypelibraryreference.h"
#include "ivarchetypereference.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"

#include <algorithm>

namespace ivm {
namespace testutils {

IVInterface::CreationInfo init(
        IVInterface::InterfaceType t, IVFunctionType *fn, const QString &name, const shared::Id &id)
{
    IVInterface::CreationInfo ci;
    ci.kind = IVInterface::OperationKind::Protected;
    ci.id = id;
    ci.type = t;
    ci.function = fn;
    ci.name = name;
    if (ci.name.isEmpty()) {
        const bool isRI = t == IVInterface::InterfaceType::Required;
        const QString directionName = isRI ? QLatin1String("RI") : QLatin1String("PI");
        const int number = fn ? (isRI ? fn->ris().size() : fn->pis().size()) : -1;
        ci.name = QString("%1_%2").arg(directionName, QString::number(number));
    }

    return ci;
}

IVInterface *createIface(IVFunctionType *fn, IVInterface::InterfaceType t, const QString &name, const shared::Id &id)
{
    IVInterface *interface = IVInterface::createIface(init(t, fn, name, id));
    if (fn->model()) {
        fn->model()->addObject(interface);
    }
    return interface;
}

IVConnection *createConnection(
        IVFunctionType *source, IVFunctionType *target, const QString &name, const shared::Id &id)
{
    const QVector<IVInterface *> requiredInterfaces = source->ris();
    auto it = std::find_if(requiredInterfaces.begin(), requiredInterfaces.end(),
            [&name](IVInterface *interface) { return interface->title() == name; });
    ivm::IVInterface *sourceIf = it != requiredInterfaces.end()
            ? *it
            : ivm::testutils::createIface(source, ivm::IVInterface::InterfaceType::Required, name);

    const QVector<IVInterface *> providedInterfaces = target->pis();
    it = std::find_if(providedInterfaces.begin(), providedInterfaces.end(),
            [&name](IVInterface *interface) { return interface->title() == name; });
    ivm::IVInterface *targetIf = it != providedInterfaces.end()
            ? *it
            : ivm::testutils::createIface(target, ivm::IVInterface::InterfaceType::Provided, name);

    auto connection = new ivm::IVConnection(targetIf, sourceIf, nullptr, id);
    ivm::IVModel *ivModel = source ? source->model() : target->model();
    if (ivModel) {
        ivModel->addObject(connection);
    }
    return connection;
}

IVInterfaceRequired *createRequiredIface(IVFunctionType *fn, const QString &name, const shared::Id &id)
{
    return createIface(fn, ivm::IVInterface::InterfaceType::Required, name, id)->as<IVInterfaceRequired *>();
}

IVInterfaceProvided *createProvidedIface(IVFunctionType *fn, const QString &name, const shared::Id &id)
{
    return createIface(fn, ivm::IVInterface::InterfaceType::Provided, name, id)->as<IVInterfaceProvided *>();
}

IVFunction *createFunction(const QString &name, QObject *parent, const shared::Id &id)
{
    auto fn = new IVFunction(parent, id);
    fn->setTitle(name);
    return fn;
}

IVFunctionType *createFunctionType(const QString &name, QObject *parent, const shared::Id &id)
{
    auto fnt = new IVFunctionType(parent, id);
    fnt->setTitle(name);
    return fnt;
}

IVComment *createComment(const QString &name, QObject *parent, const shared::Id &id)
{
    auto comment = new IVComment(parent, id);
    comment->setTitle(name);
    return comment;
}

IVConnectionLayerType *createConnectionLayer(QObject *parent)
{
    auto layer = new IVConnectionLayerType(parent);
    layer->setTitle(ivm::IVConnectionLayerType::DefaultLayerName);
    return layer;
}

IVArchetypeReference *createArchetypeReference(
        const QString &archetypeLibraryName, const QString &archetypeFunctionName, QObject *parent)
{
    auto archetypeReference = new IVArchetypeReference(parent, archetypeLibraryName, archetypeFunctionName);
    return archetypeReference;
}

IVArchetypeLibraryReference *createArchetypeLibraryReference(
        const QString &archetypeLibraryName, const QString &archetypeLibraryPath, QObject *parent)
{
    auto archetypeLibraryReference =
            new IVArchetypeLibraryReference(archetypeLibraryName, archetypeLibraryPath, parent);
    return archetypeLibraryReference;
}

}
}
