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

#include "aadlconnection.h"

#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadliface.h"
#include "aadlmodel.h"
#include "aadlnamevalidator.h"
#include "connectioncreationvalidator.h"

#include <QDebug>
#include <QPointer>

namespace ivm {

struct ConnectionHolder {
    AADLConnection::EndPointInfo *m_from { nullptr };
    AADLConnection::EndPointInfo *m_to { nullptr };
};

struct AADLConnectionPrivate {
    AADLConnectionPrivate() {}
    AADLConnectionPrivate(AADLIface *ifaceSource, AADLIface *ifaceTarget) { setData(ifaceSource, ifaceTarget); }

    void setData(AADLIface *ifaceSource, AADLIface *ifaceTarget)
    {
        m_ifaceSource = ifaceSource;
        m_ifaceTarget = ifaceTarget;
        if (m_ifaceSource && m_ifaceTarget) {
            Q_ASSERT(m_ifaceSource != m_ifaceTarget); // An interface can't be connected to itself
            Q_ASSERT(ifaceSource->parentObject()
                    && ifaceTarget->parentObject()); // The interfaces have to belong to functions
            bool isReversed = false;
            if (ifaceSource->direction() != ifaceTarget->direction()) {
                isReversed = ifaceSource->direction() == AADLIface::IfaceType::Provided
                        && ifaceTarget->direction() == AADLIface::IfaceType::Required;
            } else {
                // PI->PI or RI->RI connection
                if (shared::isAncestorOf(ifaceSource->parentObject(), ifaceTarget->parentObject())) {
                    isReversed = ifaceSource->direction() == AADLIface::IfaceType::Required;
                } else if (shared::isAncestorOf(ifaceTarget->parentObject(), ifaceSource->parentObject())) {
                    isReversed = ifaceSource->direction() == AADLIface::IfaceType::Provided;
                }
            }

            if (isReversed) {
                m_ifaceSource.swap(m_ifaceTarget);
            }
        }
    }

    AADLIface *sourceIface() const { return m_ifaceSource; }
    AADLIface *targetIface() const { return m_ifaceTarget; }

    ConnectionHolder m_delayedInit;

private:
    QPointer<AADLIface> m_ifaceSource { nullptr };
    QPointer<AADLIface> m_ifaceTarget { nullptr };
};

AADLConnection::AADLConnection(AADLIface *ifaceSource, AADLIface *ifaceTarget, QObject *parent)
    : AADLObject(AADLObject::Type::Connection, QString(), parent)
    , d(new AADLConnectionPrivate { ifaceSource, ifaceTarget })
{
}

AADLConnection::AADLConnection(
        const AADLObject::Type t, AADLIface *ifaceSource, AADLIface *ifaceTarget, QObject *parent)
    : AADLObject(t, QString(), parent)
    , d(new AADLConnectionPrivate { ifaceSource, ifaceTarget })
{
}

AADLConnection::~AADLConnection()
{
    clearPostponedEndpoints();
}

QString AADLConnection::sourceName() const
{
    return source() ? source()->title() : QString();
}

AADLObject *AADLConnection::source() const
{
    return d->sourceIface() ? d->sourceIface()->function() : nullptr;
}

QString AADLConnection::targetName() const
{
    return target() ? target()->title() : QString();
}

AADLObject *AADLConnection::target() const
{
    return d->targetIface() ? d->targetIface()->function() : nullptr;
}

AADLIface *AADLConnection::sourceInterface() const
{
    return d->sourceIface();
}

QString AADLConnection::sourceInterfaceName() const
{
    return sourceInterface() ? sourceInterface()->title() : QString();
}

AADLIface *AADLConnection::targetInterface() const
{
    return d->targetIface();
}

QString AADLConnection::targetInterfaceName() const
{
    return targetInterface() ? targetInterface()->title() : QString();
}

void AADLConnection::setInheritPI()
{
    if (isOneDirection())
        return;

    handleInheritPIChange(AADLConnection::InheritPIChange::Inherit);
}

void AADLConnection::unsetInheritPI()
{
    if (isOneDirection())
        return;

    handleInheritPIChange(AADLConnection::InheritPIChange::NotInherit);
}

void AADLConnection::handleInheritPIChange(AADLConnection::InheritPIChange inheritance)
{
    if (isOneDirection())
        return;

    auto ri = selectIface<AADLIfaceRequired *>();
    const auto pi = selectIface<const AADLIfaceProvided *>();

    if (!pi || !ri)
        return;

    if (!ri->isInheritPI() && !ri->hasPrototypePi())
        return;

    const bool rmLabel = inheritance == AADLConnection::InheritPIChange::NotInherit;
    if (rmLabel) {
        disconnect(ri, &AADLIfaceRequired::propChanged_InheritPI, this, nullptr);
        disconnect(pi, &AADLIface::titleChanged, this, nullptr);
        ri->unsetPrototype(pi);
        return;
    }

    ri->setPrototype(pi);

    connect(pi, &AADLIface::titleChanged, this, &AADLConnection::handleProvidedTitleChanged, Qt::UniqueConnection);
    connect(ri, &AADLIfaceRequired::propChanged_InheritPI, this,
            QOverload<bool>::of(&AADLConnection::handleInheritPIChange), Qt::UniqueConnection);
}

void AADLConnection::handleProvidedTitleChanged(const QString &title)
{
    Q_UNUSED(title)

    if (isOneDirection())
        return;

    if (auto ri = selectIface<AADLIfaceRequired *>()) {
        if (ri->isInheritPI()) {
            const auto pi = selectIface<const AADLIfaceProvided *>();
            ri->setPrototype(pi);
        }
    }
}

void AADLConnection::handleInheritPIChange(bool enabled)
{
    if (isOneDirection())
        return;

    if (enabled)
        setInheritPI();
    else
        unsetInheritPI();
}

void AADLConnection::setDelayedStart(AADLConnection::EndPointInfo *start)
{
    d->m_delayedInit.m_from = start;
}

void AADLConnection::setDelayedEnd(AADLConnection::EndPointInfo *end)
{
    d->m_delayedInit.m_to = end;
}

bool AADLConnection::lookupEndpointsPostponed()
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

