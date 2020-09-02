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

#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmodel.h"

#include <QDebug>
#include <algorithm>

namespace msc {

AadlChecks::AadlChecks() { }

AadlChecks::~AadlChecks() { }

void AadlChecks::setMscPlugin(MSCEditorCore *mscPlugin)
{
    m_mscPlugin = mscPlugin;
}

void AadlChecks::setIvPlugin(aadlinterface::IVEditorCore *ivPlugin)
{
    m_ivPlugin = ivPlugin;
}

/*!
   Checks all instances if they are defined in the IV model
 */
QVector<QPair<MscChart *, MscInstance *>> AadlChecks::checkInstanceNames()
{
    QVector<QPair<MscChart *, MscInstance *>> result;
    if (!m_ivPlugin || !m_mscPlugin) {
        return result;
    }

    updateAadlFunctions();

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

void AadlChecks::updateAadlFunctions()
{
    m_aadlFunctions.clear();

    if (!m_ivPlugin) {
        return;
    }

    aadl::AADLObjectsModel *aadlModel = nullptr;
    if (!m_ivPlugin->document() || !m_ivPlugin->document()->objectsModel()) {
        qWarning() << "No AADLObjectsModel";
        return;
    }
    aadlModel = m_ivPlugin->document()->objectsModel();

    const QHash<shared::Id, aadl::AADLObject *> &aadlObjects = aadlModel->objects();
    for (auto obj : aadlObjects) {
        if (obj->aadlType() == aadl::AADLObject::Type::Function) {
            if (auto func = qobject_cast<aadl::AADLObjectFunction *>(obj)) {
                m_aadlFunctions.append(func);
            }
        }
    }
}

aadl::AADLObjectFunction *AadlChecks::correspondingFunction(MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    auto it = std::find_if(m_aadlFunctions.begin(), m_aadlFunctions.end(),
            [&instance](aadl::AADLObjectFunction *func) { return instance->name() == func->title(); });

    if (it == m_aadlFunctions.end()) {
        return nullptr;
    }

    return *it;
}

}
