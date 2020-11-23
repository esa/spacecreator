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

#include "aadlnamevalidator.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "connectioncreationvalidator.h"

#include <QDebug>
#include <QPointer>

namespace aadl {

struct ConnectionHolder {
    AADLObjectConnection::EndPointInfo *m_from { nullptr };
    AADLObjectConnection::EndPointInfo *m_to { nullptr };
};

struct AADLObjectConnectionPrivate {
    AADLObjectConnectionPrivate() {}
    AADLObjectConnectionPrivate(AADLObjectIface *ifaceSource, AADLObjectIface *ifaceTarget)
    {
        setData(ifaceSource, ifaceTarget);
    }

    void setData(AADLObjectIface *ifaceSource, AADLObjectIface *ifaceTarget)
    {
        m_ifaceSource = ifaceSource;
        m_ifaceTarget = ifaceTarget;
        if (m_ifaceSource && m_ifaceTarget) {
            Q_ASSERT(m_ifaceSource != m_ifaceTarget); // An interface can't be connected to itself
            Q_ASSERT(ifaceSource->parentObject()
                    && ifaceTarget->parentObject()); // The interfaces have to belong to functions
            bool isReversed = false;
            if (ifaceSource->direction() != ifaceTarget->direction()) {
                isReversed = ifaceSource->direction() == AADLObjectIface::IfaceType::Provided
                        && ifaceTarget->direction() == AADLObjectIface::IfaceType::Required;
            } else {
                // PI->PI or RI->RI connection
                if (shared::isAncestorOf(ifaceSource->parentObject(), ifaceTarget->parentObject())) {
                    isReversed = ifaceSource->direction() == AADLObjectIface::IfaceType::Required;
                } else if (shared::isAncestorOf(ifaceTarget->parentObject(), ifaceSource->parentObject())) {
                    isReversed = ifaceSource->direction() == AADLObjectIface::IfaceType::Provided;
                }
            }

            if (isReversed) {
                m_ifaceSource.swap(m_ifaceTarget);
            }
        }
    }

    AADLObjectIface *sourceIface() const { return m_ifaceSource; }
    AADLObjectIface *targetIface() const { return m_ifaceTarget; }

    ConnectionHolder m_delayedInit;

private:
    QPointer<AADLObjectIface> m_ifaceSource { nullptr };
    QPointer<AADLObjectIface> m_ifaceTarget { nullptr };
};

AADLObjectConnection::AADLObjectConnection(AADLObjectIface *ifaceSource, AADLObjectIface *ifaceTarget, QObject *parent)
    : AADLObject(AADLObject::Type::Connection, QString(), parent)
    , d(new AADLObjectConnectionPrivate { ifaceSource, ifaceTarget })
{
}

AADLObjectConnection::AADLObjectConnection(
        const AADLObject::Type t, AADLObjectIface *ifaceSource, AADLObjectIface *ifaceTarget, QObject *parent)
    : AADLObject(t, QString(), parent)
    , d(new AADLObjectConnectionPrivate { ifaceSource, ifaceTarget })
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
    return d->sourceIface() ? d->sourceIface()->function() : nullptr;
}

QString AADLObjectConnection::targetName() const
{
    return target() ? target()->title() : QString();
}

AADLObject *AADLObjectConnection::target() const
{
    return d->targetIface() ? d->targetIface()->function() : nullptr;
}

AADLObjectIface *AADLObjectConnection::sourceInterface() const
{
    return d->sourceIface();
}

QString AADLObjectConnection::sourceInterfaceName() const
{
    return sourceInterface() ? sourceInterface()->title() : QString();
}

AADLObjectIface *AADLObjectConnection::targetInterface() const
{
    return d->targetIface();
}

QString AADLObjectConnection::targetInterfaceName() const
{
    return targetInterface() ? targetInterface()->title() : QString();
}

void AADLObjectConnection::setInheritPI()
{
    if (isOneDirection())
        return;

    handleInheritPIChange(AADLObjectConnection::InheritPIChange::Inherit);
}

void AADLObjectConnection::unsetInheritPI()
{
    if (isOneDirection())
        return;

    handleInheritPIChange(AADLObjectConnection::InheritPIChange::NotInherit);
}

void AADLObjectConnection::handleInheritPIChange(AADLObjectConnection::InheritPIChange inheritance)
{
    if (isOneDirection())
        return;

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

    if (isOneDirection())
        return;

    if (auto ri = selectIface<AADLObjectIfaceRequired *>()) {
        if (ri->isInheritPI()) {
            const auto pi = selectIface<const AADLObjectIfaceProvided *>();
            ri->setPrototype(pi);
        }
    }
}

