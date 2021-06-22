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

#include "ivconnection.h"

#include "connectioncreationvalidator.h"
#include "errorhub.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"

#include <QDebug>
#include <QMetaEnum>
#include <QPointer>

namespace ivm {

struct ConnectionHolder {
    IVConnection::EndPointInfo *m_from { nullptr };
    IVConnection::EndPointInfo *m_to { nullptr };
};

struct IVConnectionPrivate {
    IVConnectionPrivate() { }
    IVConnectionPrivate(IVInterface *ifaceSource, IVInterface *ifaceTarget) { setData(ifaceSource, ifaceTarget); }

    void setData(IVInterface *ifaceSource, IVInterface *ifaceTarget)
    {
        m_ifaceSource = ifaceSource;
        m_ifaceTarget = ifaceTarget;
        if (m_ifaceSource && m_ifaceTarget) {
            Q_ASSERT(m_ifaceSource != m_ifaceTarget); // An interface can't be connected to itself
            Q_ASSERT(ifaceSource->parentObject()
                    && ifaceTarget->parentObject()); // The interfaces have to belong to functions
            bool isReversed = false;
            if (ifaceSource->direction() != ifaceTarget->direction()) {
                isReversed = ifaceSource->direction() == IVInterface::InterfaceType::Provided
                        && ifaceTarget->direction() == IVInterface::InterfaceType::Required;
            } else {
                // PI->PI or RI->RI connection
                if (shared::isAncestorOf(ifaceSource->parentObject(), ifaceTarget->parentObject())) {
                    isReversed = ifaceSource->direction() == IVInterface::InterfaceType::Required;
                } else if (shared::isAncestorOf(ifaceTarget->parentObject(), ifaceSource->parentObject())) {
                    isReversed = ifaceSource->direction() == IVInterface::InterfaceType::Provided;
                }
            }

            if (isReversed) {
                m_ifaceSource.swap(m_ifaceTarget);
            }
        }
    }

    IVInterface *sourceIface() const { return m_ifaceSource; }
    IVInterface *targetIface() const { return m_ifaceTarget; }

