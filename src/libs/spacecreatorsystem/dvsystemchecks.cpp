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

#include "dvsystemchecks.h"

#include "dvappmodel.h"
#include "dveditorcore.h"
#include "dvfunction.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "dvsystemfunction.h"
#include "dvsysteminterface.h"
#include "errorhub.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivsystemqueries.h"
#include "spacecreatorproject.h"

#include <QDebug>
#include <algorithm>

namespace scs {

DvSystemChecks::DvSystemChecks(QObject *parent)
    : QObject(parent)
{
}

void DvSystemChecks::setStorage(SpaceCreatorProject *storage)
{
    m_storage = storage;
}

/*!
   Do all check for the given file
 */
bool DvSystemChecks::checkDVFile(DVEditorCorePtr dvCore) const
{
    bool ok = checkFunctionBindings(dvCore);
    ok = ok && checkMessageBindings(dvCore);
    return ok;
}

/*!
   Checks if all function bindings in all dv files are valid
 */
bool DvSystemChecks::checkFunctionBindings() const
{
    bool ok = true;
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        ok = ok && checkFunctionBindings(dvCore);
    }
    return ok;
}

/*!
   Perform function binding check for one DVfile
 */
bool DvSystemChecks::checkFunctionBindings(DVEditorCorePtr dvCore) const
{
    bool ok = true;
    ok = ok && checkFunctionIvValidity(dvCore);
    ok = ok && checkUniqueFunctionBindings(dvCore);
    ok = ok && checkSystemFunctionsAvailable(dvCore);
    ok = ok && checkProtectedFunctions(dvCore);
    return ok;
}

bool DvSystemChecks::checkMessageBindings() const
{
    bool ok = true;
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        ok = ok && checkMessageBindings(dvCore);
    }
    return ok;
}

bool DvSystemChecks::checkMessageBindings(DVEditorCorePtr dvCore) const
{
    bool ok = true;
    ok = ok && checkMessageIvValidity(dvCore);
    ok = ok && checkUniqueMessages(dvCore);
    return ok;
}

/*!
   Returns the corresponding IV function for \p dvFunc
   If none is found, nullptr is returned
 */
ivm::IVFunction *DvSystemChecks::correspondingFunction(dvm::DVFunction *dvFunc) const
{
    if (!m_storage && m_storage->ivQuery()) {
        return nullptr;
    }

    return m_storage->ivQuery()->functionByName(dvFunc->title());

    return nullptr;
}

/*!
   Returns the corresponding DV function for \p ivFunc in the DV editor \p dvCore
   If none is found, nullptr is returned
 */
dvm::DVFunction *DvSystemChecks::correspondingFunction(ivm::IVFunction *ivFunc, const DVEditorCorePtr &dvCore) const
{
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    dvm::DVObject *dvf = model->getObjectByName(ivFunc->title(), dvm::DVObject::Type::Function);
    return qobject_cast<dvm::DVFunction *>(dvf);
}

bool DvSystemChecks::checkFunctionIvValidity(const DVEditorCorePtr &dvCore) const
{
    if (!m_storage && m_storage->ivQuery()) {
        return true;
    }
    scs::IvSystemQueries *ivQuery = m_storage->ivQuery();
    QStringList ivFunctionNames = ivQuery->functionsNames();
    QStringList systemFunctionNames = ivQuery->pseudoFunctionsNames();

    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVFunction *> functions = model->allObjectsByType<dvm::DVFunction>();
    for (dvm::DVFunction *f : functions) {
        const QString fname = f->title();
        if (!ivFunctionNames.contains(fname, m_caseCheck)) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Function binding '%1' is not available in the interface view").arg(fname), dvCore->filePath());
        }
        if (systemFunctionNames.contains(fname, m_caseCheck)) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Function binding '%1' is a pseudo/system function").arg(fname), dvCore->filePath());
        }
    }

    return ok;
}

bool DvSystemChecks::checkUniqueFunctionBindings(const DVEditorCorePtr &dvCore) const
{
    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVFunction *> functions = model->allObjectsByType<dvm::DVFunction>();
    QStringList duplicateNames;
    for (dvm::DVFunction *f : functions) {
        const QString fTitle = f->title();
        // Check is DV functions are bound uniquely
        int count = std::count_if(functions.begin(), functions.end(),
                [f, &fTitle](dvm::DVFunction *cf) { return fTitle == cf->title() && f != cf; });
        if (count > 0 && !duplicateNames.contains(fTitle)) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Function binding '%1' is used more than once").arg(fTitle), dvCore->filePath());
            duplicateNames.append(f->title());
        }
    }

    return ok;
}

bool DvSystemChecks::checkSystemFunctionsAvailable(const DVEditorCorePtr &dvCore) const
{
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVNode *> nodes = model->allObjectsByType<dvm::DVNode>();
    bool ok = true;
    for (dvm::DVNode *node : nodes) {
        ok &= checkSystemFunctionsAvailable(node, dvCore);
    }

    return ok;
}

/*!
   Checks for all bound functions, that are connected to a pseudo function (in IV), if a matching system function is
   available in this node
 */
