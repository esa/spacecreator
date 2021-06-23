#include "ivtestutils.h"

#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"

#include <algorithm>

namespace ivm {
namespace testutils {

IVInterface::CreationInfo init(IVInterface::InterfaceType t, IVFunctionType *fn, const QString &name)
{
    IVInterface::CreationInfo ci;
    ci.kind = IVInterface::OperationKind::Protected;
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

IVInterface *createIface(IVFunctionType *fn, IVInterface::InterfaceType t, const QString &name)
{
    IVInterface *interface = IVInterface::createIface(init(t, fn, name));
    if (fn->model()) {
        fn->model()->addObject(interface);
    }
    return interface;
}

IVConnection *createConnection(IVFunctionType *source, IVFunctionType *target, const QString &name)
{
    const QVector<IVInterface *> requiredInterfaces = target->ris();
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

    auto connection = new ivm::IVConnection(targetIf, sourceIf);
    ivm::IVModel *ivModel = source ? source->model() : target->model();
    if (ivModel) {
        ivModel->addObject(connection);
    }
    return connection;
}

IVInterfaceRequired *createRequiredIface(IVFunctionType *fn, const QString &name)
{
    return createIface(fn, ivm::IVInterface::InterfaceType::Required, name)->as<IVInterfaceRequired *>();
}

IVInterfaceProvided *createProvidedIface(IVFunctionType *fn, const QString &name)
{
    return createIface(fn, ivm::IVInterface::InterfaceType::Provided, name)->as<IVInterfaceProvided *>();
}

IVFunction *createFunction(const QString &name, QObject *parent)
{
    auto fn = new IVFunction(parent);
    fn->setTitle(name);
    return fn;
}

IVFunctionType *createFunctionType(const QString &name, QObject *parent)
{
    auto fnt = new IVFunctionType(parent);
    fnt->setTitle(name);
    return fnt;
}

IVComment *createComment(const QString &name, QObject *parent)
{
    auto comment = new IVComment(parent);
    comment->setTitle(name);
    return comment;
}

}
}
