#include "aadltestutils.h"

#include "aadlconnection.h"
#include "aadlfunctiontype.h"
#include "aadlmodel.h"

#include <algorithm>

namespace ivm {
namespace testutils {

AADLIface::CreationInfo init(AADLIface::IfaceType t, AADLFunctionType *fn, const QString &name)
{
    AADLIface::CreationInfo ci;
    ci.kind = AADLIface::OperationKind::Protected;
    ci.type = t;
    ci.function = fn;
    ci.name = name;
    if (ci.name.isEmpty()) {
        const bool isRI = t == AADLIface::IfaceType::Required;
        const QString directionName = isRI ? QLatin1String("RI") : QLatin1String("PI");
        const int number = fn ? (isRI ? fn->ris().size() : fn->pis().size()) : -1;
        ci.name = QString("%1 #%2").arg(directionName, QString::number(number));
    }

    return ci;
}

AADLIface *createIface(AADLFunctionType *fn, AADLIface::IfaceType t, const QString &name)
{
    AADLIface *interface = AADLIface::createIface(init(t, fn, name));

    if (fn->objectsModel()) {
        fn->objectsModel()->addObject(interface);
    }
    return interface;
}

AADLConnection *createConnection(AADLFunctionType *source, AADLFunctionType *target, const QString &name)
{
    const QVector<AADLIface *> requiredInterfaces = target->ris();
    auto it = std::find_if(requiredInterfaces.begin(), requiredInterfaces.end(),
            [&name](AADLIface *interface) { return interface->title() == name; });
    ivm::AADLIface *sourceIf = it != requiredInterfaces.end()
            ? *it
            : ivm::testutils::createIface(source, ivm::AADLIface::IfaceType::Required, name);

    const QVector<AADLIface *> providedInterfaces = target->pis();
    it = std::find_if(providedInterfaces.begin(), providedInterfaces.end(),
            [&name](AADLIface *interface) { return interface->title() == name; });
    ivm::AADLIface *targetIf = it != providedInterfaces.end()
            ? *it
            : ivm::testutils::createIface(target, ivm::AADLIface::IfaceType::Provided, name);

    auto connection = new ivm::AADLConnection(targetIf, sourceIf);
    ivm::AADLModel *aadlModel = source ? source->objectsModel() : target->objectsModel();
    if (aadlModel) {
        aadlModel->addObject(connection);
    }
    return connection;
}

}
}
