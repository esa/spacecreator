/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivsystemchecks.h"

#include "interface/interfacedocument.h"
#include "ivconnection.h"
#include "ivconnectionchain.h"
#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclaration.h"
#include "mscmodel.h"

#include <QDebug>
#include <algorithm>

namespace scs {

IvSystemChecks::IvSystemChecks(QObject *parent)
    : msc::SystemChecks(parent)
{
}

IvSystemChecks::~IvSystemChecks() { }

void IvSystemChecks::setMscCore(msc::MSCEditorCore *mscCore)
{
    m_mscCore = mscCore;
}

void IvSystemChecks::setIvCore(QSharedPointer<ive::IVEditorCore> ivCore)
{
    if (ivCore == m_ivCore) {
        return;
    }

    if (ivm::IVModel *model = ivModel()) {
        disconnect(model, nullptr, this, nullptr);
    }

    m_ivCore = ivCore;
    Q_EMIT ivDataReset();
}

/*!
   Returns a pointer to the IV editor model
 */
const QSharedPointer<ive::IVEditorCore> &IvSystemChecks::ivCore() const
{
    return m_ivCore;
}

/*!
   Checks all instances if they are defined in the IV model as function
 */
QVector<QPair<msc::MscChart *, msc::MscInstance *>> IvSystemChecks::checkInstanceNames() const
{
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result;
    if (!m_ivCore || !m_mscCore) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscCore->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : charts) {
        for (msc::MscInstance *instance : chart->instances()) {
            ivm::IVFunction *ivFunction = correspondingFunction(instance);
            if (!ivFunction) {
                result << QPair<msc::MscChart *, msc::MscInstance *>(chart, instance);
            }
        }
    }

    return result;
}

/*!
   Checks if in a chart instances with parent/child relations re used
 */
QVector<QPair<msc::MscChart *, msc::MscInstance *>> IvSystemChecks::checkInstanceRelations() const
{
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result;
    if (!m_ivCore || !m_mscCore) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscCore->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : charts) {
        QVector<QPair<msc::MscInstance *, ivm::IVFunction *>> pairs;
        for (msc::MscInstance *instance : chart->instances()) {
            ivm::IVFunction *ivFunction = correspondingFunction(instance);
            if (ivFunction) {
                pairs << QPair<msc::MscInstance *, ivm::IVFunction *>(instance, ivFunction);
            }
        }

        QVector<ivm::IVFunction *> ivChartFunctions;
        for (QPair<msc::MscInstance *, ivm::IVFunction *> pair : pairs) {
            ivChartFunctions.append(pair.second);
        }
        for (QPair<msc::MscInstance *, ivm::IVFunction *> pair : pairs) {
            if (hasAncestor(pair.second, ivChartFunctions) || hasDescendant(pair.second, ivChartFunctions)) {
                result << QPair<msc::MscChart *, msc::MscInstance *>(chart, pair.first);
            }
        }
    }

    return result;
}

/*!
   Checks if the given MSC instance has a corresponding iv function
 */
bool IvSystemChecks::checkInstance(const msc::MscInstance *instance) const
{
    if (!m_ivCore) {
        return true;
    }

    ivm::IVFunction *ivFunction = correspondingFunction(instance);
    return ivFunction != nullptr;
}

/*!
   Returns a list of the names of all functions in the iv model
 */
QStringList IvSystemChecks::functionsNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->ivFunctionsNames();
}

/*!
   Checks all messages if they are defined in the IV model as connection
   Returns all messages with their chart, that are not part
 */
QVector<QPair<msc::MscChart *, msc::MscMessage *>> IvSystemChecks::checkMessages() const
{
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> result;
    if (!m_ivCore || !m_mscCore) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscCore->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : qAsConst(charts)) {
        for (msc::MscMessage *message : chart->messages()) {
            if (!checkMessage(message)) {
                result << QPair<msc::MscChart *, msc::MscMessage *>(chart, message);
            }
        }
    }

    return result;
}

/*!
   Checks if the given MSC message has a corresponding iv connection
   @return Returns false, if the message is not part of the iv model
 */
