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

#include "aadlsystemchecks.h"

#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
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

namespace msc {

AadlSystemChecks::AadlSystemChecks(QObject *parent)
    : QObject((parent))
{
}

AadlSystemChecks::~AadlSystemChecks() { }

void AadlSystemChecks::setMscCore(MSCEditorCore *mscCore)
{
    m_mscCore = mscCore;
}

void AadlSystemChecks::setIvCore(QSharedPointer<aadlinterface::IVEditorCore> ivCore)
{
    if (ivCore == m_ivCore) {
        return;
    }

    if (aadl::AADLObjectsModel *model = aadlModel()) {
        disconnect(model, nullptr, this, nullptr);
    }

    m_ivCore = ivCore;
    Q_EMIT ivCoreChanged();
}

/*!
   Returns a pointer to the IV editor model
 */
const QSharedPointer<aadlinterface::IVEditorCore> &AadlSystemChecks::ivCore() const
{
    return m_ivCore;
}

/*!
   Returns if the IV model has be set
 */
bool AadlSystemChecks::hasIvCore() const
{
    return !m_ivCore.isNull();
}

/*!
   Checks all instances if they are defined in the IV model as function
 */
QVector<QPair<MscChart *, MscInstance *>> AadlSystemChecks::checkInstanceNames() const
{
    QVector<QPair<MscChart *, MscInstance *>> result;
    if (!m_ivCore || !m_mscCore) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscCore->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : charts) {
        for (msc::MscInstance *instance : chart->instances()) {
            aadl::AADLObjectFunction *aadlFunction = correspondingFunction(instance);
            if (!aadlFunction) {
                result << QPair<MscChart *, MscInstance *>(chart, instance);
            }
        }
    }

    return result;
}

/*!
   Checks if in a chart instances with parent/child relations re used
 */
QVector<QPair<MscChart *, MscInstance *>> AadlSystemChecks::checkInstanceRelations() const
{
    QVector<QPair<MscChart *, MscInstance *>> result;
    if (!m_ivCore || !m_mscCore) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscCore->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : charts) {
        QVector<QPair<MscInstance *, aadl::AADLObjectFunction *>> pairs;
        for (msc::MscInstance *instance : chart->instances()) {
            aadl::AADLObjectFunction *aadlFunction = correspondingFunction(instance);
            if (aadlFunction) {
                pairs << QPair<MscInstance *, aadl::AADLObjectFunction *>(instance, aadlFunction);
            }
        }

        QVector<aadl::AADLObjectFunction *> aadlChartFunctions;
        for (QPair<MscInstance *, aadl::AADLObjectFunction *> pair : pairs) {
            aadlChartFunctions.append(pair.second);
        }
        for (QPair<MscInstance *, aadl::AADLObjectFunction *> pair : pairs) {
            if (hasAncestor(pair.second, aadlChartFunctions) || hasDescendant(pair.second, aadlChartFunctions)) {
                result << QPair<MscChart *, MscInstance *>(chart, pair.first);
            }
        }
    }

    return result;
}

/*!
   Checks if the given MSC instance has a corresponding aadl function
 */
bool AadlSystemChecks::checkInstance(const MscInstance *instance) const
{
    if (!m_ivCore) {
        return true;
    }

    aadl::AADLObjectFunction *aadlFunction = correspondingFunction(instance);
    return aadlFunction != nullptr;
}

/*!
   Returns a list of the names of all functions in the aadl model
 */
QStringList AadlSystemChecks::functionsNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->aadlFunctionsNames();
}

/*!
   Checks all messages if they are defined in the IV model as connection
 */
QVector<QPair<MscChart *, MscMessage *>> AadlSystemChecks::checkMessages() const
{
    QVector<QPair<MscChart *, MscMessage *>> result;
    if (!m_ivCore || !m_mscCore) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscCore->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : charts) {
        for (msc::MscInstanceEvent *event : chart->instanceEvents()) {
            if (auto message = qobject_cast<msc::MscMessage *>(event)) {
                aadl::AADLObjectConnection *aadlConnection = correspondingConnection(message);
                if (!aadlConnection) {
                    result << QPair<MscChart *, MscMessage *>(chart, message);
                }
            }
        }
    }

    return result;
}

/*!
   Checks if the given MSC message has a corresponding aadl connection
 */
bool AadlSystemChecks::checkMessage(const MscMessage *message) const
{
    if (!m_ivCore) {
        return true;
    }

    aadl::AADLObjectConnection *aadlConnection = correspondingConnection(message);
    return aadlConnection != nullptr;
}

/*!
   Returns a list of the names of all connections in the aadl model
   \sa connectionNamesFromTo
 */
QStringList AadlSystemChecks::connectionNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->aadlConnectionNames();
}

bool AadlSystemChecks::connectionExists(const QString &name, const QString &sourceName, const QString &targetName) const
{
    aadl::AADLObjectsModel *model = aadlModel();
    if (model == nullptr) {
        return true;
    }

    return model->getConnection(name, sourceName, targetName, m_caseCheck) != nullptr;
}

/*!
   Returns all connections that have \p sourceName as required interface and \p targetName as providedinterface
   \note the names are aadl encoded by this function
   \sa connectionNames
 */
