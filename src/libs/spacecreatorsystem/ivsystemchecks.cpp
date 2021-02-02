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

#include "aadlconnection.h"
#include "aadlconnectionchain.h"
#include "aadlfunction.h"
#include "aadlmodel.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
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

    if (ivm::AADLModel *model = aadlModel()) {
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
            ivm::AADLFunction *aadlFunction = correspondingFunction(instance);
            if (!aadlFunction) {
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
        QVector<QPair<msc::MscInstance *, ivm::AADLFunction *>> pairs;
        for (msc::MscInstance *instance : chart->instances()) {
            ivm::AADLFunction *aadlFunction = correspondingFunction(instance);
            if (aadlFunction) {
                pairs << QPair<msc::MscInstance *, ivm::AADLFunction *>(instance, aadlFunction);
            }
        }

        QVector<ivm::AADLFunction *> aadlChartFunctions;
        for (QPair<msc::MscInstance *, ivm::AADLFunction *> pair : pairs) {
            aadlChartFunctions.append(pair.second);
        }
        for (QPair<msc::MscInstance *, ivm::AADLFunction *> pair : pairs) {
            if (hasAncestor(pair.second, aadlChartFunctions) || hasDescendant(pair.second, aadlChartFunctions)) {
                result << QPair<msc::MscChart *, msc::MscInstance *>(chart, pair.first);
            }
        }
    }

    return result;
}

/*!
   Checks if the given MSC instance has a corresponding aadl function
 */
bool IvSystemChecks::checkInstance(const msc::MscInstance *instance) const
{
    if (!m_ivCore) {
        return true;
    }

    ivm::AADLFunction *aadlFunction = correspondingFunction(instance);
    return aadlFunction != nullptr;
}

/*!
   Returns a list of the names of all functions in the aadl model
 */
QStringList IvSystemChecks::functionsNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->aadlFunctionsNames();
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
        for (msc::MscInstanceEvent *event : chart->instanceEvents()) {
            if (auto message = qobject_cast<msc::MscMessage *>(event)) {
                if (!checkMessage(message)) {
                    result << QPair<msc::MscChart *, msc::MscMessage *>(chart, message);
                }
            }
        }
    }

    return result;
}

/*!
   Checks if the given MSC message has a corresponding aadl connection
   @return Returns false, if the message is not part of the aadl model
 */
