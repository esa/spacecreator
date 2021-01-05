#include "aadltestutils.h"

#include "aadlobjectconnection.h"
#include "aadlobjectfunctiontype.h"

#include <algorithm>

namespace aadl {
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
    return AADLObjectIface::createIface(init(t, fn, name));
}

AADLObjectConnection *createConnection(
        AADLObjectFunctionType *source, AADLObjectFunctionType *target, const QString &name)
{
    const QVector<AADLObjectIface *> requiredInterfaces = target->ris();
    auto it = std::find_if(requiredInterfaces.begin(), requiredInterfaces.end(),
            [&name](AADLObjectIface *interface) { return interface->title() == name; });
    aadl::AADLObjectIface *sourceIf = it != requiredInterfaces.end()
            ? *it
            : aadl::testutils::createIface(source, aadl::AADLObjectIface::IfaceType::Required, name);

    const QVector<AADLObjectIface *> providedInterfaces = target->pis();
    it = std::find_if(providedInterfaces.begin(), providedInterfaces.end(),
            [&name](AADLObjectIface *interface) { return interface->title() == name; });
    aadl::AADLObjectIface *targetIf = it != providedInterfaces.end()
            ? *it
            : aadl::testutils::createIface(target, aadl::AADLObjectIface::IfaceType::Provided, name);

    return new aadl::AADLObjectConnection(targetIf, sourceIf);
}

}
}
