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

#pragma once

#include <QPair>
#include <QPointer>
#include <QSharedPointer>
#include <QVector>
#include <QWeakPointer>

namespace aadl {
class AADLObject;
class AADLObjectConnection;
class AADLObjectFunction;
class AADLObjectsModel;
}

namespace aadlinterface {
class IVEditorCore;
}

namespace msc {
class MscChart;
class MSCEditorCore;
class MscInstance;
class MscMessage;

/*!
   \brief The AadlChecks class is used to check consistency of a msc model with one aadl model
 */
class AadlChecks
{
public:
    AadlChecks();
    ~AadlChecks();

    void setMscPlugin(msc::MSCEditorCore *mscPlugin);
    void setIvPlugin(QSharedPointer<aadlinterface::IVEditorCore> ivPlugin);

    QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceNames();
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceRelations();

    QVector<QPair<msc::MscChart *, msc::MscMessage *>> checkMessages();

private:
    aadl::AADLObjectsModel *aadlModel() const;
    void updateAadlFunctions();
    aadl::AADLObjectFunction *correspondingFunction(msc::MscInstance *instance) const;
    bool correspond(const aadl::AADLObject *aadlObj, const msc::MscInstance *instance) const;
    bool correspond(const aadl::AADLObjectFunction *aadlFunc, const msc::MscInstance *instance) const;
    bool hasAncestor(aadl::AADLObjectFunction *func, const QVector<aadl::AADLObjectFunction *> allFunctions) const;
    bool hasDescendant(aadl::AADLObjectFunction *func, const QVector<aadl::AADLObjectFunction *> allFunctions) const;
    bool isAncestor(aadl::AADLObjectFunction *func, aadl::AADLObjectFunction *otherFunc) const;

    void updateAadlConnections();
    aadl::AADLObjectConnection *correspondingConnection(msc::MscMessage *message) const;
    bool correspond(const aadl::AADLObjectConnection *connection, const msc::MscMessage *message) const;

    QPointer<msc::MSCEditorCore> m_mscPlugin;
    QWeakPointer<aadlinterface::IVEditorCore> m_ivPlugin;

    QVector<aadl::AADLObjectFunction *> m_aadlFunctions;
    QVector<aadl::AADLObjectConnection *> m_aadlConnections;
};

}
