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

#include "aadlobjectsmodel.h"
#include "interface/interfacedocument.h"
#include "iveditorplugin.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscplugin.h"

#include <QDebug>

namespace msc {

AadlChecks::AadlChecks() { }

AadlChecks::~AadlChecks() { }

void AadlChecks::setMscPlugin(MSCPlugin *mscPlugin)
{
    m_mscPlugin = mscPlugin;
}

void AadlChecks::setIvPlugin(aadlinterface::IVEditorPlugin *ivPlugin)
{
    m_ivPlugin = ivPlugin;
}

/*!
   Checks all instances if they are defined in the IV model
 */
QVector<QPair<MscChart *, MscInstance *>> AadlChecks::checkInstances()
{
    QVector<QPair<MscChart *, MscInstance *>> result;
    if (!m_ivPlugin || !m_mscPlugin) {
        return result;
    }

    aadl::AADLObjectsModel *aadlModel = nullptr;
    if (!m_ivPlugin->document() || !m_ivPlugin->document()->objectsModel()) {
        qWarning() << "No AADLObjectsModel";
        return result;
    }
    aadlModel = m_ivPlugin->document()->objectsModel();

    QStringList functionNames;
    const QHash<shared::Id, aadl::AADLObject *> &aadlObjects = aadlModel->objects();
    for (auto obj : aadlObjects) {
        if (obj->aadlType() == aadl::AADLObject::Type::Function) {
            functionNames.append(obj->title());
        }
    }

    QVector<msc::MscChart *> charts = m_mscPlugin->mainModel()->mscModel()->allCharts();
    for (msc::MscChart *chart : charts) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (!functionNames.contains(instance->name())) {
                result << QPair<MscChart *, MscInstance *>(chart, instance);
            }
        }
    }

    return result;
}

}
