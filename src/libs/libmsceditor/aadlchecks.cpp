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

#include "aadlchecks.h"

#include "aadlnamevalidator.h"
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
#include "mscmodel.h"

#include <QDebug>
#include <algorithm>

namespace msc {

AadlChecks::AadlChecks(QObject *parent)
    : QObject((parent))
{
}

AadlChecks::~AadlChecks() { }

void AadlChecks::setMscPlugin(MSCEditorCore *mscPlugin)
{
    m_mscPlugin = mscPlugin;
}

void AadlChecks::setIvPlugin(QSharedPointer<aadlinterface::IVEditorCore> ivPlugin)
{
    if (ivPlugin == m_ivPlugin) {
        return;
    }

    if (aadl::AADLObjectsModel *model = aadlModel()) {
        disconnect(model, nullptr, this, nullptr);
    }

    m_ivPlugin = ivPlugin;
    updateAadlItems();

    if (aadl::AADLObjectsModel *model = aadlModel()) {
        connect(model, &aadl::AADLObjectsModel::aadlObjectAdded, this, &msc::AadlChecks::updateAadlItems);
        connect(model, &aadl::AADLObjectsModel::aadlObjectRemoved, this, &msc::AadlChecks::updateAadlItems);
    }

    Q_EMIT ivPluginChanged();
}

/*!
   Checks all instances if they are defined in the IV model as function
 */
QVector<QPair<MscChart *, MscInstance *>> AadlChecks::checkInstanceNames() const
{
    QVector<QPair<MscChart *, MscInstance *>> result;
    if (!m_ivPlugin || !m_mscPlugin) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscPlugin->mainModel()->mscModel()->allCharts();
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
QVector<QPair<MscChart *, MscInstance *>> AadlChecks::checkInstanceRelations() const
{
    QVector<QPair<MscChart *, MscInstance *>> result;
    if (!m_ivPlugin || !m_mscPlugin) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscPlugin->mainModel()->mscModel()->allCharts();
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
   Checks ich the given MSC insatnce has a corresponding aadl function
 */
bool AadlChecks::checkInstance(const MscInstance *instance) const
{
    if (!m_ivPlugin) {
        return true;
    }

    aadl::AADLObjectFunction *aadlFunction = correspondingFunction(instance);
    return aadlFunction != nullptr;
}

/*!
   Checks all messages if they are defined in the IV model as connection
 */
QVector<QPair<MscChart *, MscMessage *>> AadlChecks::checkMessages() const
{
    QVector<QPair<MscChart *, MscMessage *>> result;
    if (!m_ivPlugin || !m_mscPlugin) {
        return result;
    }

    QVector<msc::MscChart *> charts = m_mscPlugin->mainModel()->mscModel()->allCharts();
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

aadl::AADLObjectsModel *AadlChecks::aadlModel() const
{
    if (!m_ivPlugin) {
        return {};
    }

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin = m_ivPlugin.toStrongRef();
    if (!ivPlugin->document() || !ivPlugin->document()->objectsModel()) {
        qWarning() << "No AADLObjectsModel";
        return {};
    }

    return ivPlugin->document()->objectsModel();
}

/*!
   Updates the list of functions from the aadl model
 */
void AadlChecks::updateAadlItems()
{
    m_aadlFunctions.clear();
    m_aadlConnections.clear();

    aadl::AADLObjectsModel *aadlModel = this->aadlModel();
    if (!aadlModel) {
        return;
    }

    const QHash<shared::Id, aadl::AADLObject *> &aadlObjects = aadlModel->objects();
    for (auto obj : aadlObjects) {
        if (obj->aadlType() == aadl::AADLObject::Type::Function) {
            if (auto func = dynamic_cast<aadl::AADLObjectFunction *>(obj)) {
                m_aadlFunctions.append(func);
            }
        }
        if (obj->aadlType() == aadl::AADLObject::Type::Connection) {
            if (auto func = dynamic_cast<aadl::AADLObjectConnection *>(obj)) {
                m_aadlConnections.append(func);
            }
        }
    }
}

/*!
   Returns the aadl functions that correlates to the given msc instance
 */
aadl::AADLObjectFunction *AadlChecks::correspondingFunction(const MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    auto it = std::find_if(m_aadlFunctions.begin(), m_aadlFunctions.end(),
            [this, &instance](aadl::AADLObjectFunction *func) { return correspond(func, instance); });

    if (it == m_aadlFunctions.end()) {
        return nullptr;
    }

    return *it;
}

/*!
   Return true, if the aadl object is a function and the msc instance are the same
 */
bool AadlChecks::correspond(const aadl::AADLObject *aadlObj, const MscInstance *instance) const
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
bool AadlChecks::correspond(const aadl::AADLObjectFunction *aadlFunc, const MscInstance *instance) const
{
    if (aadlFunc == nullptr && instance == nullptr) {
        // if both are invalid, it the same
        return true;
    }
    if (aadlFunc == nullptr || instance == nullptr) {
        // if only one is invalid, it's not ok
        return false;
    }

    const QString instanceName =
            aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Function, instance->name());
    return instanceName == aadlFunc->title();
}

/*!
   Returns if the given aadl function \p func has an ancestor (is nesed by) one of the functions
 */
bool AadlChecks::hasAncestor(
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
bool AadlChecks::hasDescendant(
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
bool AadlChecks::isAncestor(aadl::AADLObjectFunction *func, aadl::AADLObjectFunction *otherFunc) const
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
aadl::AADLObjectConnection *AadlChecks::correspondingConnection(MscMessage *message) const
{
    if (!message) {
        return nullptr;
    }

    auto it = std::find_if(m_aadlConnections.begin(), m_aadlConnections.end(),
            [this, &message](aadl::AADLObjectConnection *connection) { return correspond(connection, message); });

    if (it == m_aadlConnections.end()) {
        return nullptr;
    }

    return *it;
}

/**
   Returns if the aadl connection and the msc message are the same
 */
bool AadlChecks::correspond(const aadl::AADLObjectConnection *connection, const MscMessage *message) const
{
    const QString messageName =
            aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Connection, message->name());
    bool nameOk = true;
    if (!connection->targetInterfaceName().isEmpty()) {
        nameOk &= messageName == connection->targetInterfaceName();
    }
    return correspond(connection->source(), message->sourceInstance())
            && correspond(connection->target(), message->targetInstance()) && nameOk;
}

}