bool IvSystemChecks::checkMessage(const msc::MscMessage *message) const
{
    if (!m_ivCore || !message) {
        return true;
    }

    const QString sourceName = message->sourceInstance() ? message->sourceInstance()->name() : "";
    const QString targetName = message->targetInstance() ? message->targetInstance()->name() : "";
    if (!sourceName.isEmpty() && !targetName.isEmpty()) {
        QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(*ivModel());
        for (const ivm::IVConnectionChain *chain : qAsConst(chains)) {
            if (chain->contains(message->name(), sourceName, targetName)) {
                return true;
            }
        }
    }

    QList<ivm::IVInterface *> interfaces = ivModel()->getIfacesByName(message->name());
    // From the environment
    if (sourceName.isEmpty() && !targetName.isEmpty()) {
        for (ivm::IVInterface *interface : qAsConst(interfaces)) {
            if (auto func = qobject_cast<ivm::IVFunction *>(interface->parent())) {
                if (targetName.compare(func->title(), m_caseCheck) == 0) {
                    if (interface->kind() == ivm::IVInterface::OperationKind::Cyclic) {
                        return true;
                    }
                }
            }
        }
    }
    // To the environment
    if (!sourceName.isEmpty() && targetName.isEmpty()) {
        for (ivm::IVInterface *interface : qAsConst(interfaces)) {
            if (auto func = qobject_cast<ivm::IVFunction *>(interface->parent())) {
                if (sourceName.compare(func->title(), m_caseCheck) == 0) {
                    if (interface->kind() == ivm::IVInterface::OperationKind::Cyclic) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

/*!
   Returns a list of the names of all connections in the iv model
   \sa connectionNamesFromTo
 */
QStringList IvSystemChecks::connectionNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->ivConnectionNames();
}

/*!
   Returns all connections that have \p sourceName as required interface and \p targetName as provided interface
   \note the names are iv encoded by this function
   \sa connectionNames
 */
QStringList IvSystemChecks::connectionNamesFromTo(const QString &sourceName, const QString &targetName) const
{
    if (!m_ivCore) {
        return {};
    }

    QStringList connectionNames;
    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(*ivModel());
    for (const ivm::IVConnectionChain *chain : qAsConst(chains)) {
        const QStringList names = chain->connectionNames(sourceName, targetName);
        connectionNames += names;
    }
    connectionNames.removeDuplicates();
    return connectionNames;
}

/*!
   Returns a pointer to the IV model of the in-core
 */
ivm::IVModel *IvSystemChecks::ivModel() const
{
    if (!m_ivCore) {
        return {};
    }

    if (!m_ivCore->document() || !m_ivCore->document()->objectsModel()) {
        qWarning() << "No IVModel";
        return {};
    }

    return m_ivCore->document()->objectsModel();
}

/*!
   Returns the iv functions that corresponds to the given msc instance
 */
ivm::IVFunction *IvSystemChecks::correspondingFunction(const msc::MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    const QVector<ivm::IVFunction *> functions = m_ivCore->allIVFunctions();
    auto it = std::find_if(functions.cbegin(), functions.cend(),
            [this, &instance](ivm::IVFunction *func) { return correspond(func, instance); });

    if (it == functions.cend()) {
        return nullptr;
    }

    return *it;
}

/*!
   Return true, if the iv object is a function and the msc instance are the same
 */
bool IvSystemChecks::correspond(const ivm::IVObject *ivObj, const msc::MscInstance *instance) const
{
    if (ivObj == nullptr && instance == nullptr) {
        // if both are invalid, it the same
        return true;
    }
    if (ivObj == nullptr) {
        return false;
    }

    if (ivObj->type() == ivm::IVObject::Type::Function) {
        if (auto func = qobject_cast<const ivm::IVFunction *>(ivObj)) {
            return correspond(func, instance);
        }
    }
    return false;
}

/*!
   Return true, if the iv function and the msc instance are the same
 */
bool IvSystemChecks::correspond(const ivm::IVFunction *ivFunc, const msc::MscInstance *instance) const
{
    if (ivFunc == nullptr && instance == nullptr) {
        // if both are invalid, it the same
        return true;
    }
    if (ivFunc == nullptr || instance == nullptr) {
        // if only one is invalid, it's not ok
        return false;
    }

    return instance->name().compare(ivFunc->title(), m_caseCheck) == 0;
}

/*!
   Returns if the given iv function \p func has an ancestor (is nested by) one of the functions
 */
bool IvSystemChecks::hasAncestor(ivm::IVFunction *func, const QVector<ivm::IVFunction *> allFunctions) const
{
    for (ivm::IVFunction *f : allFunctions) {
        if (isAncestor(func, f)) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the given iv function \p func has an descendant (is nesting by) at least one of the functions
 */
bool IvSystemChecks::hasDescendant(ivm::IVFunction *func, const QVector<ivm::IVFunction *> allFunctions) const
{
    for (ivm::IVFunction *f : allFunctions) {
        if (isAncestor(f, func)) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the given iv function \p otherFunc is an ancestor (parent, grand-parent, ...) of the function \p func
 */
bool IvSystemChecks::isAncestor(ivm::IVFunction *func, ivm::IVFunction *otherFunc) const
{
    if (!func || !otherFunc || func == otherFunc) {
        return false;
    }

    auto checkObj = dynamic_cast<ivm::IVObject *>(func->parent());
    while (checkObj) {
        if (checkObj == otherFunc) {
            return true;
        }
        checkObj = dynamic_cast<ivm::IVObject *>(checkObj->parent());
    }

    return false;
}

/**
   Returns if the iv connection and the msc message are the same
 */
bool IvSystemChecks::correspond(const ivm::IVConnection *connection, const msc::MscMessage *message) const
{
    bool nameOk = true;
    if (!connection->name().isEmpty()) {
        nameOk &= message->name().compare(connection->name(), m_caseCheck) == 0;
    }
    return nameOk && correspond(connection->source(), message->sourceInstance())
            && correspond(connection->target(), message->targetInstance());
}

/*!
   Returns all iv connections as msc message declarations
 */
QVector<msc::MscMessageDeclaration *> IvSystemChecks::allConnectionsAsDeclaration() const
{
    QVector<msc::MscMessageDeclaration *> result;

    for (ivm::IVConnection *connection : m_ivCore->allIVConnections()) {
        auto declaration = new msc::MscMessageDeclaration();
        declaration->setNames({ connection->name() });
        QStringList params;
        for (ivm::InterfaceParameter p : connection->params()) {
            params.append(p.paramTypeName());
        }
        declaration->setTypeRefList(params);
        result.append(declaration);
    }

    return result;
}

/*!
   Returns if strings comparisons fo IV and MSC string should be case sensitive
 */
Qt::CaseSensitivity IvSystemChecks::stringSensitivity() const
{
    return m_caseCheck;
}

/*!
   Returns if the IV model has be set
 */
bool IvSystemChecks::hasValidSystem() const
{
    return !m_ivCore.isNull();
}

}
