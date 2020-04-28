#pragma once

#include "tab_aadl/aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"

#include <QString>

namespace TASTEtest {

inline taste3::aadl::AADLObjectIface::CreationInfo init(taste3::aadl::AADLObjectIface::IfaceType t,
                                                        taste3::aadl::AADLObjectFunctionType *fn, const QString &name = QString())
{
    using namespace taste3::aadl;

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

inline taste3::aadl::AADLObjectIface *
createIface(taste3::aadl::AADLObjectFunctionType *fn,
            const taste3::aadl::AADLObjectIface::IfaceType t = taste3::aadl::AADLObjectIface::IfaceType::Provided,
            const QString &name = QString())
{
    return taste3::aadl::AADLObjectIface::createIface(init(t, fn, name));
}

} // namespace TASTEtest