void AADLObjectConnection::handleInheritPIChange(bool enabled)
{
    if (isOneDirection())
        return;

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
    if (!d->m_delayedInit.m_from) {
        qWarning() << "Can't connect, as the source is missing";
        return false;
    }
    if (!d->m_delayedInit.m_to) {
        qWarning() << "Can't connect, as the target is missing";
        return false;
    }

    Q_ASSERT(d->m_delayedInit.m_to->isReady());
    Q_ASSERT(d->m_delayedInit.m_from->isReady());

    auto findFunction = [&](const AADLObjectConnection::EndPointInfo *const info) -> AADLObject * {
        if (!info || info->m_functionName.isEmpty())
            return nullptr;

        AADLObject *aadlFunction = objectsModel()->getObjectByName(info->m_functionName);
        if (!aadlFunction) {
            // Try with old encoding
            aadlFunction = objectsModel()->getObjectByName(
                    AADLNameValidator::encodeName(AADLObject::Type::Function, info->m_functionName));
            if (!aadlFunction) {
                QString warningMessage = QStringLiteral("Unable to find Fn/FnType %1").arg(info->m_functionName);
                qWarning() << qPrintable(warningMessage);
            }
        }
        return aadlFunction;
    };

    auto findIface = [&](const AADLObjectConnection::EndPointInfo *const info,
                             AADLObject *parentObject) -> AADLObjectIface * {
        if (!info || info->m_interfaceName.isEmpty()) {
            return nullptr;
        }

        AADLObjectFunctionType *parentObj = parentObject ? parentObject->as<AADLObjectFunctionType *>() : nullptr;
        AADLObjectIface *aadlIface =
                objectsModel()->getIfaceByName(info->m_interfaceName, info->m_ifaceDirection, parentObj);
        if (!aadlIface) {
            // Try with old encoding
            const QString encodedName =
                    AADLNameValidator::encodeName(AADLObject::Type::RequiredInterface, info->m_interfaceName);
            aadlIface = objectsModel()->getIfaceByName(encodedName, info->m_ifaceDirection, parentObj);
            if (!aadlIface) {
                QString warningMessage = QStringLiteral("Unable to find Interface %1").arg(info->m_interfaceName);
                qWarning() << qPrintable(warningMessage);
            }
        }
        return aadlIface;
    };

    AADLObject *objFrom = findFunction(d->m_delayedInit.m_from);
    AADLObjectIface *ifaceFrom = findIface(d->m_delayedInit.m_from, objFrom);

    AADLObject *objTo = findFunction(d->m_delayedInit.m_to);
    AADLObjectIface *ifaceTo = findIface(d->m_delayedInit.m_to, objTo);

    if (!objFrom || !ifaceFrom || !objTo || !ifaceTo) {
        return false;
    }

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

    d->setData(ifaceFrom, ifaceTo);

    clearPostponedEndpoints();
    return true;
}

bool AADLObjectConnection::needPostponedInit() const
{
    return !(d->sourceIface() && d->targetIface());
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
    if (needPostponedInit() && !lookupEndpointsPostponed()) {
        qWarning() << "Postponed Connection initialization failed";
        return false;
    }

    setInheritPI();
    return true;
}

AADLObjectConnection::ConnectionType AADLObjectConnection::connectionType() const
{

    const AADLObjectIface *srcIface = sourceInterface();
    const AADLObjectIface *dstIface = targetInterface();
    if (!srcIface || !dstIface)
        return ConnectionType::NotAConnection;

    if (srcIface->isProvided())
        return dstIface->isRequired() ? ConnectionType::PI2RI : ConnectionType::PI2PI;
    else
        return dstIface->isRequired() ? ConnectionType::RI2RI : ConnectionType::RI2PI;
}

bool AADLObjectConnection::isOneDirection() const
{
    return sourceInterface()->direction() == targetInterface()->direction();
}
/*!
   Returns the name of a connection. Usually that's the name of the provider interface
 */
QString AADLObjectConnection::name() const
{
    return targetInterfaceName().isEmpty() ? sourceInterfaceName() : targetInterfaceName();
}

/*!
   Returns the parameters. Usually that's the parameters  of the provider interface
 */
QVector<IfaceParameter> AADLObjectConnection::params() const
{
    if (d->targetIface()) {
        return d->targetIface()->params();
    }
    if (d->sourceIface()) {
        return d->sourceIface()->params();
    }
    return {};
}

}
