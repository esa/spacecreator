#include "aadltestutils.h"

#include "aadlobjectconnection.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectsmodel.h"

#include <algorithm>

namespace ivm {
namespace testutils {

AADLObjectIface::CreationInfo init(AADLObjectIface::IfaceType t, AADLObjectFunctionType *fn, const QString &name)
{
    AADLObjectIface::CreationInfo ci;
    ci.kind = AADLObjectIface::OperationKind::Protected;
    ci.type = t;
    ci.function = fn;
    ci.name = name;
    if (ci.name.isEmpty()) {
        const bool isRI = t == AADLObjectIface::IfaceType::Required;
        const QString directionName = isRI ? QLatin1String("RI") : QLatin1String("PI");
        const int number = fn ? (isRI ? fn->ris().size() : fn->pis().size()) : -1;
        ci.name = QString("%1 #%2").arg(directionName, QString::number(number));
    }

    return ci;
}

AADLObjectIface *createIface(AADLObjectFunctionType *fn, AADLObjectIface::IfaceType t, const QString &name)
{
    AADLObjectIface *interface = AADLObjectIface::createIface(init(t, fn, name));

    if (fn->objectsModel()) {
        fn->objectsModel()->addObject(interface);
    }
    return interface;
}

AADLObjectConnection *createConnection(
        AADLObjectFunctionType *source, AADLObjectFunctionType *target, const QString &name)
{
    const QVector<AADLObjectIface *> requiredInterfaces = target->ris();
    auto it = std::find_if(requiredInterfaces.begin(), requiredInterfaces.end(),
            [&name](AADLObjectIface *interface) { return interface->title() == name; });
    ivm::AADLObjectIface *sourceIf = it != requiredInterfaces.end()
            ? *it
            : ivm::testutils::createIface(source, ivm::AADLObjectIface::IfaceType::Required, name);

    const QVector<AADLObjectIface *> providedInterfaces = target->pis();
    it = std::find_if(providedInterfaces.begin(), providedInterfaces.end(),
            [&name](AADLObjectIface *interface) { return interface->title() == name; });
    ivm::AADLObjectIface *targetIf = it != providedInterfaces.end()
            ? *it
            : ivm::testutils::createIface(target, ivm::AADLObjectIface::IfaceType::Provided, name);

    auto connection = new ivm::AADLObjectConnection(targetIf, sourceIf);
    ivm::AADLObjectsModel *aadlModel = source ? source->objectsModel() : target->objectsModel();
    if (aadlModel) {
        aadlModel->addObject(connection);
    }
    return connection;
}

}
}
