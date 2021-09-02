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
#include "dvmodel.h"
#include "errorhub.h"
#include "ivsystemqueries.h"
#include "spacecreatorproject.h"

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
    return checkFunctionBindings(dvCore);
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
    ok = ok && checkMessageIvValidity(dvCore);
    ok = ok && checkUniqueMessageBindings(dvCore);
    return ok;
}

bool DvSystemChecks::checkMessageIvValidity(const DVEditorCorePtr &dvCore) const
{
    if (!m_storage && m_storage->ivQuery()) {
        return true;
    }
    scs::IvSystemQueries *ivQuery = m_storage->ivQuery();
    QStringList ivFunctionNames = ivQuery->functionsNames();

    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVFunction *> functions = model->allObjectsByType<dvm::DVFunction>();
    for (dvm::DVFunction *f : functions) {
        if (!ivFunctionNames.contains(f->title(), m_caseCheck)) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Function binding '%1' is not available in the interface view").arg(f->title()),
                    dvCore->filePath());
        }
    }

    return ok;
}

bool DvSystemChecks::checkUniqueMessageBindings(const DVEditorCorePtr &dvCore) const
{
    if (!m_storage) {
        return true;
    }
    bool ok = true;
    dvm::DVModel *model = dvCore->appModel()->objectsModel();
    QVector<dvm::DVFunction *> functions = model->allObjectsByType<dvm::DVFunction>();
    QStringList duplicateNames;
    for (dvm::DVFunction *f : functions) {
        const QString fTitle = f->title();
        // Check is DV functions are bound uniquely
        int c = std::count_if(functions.begin(), functions.end(),
                [f, &fTitle](dvm::DVFunction *cf) { return fTitle == cf->title() && f != cf; });
        if (c > 0 && !duplicateNames.contains(fTitle)) {
            ok = false;
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Function binding '%1' is used more than once").arg(fTitle), dvCore->filePath());
            duplicateNames.append(f->title());
        }
    }

    return ok;
}

} // namespace scs