bool DvSystemChecks::checkSystemFunctionsAvailable(const dvm::DVNode *node, const DVEditorCorePtr &dvCore) const
{
    if (!m_storage && m_storage->ivQuery()) {
        return true;
    }

    IvSystemQueries *ivQuery = m_storage->ivQuery();
    bool ok = true;
    QStringList boundFunctions;
    for (dvm::DVPartition *partition : node->partitions()) {
        boundFunctions += partition->functionsNames();
    }

    QList<ivm::IVFunction *> boundSystemFunctions;
    for (const QString &func : boundFunctions) {
        boundSystemFunctions += ivQuery->connectedPseudoFunctions(func);
    }

    // check if functions exist
    for (ivm::IVFunction *func : boundSystemFunctions) {
        dvm::DVSystemFunction *sysFunc = node->systemFunction(func->title());
        if (!sysFunc) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Node '%1' misses a pseudo function '%2'").arg(node->title(), func->title()),
                    dvCore->filePath());
            continue;
        }
        // Check if interfaces match
        for (ivm::IVInterface *iface : func->allInterfaces()) {
            bool foundMatch = false;
            for (dvm::DVSystemInterface *sysFace : sysFunc->interfaces()) {
                if (sysFace->title().compare(iface->title()) == 0) {
                    // check parameters
                    if (sysFace->params() != iface->params()) {
                        ok = false;
                        shared::ErrorHub::addError(shared::ErrorItem::Error,
                                tr("Parameters of interface '%1'.'%2'.'%3' does not match")
                                        .arg(node->title(), func->title(), sysFace->title()),
                                dvCore->filePath());
                    }
                    foundMatch = true;
                    break;
                }
            }
            if (!foundMatch) {
                ok = false;
                shared::ErrorHub::addError(shared::ErrorItem::Error,
                        tr("Node '%1' misses interface '%2' in pseudo function '%3'")
                                .arg(node->title(), iface->title(), func->title()),
                        dvCore->filePath());
            }
        }
    }

    return ok;
}

/*!
   Checks that functions that are bound in one partition, that have other functions connected protected interfaces have
   to be in the same partition
 */
bool DvSystemChecks::checkProtectedFunctions(const DVEditorCorePtr &dvCore) const
{
    if (!m_storage && m_storage->ivQuery()) {
        return true;
    }
    IvSystemQueries *ivQuery = m_storage->ivQuery();

    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVFunction *> functions = model->allObjectsByType<dvm::DVFunction>();
    for (dvm::DVFunction *dvf : functions) {
        dvm::DVPartition *partition = dvf->partition();
        if (!partition) {
            continue;
        }
        // Get all connected "protected" functions
        QList<ivm::IVFunction *> protectedFunctions = ivQuery->connectedProtectedFunctions(dvf->title());
        QList<dvm::DVFunction *> boundFunctions;
        for (ivm::IVFunction *ivf : protectedFunctions) {
            dvm::DVFunction *f = correspondingFunction(ivf, dvCore);
            if (f) {
                boundFunctions.append(f);
            } else {
                dvm::DVNode *node = partition->node();
                if (!node || !node->hasSystemFunction(ivf->title())) {
                    shared::ErrorHub::addError(shared::ErrorItem::Error,
                            tr("Bound protected function '%1' misses '%2'").arg(dvf->title(), ivf->title()),
                            dvCore->filePath());
                    ok = false;
                }
            }
        }

        // check if the functions are in the same partition
        for (dvm::DVFunction *f : boundFunctions) {
            if (f->partition() != partition) {
                shared::ErrorHub::addError(shared::ErrorItem::Error,
                        tr("Bound protected function '%1' is not in partition '%2' like '%3'")
                                .arg(f->title(), partition->title(), dvf->title()),
                        dvCore->filePath());
                ok = false;
            }
        }
    }

    return ok;
}

bool DvSystemChecks::checkMessageIvValidity(const DVEditorCorePtr &dvCore) const
{
    if (!m_storage && m_storage->ivQuery()) {
        return true;
    }

    IvSystemQueries *ivQuery = m_storage->ivQuery();
    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVMessage *> messages = model->allObjectsByType<dvm::DVMessage>();
    for (dvm::DVMessage *msg : messages) {
        if (!ivQuery->connectionExists(
                    msg->fromFunction(), msg->fromInterface(), msg->toFunction(), msg->toInterface())) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Message binding %1.%2 -> %3.%4 is not available in the interface view")
                            .arg(msg->fromFunction(), msg->fromInterface(), msg->toFunction(), msg->toInterface()),
                    dvCore->filePath());
        }
    }

    return ok;
}

bool DvSystemChecks::checkUniqueMessages(const DVEditorCorePtr &dvCore) const
{
    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVMessage *> messages = model->allObjectsByType<dvm::DVMessage>();
    for (dvm::DVMessage *msg : messages) {
        int count = std::count_if(messages.begin(), messages.end(), [msg](dvm::DVMessage *m) {
            return m->isEqual(msg) && m->fromNode() == msg->fromNode() && m->toNode() == msg->toNode();
        });
        if (count > 1) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Message binding %1.%2 -> %3.%4 more than once")
                            .arg(msg->fromFunction(), msg->fromInterface(), msg->toFunction(), msg->toInterface()),
                    dvCore->filePath());
        }
    }
    return ok;
}

} // namespace scs