    ConnectionHolder m_delayedInit;

private:
    QPointer<IVInterface> m_ifaceSource { nullptr };
    QPointer<IVInterface> m_ifaceTarget { nullptr };
};

IVConnection::IVConnection(IVInterface *ifaceSource, IVInterface *ifaceTarget, QObject *parent)
    : IVObject(IVObject::Type::Connection, QString(), parent)
    , d(new IVConnectionPrivate { ifaceSource, ifaceTarget })
{
}

IVConnection::IVConnection(const IVObject::Type t, IVInterface *ifaceSource, IVInterface *ifaceTarget, QObject *parent)
    : IVObject(t, QString(), parent)
    , d(new IVConnectionPrivate { ifaceSource, ifaceTarget })
{
}

IVConnection::~IVConnection()
{
    clearPostponedEndpoints();
}

QString IVConnection::sourceName() const
{
    return source() ? source()->title() : QString();
}

IVObject *IVConnection::source() const
{
    return d->sourceIface() ? d->sourceIface()->function() : nullptr;
}

QString IVConnection::targetName() const
{
    return target() ? target()->title() : QString();
}

IVObject *IVConnection::target() const
{
    return d->targetIface() ? d->targetIface()->function() : nullptr;
}

IVInterface *IVConnection::sourceInterface() const
{
    return d->sourceIface();
}

QString IVConnection::sourceInterfaceName() const
{
    return sourceInterface() ? sourceInterface()->title() : QString();
}

IVInterface *IVConnection::targetInterface() const
{
    return d->targetIface();
}

QString IVConnection::targetInterfaceName() const
{
    return targetInterface() ? targetInterface()->title() : QString();
}

void IVConnection::setInheritPI()
{
    if (isOneDirection())
        return;

    handleInheritPIChange(IVConnection::InheritPIChange::Inherit);
}

void IVConnection::unsetInheritPI()
{
    if (isOneDirection())
        return;

    handleInheritPIChange(IVConnection::InheritPIChange::NotInherit);
}

void IVConnection::handleInheritPIChange(IVConnection::InheritPIChange inheritance)
{
    if (isOneDirection())
        return;

    auto ri = selectIface<IVInterfaceRequired *>();
    const auto pi = selectIface<const IVInterfaceProvided *>();

    if (!pi || !ri)
        return;

    if (!ri->isInheritPI() && !ri->hasPrototypePi())
        return;

    const bool rmLabel = inheritance == IVConnection::InheritPIChange::NotInherit;
    if (rmLabel) {
        disconnect(ri, &IVInterfaceRequired::propChanged_InheritPI, this, nullptr);
        disconnect(pi, &IVInterface::titleChanged, this, nullptr);
        ri->unsetPrototype(pi);
        return;
    }

    ri->setPrototype(pi);

    connect(pi, &IVInterface::titleChanged, this, &IVConnection::handleProvidedTitleChanged, Qt::UniqueConnection);
    connect(ri, &IVInterfaceRequired::propChanged_InheritPI, this,
            QOverload<bool>::of(&IVConnection::handleInheritPIChange), Qt::UniqueConnection);
}

void IVConnection::handleProvidedTitleChanged(const QString &title)
{
    Q_UNUSED(title)

    if (isOneDirection())
        return;

    if (auto ri = selectIface<IVInterfaceRequired *>()) {
        if (ri->isInheritPI()) {
            const auto pi = selectIface<const IVInterfaceProvided *>();
            ri->setPrototype(pi);
        }
    }
}

void IVConnection::handleInheritPIChange(bool enabled)
{
    if (isOneDirection())
        return;

    if (enabled)
        setInheritPI();
    else
        unsetInheritPI();
}

void IVConnection::setDelayedStart(IVConnection::EndPointInfo *start)
{
    d->m_delayedInit.m_from = start;
}

void IVConnection::setDelayedEnd(IVConnection::EndPointInfo *end)
{
    d->m_delayedInit.m_to = end;
}

bool IVConnection::lookupEndpointsPostponed()
{
    if (!d->m_delayedInit.m_from) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("Connection removed - Target source"), "");
        qWarning() << "Can't connect, as the source is missing";
        return false;
    }
    if (!d->m_delayedInit.m_to) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("Connection removed - Target missing"), "");
        qWarning() << "Can't connect, as the target is missing";
        return false;
    }

    Q_ASSERT(d->m_delayedInit.m_to->isReady());
    Q_ASSERT(d->m_delayedInit.m_from->isReady());

    auto findFunction = [&](const IVConnection::EndPointInfo *const info) -> IVObject * {
        if (!info || info->m_functionName.isEmpty())
            return nullptr;

        IVObject *ivFunction = model()->getObjectByName(info->m_functionName);
        if (!ivFunction) {
            // Try with old encoding
            ivFunction = model()->getObjectByName(
                    IVNameValidator::encodeName(IVObject::Type::Function, info->m_functionName));
            if (!ivFunction) {
                QString warningMessage = QStringLiteral("Unable to find Fn/FnType %1").arg(info->m_functionName);
                qWarning() << qPrintable(warningMessage);
                shared::ErrorHub::addError(shared::ErrorItem::Warning,
                        tr("Connection removed - Unable to find Fn/FnType %1").arg(info->m_functionName), "");
            }
        }
        return ivFunction;
    };

    auto findIface = [&](const IVConnection::EndPointInfo *const info, IVObject *parentObject) -> IVInterface * {
        if (!info || info->m_interfaceName.isEmpty()) {
            return nullptr;
        }

        IVFunctionType *parentObj = parentObject ? parentObject->as<IVFunctionType *>() : nullptr;
        IVInterface *ivIface = model()->getIfaceByName(info->m_interfaceName, info->m_ifaceDirection, parentObj);
        if (!ivIface) {
            // Try with old encoding
            const QString encodedName =
                    IVNameValidator::encodeName(IVObject::Type::RequiredInterface, info->m_interfaceName);
            ivIface = model()->getIfaceByName(encodedName, info->m_ifaceDirection, parentObj);
            if (!ivIface) {
                QString warningMessage = QStringLiteral("Unable to find Interface %1").arg(info->m_interfaceName);
                shared::ErrorHub::addError(shared::ErrorItem::Warning,
                        tr("Connection removed - Unable to find interface %1").arg(info->m_interfaceName), "");
                qWarning() << qPrintable(warningMessage);
            }
        }
        return ivIface;
    };

    IVObject *objFrom = findFunction(d->m_delayedInit.m_from);
    IVInterface *ifaceFrom = findIface(d->m_delayedInit.m_from, objFrom);

    IVObject *objTo = findFunction(d->m_delayedInit.m_to);
    IVInterface *ifaceTo = findIface(d->m_delayedInit.m_to, objTo);

    if (!objFrom || !ifaceFrom || !objTo || !ifaceTo) {
        return false;
    }

    const ConnectionCreationValidator::FailReason status = ConnectionCreationValidator::canConnect(
            objFrom->as<IVFunction *>(), objTo->as<IVFunction *>(), ifaceFrom, ifaceTo);

    if (status != ConnectionCreationValidator::FailReason::NotFail) {
        qWarning() << QString("Can't connect %1.%2->%3.%4 - ")
                              .arg(objFrom->title(), ifaceFrom->title(), objTo->title(), ifaceTo->title())
                   << status;
        qWarning() << objFrom;
        qWarning() << ifaceFrom << ifaceFrom->parentObject();
        qWarning() << objTo;
        qWarning() << ifaceTo << ifaceTo->parentObject();
        QMetaEnum metaEnum = QMetaEnum::fromType<ivm::ConnectionCreationValidator::FailReason>();
        QString warning = tr("Connection '%1'.'%2' <-> '%3'.'%4' removed - Reason: %5")
                                  .arg(objFrom->title(), ifaceFrom->titleUI(), objTo->title(), ifaceTo->titleUI())
                                  .arg(metaEnum.valueToKey(int(status)));
        shared::ErrorHub::addError(shared::ErrorItem::Warning, warning, "");

        return false;
    }

    d->setData(ifaceFrom, ifaceTo);

    clearPostponedEndpoints();
    return true;
}