QStringList AadlSystemChecks::connectionNamesFromTo(const QString &sourceName, const QString &targetName) const
{
    if (!m_ivCore) {
        return {};
    }

    const QVector<aadl::AADLObjectConnection *> connections = m_ivCore->allAadlConnections();
    QStringList connectionNames;
    for (const aadl::AADLObjectConnection *aadlConnection : connections) {
        if (aadlConnection && !aadlConnection->targetInterfaceName().isEmpty()) {
            if (aadlConnection->sourceName().compare(sourceName, m_caseCheck) == 0
                    && aadlConnection->targetName().compare(targetName, m_caseCheck) == 0) {
                connectionNames << aadlConnection->targetInterfaceName();
            }
        }
    }
    connectionNames.removeDuplicates();
    return connectionNames;
}

aadl::AADLObjectsModel *AadlSystemChecks::aadlModel() const
{
    if (!m_ivCore) {
        return {};
    }

    if (!m_ivCore->document() || !m_ivCore->document()->objectsModel()) {
        qWarning() << "No AADLObjectsModel";
        return {};
    }

    return m_ivCore->document()->objectsModel();
}

/*!
   Returns the aadl functions that corresponds to the given msc instance
 */
aadl::AADLObjectFunction *AadlSystemChecks::correspondingFunction(const MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    const QVector<aadl::AADLObjectFunction *> functions = m_ivCore->allAadlFunctions();
    auto it = std::find_if(functions.cbegin(), functions.cend(),
            [this, &instance](aadl::AADLObjectFunction *func) { return correspond(func, instance); });

    if (it == functions.cend()) {
        return nullptr;
    }

    return *it;
}

/*!
   Return true, if the aadl object is a function and the msc instance are the same
 */
bool AadlSystemChecks::correspond(const aadl::AADLObject *aadlObj, const MscInstance *instance) const
{
    if (aadlObj == nullptr && instance == nullptr) {
        // if both are invalid, it the same
        return true;
    }
    if (aadlObj == nullptr) {
        return false;
    }

    if (aadlObj->aadlType() == aadl::AADLObject::Type::Function) {
        if (auto func = qobject_cast<const aadl::AADLObjectFunction *>(aadlObj)) {
            return correspond(func, instance);
        }
    }
    return false;
}

/*!
   Return true, if the aadl function and the msc instance are the same
 */
bool AadlSystemChecks::correspond(const aadl::AADLObjectFunction *aadlFunc, const MscInstance *instance) const
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
   Returns if the given aadl function \p func has an ancestor (is nesed by) one of the functions
 */
bool AadlSystemChecks::hasAncestor(
        aadl::AADLObjectFunction *func, const QVector<aadl::AADLObjectFunction *> allFunctions) const
{
    for (aadl::AADLObjectFunction *f : allFunctions) {
        if (isAncestor(func, f)) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the given aadl function \p func has an descendant (is nesting by) at least one of the functions
 */
bool AadlSystemChecks::hasDescendant(
        aadl::AADLObjectFunction *func, const QVector<aadl::AADLObjectFunction *> allFunctions) const
{
    for (aadl::AADLObjectFunction *f : allFunctions) {
        if (isAncestor(f, func)) {
            return true;
        }
    }
    return false;
}

/*!
   Returns if the given aadl function \p otherFunc is an ancestor (parent, grand-parent, ...) of the function \p func
 */
bool AadlSystemChecks::isAncestor(aadl::AADLObjectFunction *func, aadl::AADLObjectFunction *otherFunc) const
{
    if (!func || !otherFunc || func == otherFunc) {
        return false;
    }

    auto checkObj = dynamic_cast<aadl::AADLObject *>(func->parent());
    while (checkObj) {
        if (checkObj == otherFunc) {
            return true;
        }
        checkObj = dynamic_cast<aadl::AADLObject *>(checkObj->parent());
    }

    return false;
}

/*!
   Returns the cooresponding aadl connection for the given \p message.
   If no such connection exists, a nullptr is returned.
 */
aadl::AADLObjectConnection *AadlSystemChecks::correspondingConnection(const MscMessage *message) const
{
    if (!message) {
        return nullptr;
    }

    const QVector<aadl::AADLObjectConnection *> connections = m_ivCore->allAadlConnections();
    auto it = std::find_if(connections.cbegin(), connections.cend(),
            [this, &message](aadl::AADLObjectConnection *connection) { return correspond(connection, message); });

    if (it == connections.cend()) {
        return nullptr;
    }

    return *it;
}

/**
   Returns if the aadl connection and the msc message are the same
 */
bool AadlSystemChecks::correspond(const aadl::AADLObjectConnection *connection, const MscMessage *message) const
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
QVector<MscMessageDeclaration *> AadlSystemChecks::allConnectionsAsDeclaration() const
{
    QVector<MscMessageDeclaration *> result;

    for (aadl::AADLObjectConnection *connection : m_ivCore->allAadlConnections()) {
        auto declaration = new MscMessageDeclaration();
        declaration->setNames({ connection->name() });
        QStringList params;
        for (aadl::IfaceParameter p : connection->params()) {
            params.append(p.paramTypeName());
        }
        declaration->setTypeRefList(params);
        result.append(declaration);
    }

    return result;
}

}
