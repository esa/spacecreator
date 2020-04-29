#pragma once

#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"

#include <QString>

namespace TASTEtest {

inline aadl::AADLObjectIface::CreationInfo init(aadl::AADLObjectIface::IfaceType t, aadl::AADLObjectFunctionType *fn, const QString &name = QString())
{
    aadl::AADLObjectIface::CreationInfo ci;
    ci.kind = aadl::AADLObjectIface::OperationKind::Protected;
    ci.type = t;
    ci.function = fn;
    ci.name = name;
    if (ci.name.isEmpty()) {
        const bool isRI = t == aadl::AADLObjectIface::IfaceType::Required;
        const QString directionName = isRI ? QLatin1String("RI") : QLatin1String("PI");
        const int number = fn ? (isRI ? fn->ris().size() : fn->pis().size()) : -1;
        ci.name = QString("%1 #%2").arg(directionName, QString::number(number));
    }

    return ci;
}

inline aadl::AADLObjectIface* createIface(aadl::AADLObjectFunctionType *fn, const aadl::AADLObjectIface::IfaceType t = aadl::AADLObjectIface::IfaceType::Provided, const QString &name = QString())
{
    return aadl::AADLObjectIface::createIface(init(t, fn, name));
}

}