    auto findFunction = [&](const AADLConnection::EndPointInfo *const info) -> AADLObject * {
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

    auto findIface = [&](const AADLConnection::EndPointInfo *const info, AADLObject *parentObject) -> AADLIface * {
        if (!info || info->m_interfaceName.isEmpty()) {
            return nullptr;
        }

        AADLFunctionType *parentObj = parentObject ? parentObject->as<AADLFunctionType *>() : nullptr;
        AADLIface *aadlIface = objectsModel()->getIfaceByName(info->m_interfaceName, info->m_ifaceDirection, parentObj);
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
    AADLIface *ifaceFrom = findIface(d->m_delayedInit.m_from, objFrom);

    AADLObject *objTo = findFunction(d->m_delayedInit.m_to);
    AADLIface *ifaceTo = findIface(d->m_delayedInit.m_to, objTo);

    if (!objFrom || !ifaceFrom || !objTo || !ifaceTo) {
        return false;
    }

    const ConnectionCreationValidator::FailReason status = ConnectionCreationValidator::canConnect(
            objFrom->as<AADLFunction *>(), objTo->as<AADLFunction *>(), ifaceFrom, ifaceTo);

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

bool AADLConnection::needPostponedInit() const
{
    return !(d->sourceIface() && d->targetIface());
}

void AADLConnection::clearPostponedEndpoints()
{
    delete d->m_delayedInit.m_from;
    d->m_delayedInit.m_from = nullptr;

    delete d->m_delayedInit.m_to;
    d->m_delayedInit.m_to = nullptr;
}

bool AADLConnection::postInit()
{
    if (needPostponedInit() && !lookupEndpointsPostponed()) {
        qWarning() << "Postponed Connection initialization failed";
        return false;
    }

    setInheritPI();
    return true;
}

AADLConnection::ConnectionType AADLConnection::connectionType() const
{

    const AADLIface *srcIface = sourceInterface();
    const AADLIface *dstIface = targetInterface();
    if (!srcIface || !dstIface)
        return ConnectionType::NotAConnection;

    if (srcIface->isProvided())
        return dstIface->isRequired() ? ConnectionType::PI2RI : ConnectionType::PI2PI;
    else
        return dstIface->isRequired() ? ConnectionType::RI2RI : ConnectionType::RI2PI;
}

bool AADLConnection::isOneDirection() const
{
    return sourceInterface()->direction() == targetInterface()->direction();
}
/*!
   Returns the name of a connection. Usually that's the name of the provider interface
 */
QString AADLConnection::name() const
{
    return targetInterfaceName().isEmpty() ? sourceInterfaceName() : targetInterfaceName();
}

/*!
   Returns the parameters. Usually that's the parameters  of the provider interface
 */
QVector<IfaceParameter> AADLConnection::params() const
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

QDebug operator<<(QDebug debug, const ivm::AADLConnection &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << QString("%1.%2<->%3.%4")
                               .arg(c.sourceName(), c.sourceInterfaceName(), c.targetName(), c.targetInterfaceName());
    return debug;
}