bool IvSystemChecks::checkMessage(const msc::MscMessage *message) const
{
    if (!m_ivCore || !message) {
        return true;
    }

    const QString sourceName = message->sourceInstance() ? message->sourceInstance()->name() : "";
    const QString targetName = message->targetInstance() ? message->targetInstance()->name() : "";
    if (!sourceName.isEmpty() && !targetName.isEmpty()) {
        QList<ivm::AADLConnectionChain *> chains = ivm::AADLConnectionChain::build(*aadlModel());
        for (const ivm::AADLConnectionChain *chain : qAsConst(chains)) {
            if (chain->contains(message->name(), sourceName, targetName)) {
                return true;
            }
        }
    }

    QList<ivm::AADLIface *> interfaces = aadlModel()->getIfacesByName(message->name());
    // From the environment
    if (sourceName.isEmpty() && !targetName.isEmpty()) {
        for (ivm::AADLIface *interface : qAsConst(interfaces)) {
            if (auto func = qobject_cast<ivm::AADLFunction *>(interface->parent())) {
                if (targetName.compare(func->title(), m_caseCheck) == 0) {
                    if (interface->kind() == ivm::AADLIface::OperationKind::Cyclic) {
                        return true;
                    }
                }
            }
        }
    }
    // To the environment
    if (!sourceName.isEmpty() && targetName.isEmpty()) {
        for (ivm::AADLIface *interface : qAsConst(interfaces)) {
            if (auto func = qobject_cast<ivm::AADLFunction *>(interface->parent())) {
                if (sourceName.compare(func->title(), m_caseCheck) == 0) {
                    if (interface->kind() == ivm::AADLIface::OperationKind::Cyclic) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

/*!
   Returns a list of the names of all connections in the aadl model
   \sa connectionNamesFromTo
 */
QStringList IvSystemChecks::connectionNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->aadlConnectionNames();
}

/*!
   Returns all connections that have \p sourceName as required interface and \p targetName as provided interface
   \note the names are aadl encoded by this function
   \sa connectionNames
 */
QStringList IvSystemChecks::connectionNamesFromTo(const QString &sourceName, const QString &targetName) const
{
    if (!m_ivCore) {
        return {};
    }

    QStringList connectionNames;
    QList<ivm::AADLConnectionChain *> chains = ivm::AADLConnectionChain::build(*aadlModel());
    for (const ivm::AADLConnectionChain *chain : qAsConst(chains)) {
        const QStringList names = chain->connectionNames(sourceName, targetName);
        connectionNames += names;
    }
    connectionNames.removeDuplicates();
    return connectionNames;
}

/*!
   Returns a pointer to the AADL model of the in-core
 */
ivm::AADLModel *IvSystemChecks::aadlModel() const
{
    if (!m_ivCore) {
        return {};
    }

    if (!m_ivCore->document() || !m_ivCore->document()->objectsModel()) {
        qWarning() << "No AADLModel";
        return {};
    }

    return m_ivCore->document()->objectsModel();
}

/*!
   Returns the aadl functions that corresponds to the given msc instance
 */
ivm::AADLFunction *IvSystemChecks::correspondingFunction(const msc::MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    const QVector<ivm::AADLFunction *> functions = m_ivCore->allAadlFunctions();
    auto it = std::find_if(functions.cbegin(), functions.cend(),
            [this, &instance](ivm::AADLFunction *func) { return correspond(func, instance); });

    if (it == functions.cend()) {
        return nullptr;
    }

    return *it;
}

/*!
   Return true, if the aadl object is a function and the msc instance are the same
 */
bool IvSystemChecks::correspond(const ivm::AADLObject *aadlObj, const msc::MscInstance *instance) const
{
    if (aadlObj == nullptr && instance == nullptr) {
        // if both are invalid, it the same
        return true;
    }
    if (aadlObj == nullptr) {
        return false;
    }

    if (aadlObj->aadlType() == ivm::AADLObject::Type::Function) {
        if (auto func = qobject_cast<const ivm::AADLFunction *>(aadlObj)) {
            return correspond(func, instance);
        }
    }
    return false;
}

/*!
   Return true, if the aadl function and the msc instance are the same
 */
bool IvSystemChecks::correspond(const ivm::AADLFunction *aadlFunc, const msc::MscInstance *instance) const
{
    if (aadlFunc == nullptr && instance == nullptr) {
        // if both are invalid, it the same
        return true;
    }
    if (aadlFunc == nullptr || instance == nullptr) {
        // if only one is invalid, it's not ok
        return false;
    }

    return instance->name().compare(aadlFunc->title(), m_caseCheck) == 0;
}

/*!
   Returns if the given aadl function \p func has an ancestor (is nested by) one of the functions
 */
bool IvSystemChecks::hasAncestor(ivm::AADLFunction *func, const QVector<ivm::AADLFunction *> allFunctions) const
{
    for (ivm::AADLFunction *f : allFunctions) {
        if (isAncestor(func, f)) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the given aadl function \p func has an descendant (is nesting by) at least one of the functions
 */
bool IvSystemChecks::hasDescendant(ivm::AADLFunction *func, const QVector<ivm::AADLFunction *> allFunctions) const
{
    for (ivm::AADLFunction *f : allFunctions) {
        if (isAncestor(f, func)) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the given aadl function \p otherFunc is an ancestor (parent, grand-parent, ...) of the function \p func
 */
bool IvSystemChecks::isAncestor(ivm::AADLFunction *func, ivm::AADLFunction *otherFunc) const
{
    if (!func || !otherFunc || func == otherFunc) {
        return false;
    }

    auto checkObj = dynamic_cast<ivm::AADLObject *>(func->parent());
    while (checkObj) {
        if (checkObj == otherFunc) {
            return true;
        }
        checkObj = dynamic_cast<ivm::AADLObject *>(checkObj->parent());
    }

    return false;
}

/**
   Returns if the aadl connection and the msc message are the same
 */
bool IvSystemChecks::correspond(const ivm::AADLConnection *connection, const msc::MscMessage *message) const
{
    bool nameOk = true;
    if (!connection->name().isEmpty()) {
        nameOk &= message->name().compare(connection->name(), m_caseCheck) == 0;
    }
    return nameOk && correspond(connection->source(), message->sourceInstance())
            && correspond(connection->target(), message->targetInstance());
}

/*!
   Returns all aadl connections as msc message declarations
 */
QVector<msc::MscMessageDeclaration *> IvSystemChecks::allConnectionsAsDeclaration() const
{
    QVector<msc::MscMessageDeclaration *> result;

    for (ivm::AADLConnection *connection : m_ivCore->allAadlConnections()) {
        auto declaration = new msc::MscMessageDeclaration();
        declaration->setNames({ connection->name() });
        QStringList params;
        for (ivm::IfaceParameter p : connection->params()) {
            params.append(p.paramTypeName());
        }
        declaration->setTypeRefList(params);
        result.append(declaration);
    }

    return result;
}

/*!
   Returns if strings comparisons fo AADL and MSC string should be case sensitive
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