bool IVConnection::needPostponedInit() const
{
    bool ok = !(d->sourceIface() && d->targetIface());
    if (!ok) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Warning, tr("Connection '%1' removed - misses an interface").arg(titleUI()), "");
    }
    return ok;
}

void IVConnection::clearPostponedEndpoints()
{
    delete d->m_delayedInit.m_from;
    d->m_delayedInit.m_from = nullptr;

    delete d->m_delayedInit.m_to;
    d->m_delayedInit.m_to = nullptr;
}

bool IVConnection::postInit()
{
    if (needPostponedInit() && !lookupEndpointsPostponed()) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("Postponed Connection initialization failed"), "");
        qWarning() << "Postponed Connection initialization failed";
        return false;
    }

    setInheritPI();
    return true;
}

IVConnection::ConnectionType IVConnection::connectionType() const
{

    const IVInterface *srcIface = sourceInterface();
    const IVInterface *dstIface = targetInterface();
    if (!srcIface || !dstIface)
        return ConnectionType::NotAConnection;

    if (srcIface->isProvided())
        return dstIface->isRequired() ? ConnectionType::PI2RI : ConnectionType::PI2PI;
    else
        return dstIface->isRequired() ? ConnectionType::RI2RI : ConnectionType::RI2PI;
}

bool IVConnection::isOneDirection() const
{
    return sourceInterface()->direction() == targetInterface()->direction();
}
/*!
   Returns the name of a connection. Usually that's the name of the provider interface
 */
QString IVConnection::name() const
{
    return targetInterfaceName().isEmpty() ? sourceInterfaceName() : targetInterfaceName();
}

/*!
   Returns the parameters. Usually that's the parameters  of the provider interface
 */
QVector<InterfaceParameter> IVConnection::params() const
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

QDebug operator<<(QDebug debug, const ivm::IVConnection &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << QString("%1.%2<->%3.%4")
                               .arg(c.sourceName(), c.sourceInterfaceName(), c.targetName(), c.targetInterfaceName());
    return debug;
}
