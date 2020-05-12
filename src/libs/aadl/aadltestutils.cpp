#include "aadltestutils.h"

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

}
}
