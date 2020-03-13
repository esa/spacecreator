/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "aadlobjectconnection.h"

#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "tab_interface/connectioncreationvalidator.h"

#include <QDebug>
#include <QPointer>

namespace taste3 {
namespace aadl {

struct ConnectionHolder {
    AADLObjectConnection::EndPointInfo *m_from { nullptr };
    AADLObjectConnection::EndPointInfo *m_to { nullptr };
};

struct AADLObjectConnectionPrivate {
    AADLObjectConnectionPrivate() {}
    AADLObjectConnectionPrivate(AADLObject *source, AADLObject *target, AADLObjectIface *ifaceSource,
                                AADLObjectIface *ifaceTarget)
        : m_source(source)
        , m_target(target)
        , m_ifaceSource(ifaceSource)
        , m_ifaceTarget(ifaceTarget)
    {
    }
    QPointer<AADLObject> m_source { nullptr };
    QPointer<AADLObject> m_target { nullptr };
    QPointer<AADLObjectIface> m_ifaceSource { nullptr };
    QPointer<AADLObjectIface> m_ifaceTarget { nullptr };

    ConnectionHolder m_delayedInit;
};

AADLObjectConnection::AADLObjectConnection(AADLObject *source, AADLObject *target, AADLObjectIface *ifaceSource,
                                           AADLObjectIface *ifaceTarget, QObject *parent)
    : AADLObject(AADLObject::Type::Connection, QString(), parent)
    , d(new AADLObjectConnectionPrivate { source, target, ifaceSource, ifaceTarget })
{
}

AADLObjectConnection::~AADLObjectConnection()
{
    clearPostponedEndpoints();
}

QString AADLObjectConnection::sourceName() const
{
    return source() ? source()->title() : QString();
}

AADLObject *AADLObjectConnection::source() const
{
    return d->m_source;
}

QString AADLObjectConnection::targetName() const
{
    return target() ? target()->title() : QString();
}

AADLObject *AADLObjectConnection::target() const
{
    return d->m_target;
}

AADLObjectIface *AADLObjectConnection::sourceInterface() const
{
    return d->m_ifaceSource;
}

QString AADLObjectConnection::sourceInterfaceName() const
{
    return sourceInterface() ? sourceInterface()->title() : QString();
}

AADLObjectIface *AADLObjectConnection::targetInterface() const
{
    return d->m_ifaceTarget;
}

QString AADLObjectConnection::targetInterfaceName() const
{
    return targetInterface() ? targetInterface()->title() : QString();
}

bool AADLObjectConnection::sourceInterfaceIsRequired() const
{
    return sourceInterface() ? sourceInterface()->isRequired() : false;
}

bool AADLObjectConnection::sourceInterfaceIsProvided() const
{
    return sourceInterface() ? sourceInterface()->isProvided() : false;
}

bool AADLObjectConnection::targetInterfaceIsRequired() const
{
    return targetInterface() ? targetInterface()->isRequired() : false;
}

bool AADLObjectConnection::targetInterfaceIsProvided() const
{
    return targetInterface() ? targetInterface()->isProvided() : false;
}

void AADLObjectConnection::setInheritPI()
{
    handleInheritPIChange(AADLObjectConnection::InheritPIChange::Inherit);
}

void AADLObjectConnection::unsetInheritPI()
{
    handleInheritPIChange(AADLObjectConnection::InheritPIChange::NotInherit);
}

void AADLObjectConnection::handleInheritPIChange(AADLObjectConnection::InheritPIChange inheritance)
{
    auto ri = selectIface<AADLObjectIfaceRequired *>();
    const auto pi = selectIface<const AADLObjectIfaceProvided *>();

    if (!pi || !ri)
        return;

    if (!ri->isInheritPI())
        return;

    const bool rmLabel = inheritance == AADLObjectConnection::InheritPIChange::NotInherit;
    if (rmLabel) {
        disconnect(ri, &AADLObjectIfaceRequired::propChanged_InheritPI, this, nullptr);
        disconnect(pi, &AADLObjectIface::titleChanged, this, nullptr);
        ri->unsetPrototype(pi);
        return;
    }

    ri->setPrototype(pi);

    connect(pi, &AADLObjectIface::titleChanged, this, &AADLObjectConnection::handleProvidedTitleChanged,
            Qt::UniqueConnection);
    connect(ri, &AADLObjectIfaceRequired::propChanged_InheritPI, this,
            QOverload<bool>::of(&AADLObjectConnection::handleInheritPIChange), Qt::UniqueConnection);
}

void AADLObjectConnection::handleProvidedTitleChanged(const QString &title)
{
    Q_UNUSED(title)

    if (auto ri = selectIface<AADLObjectIfaceRequired *>()) {
        if (ri->isInheritPI()) {
            const auto pi = selectIface<const AADLObjectIfaceProvided *>();
            ri->setPrototype(pi);
        }
    }
}

void AADLObjectConnection::handleInheritPIChange(bool enabled)
{
    if (enabled)
        setInheritPI();
    else
        unsetInheritPI();
}

void AADLObjectConnection::setDelayedStart(AADLObjectConnection::EndPointInfo *start)
{
    d->m_delayedInit.m_from = start;
}

void AADLObjectConnection::setDelayedEnd(AADLObjectConnection::EndPointInfo *end)
{
    d->m_delayedInit.m_to = end;
}

bool AADLObjectConnection::lookupEndpointsPostponed()
{
    if (!d->m_delayedInit.m_from || !d->m_delayedInit.m_to)
        return true;

    AADLObject *objFrom = objectsModel()->getObjectByName(d->m_delayedInit.m_from->m_functionName);
    AADLObjectIface *ifaceFrom = objectsModel()->getIfaceByName(
            d->m_delayedInit.m_from->m_interfaceName, d->m_delayedInit.m_from->m_ifaceDirection,
            objFrom ? objFrom->as<AADLObjectFunctionType *>() : nullptr);

    AADLObject *objTo = objectsModel()->getObjectByName(d->m_delayedInit.m_to->m_functionName);
    AADLObjectIface *ifaceTo = objectsModel()->getIfaceByName(d->m_delayedInit.m_to->m_interfaceName,
                                                              d->m_delayedInit.m_to->m_ifaceDirection,
                                                              objTo ? objTo->as<AADLObjectFunctionType *>() : nullptr);

    if (!objFrom || !ifaceFrom || !objTo || !ifaceTo)
        return false;

    const ConnectionCreationValidator::FailReason status = ConnectionCreationValidator::canConnect(
            objFrom->as<AADLObjectFunction *>(), objTo->as<AADLObjectFunction *>(), ifaceFrom, ifaceTo);

    if (status != ConnectionCreationValidator::FailReason::NotFail) {
        qWarning() << QString("Can't connect %1.%2->%3.%4 - ")
                              .arg(objFrom->title(), ifaceFrom->title(), objTo->title(), ifaceTo->title())
                   << status;
        qWarning() << objFrom;
        qWarning() << ifaceFrom << ifaceFrom->parentObject();
        qWarning() << objTo;
        qWarning() << ifaceTo << ifaceTo->parentObject();

        return false;
    }

    d->m_source = objFrom;
    d->m_target = objTo;
    d->m_ifaceSource = ifaceFrom;
    d->m_ifaceTarget = ifaceTo;

    clearPostponedEndpoints();
    return true;
}

void AADLObjectConnection::clearPostponedEndpoints()
{
    delete d->m_delayedInit.m_from;
    d->m_delayedInit.m_from = nullptr;

    delete d->m_delayedInit.m_to;
    d->m_delayedInit.m_to = nullptr;
}

bool AADLObjectConnection::postInit()
{
    if (!lookupEndpointsPostponed()) {
        qWarning() << "Postponed Connection initialization failed";
        return false;
    }

    setInheritPI();
    return true;
}

} // ns aadl
} // ns taste3
