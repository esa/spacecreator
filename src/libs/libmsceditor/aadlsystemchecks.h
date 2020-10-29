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

#include <QObject>
#include <QPair>
#include <QPointer>
#include <QSharedPointer>
#include <QStringList>
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
   \brief The AadlSystemChecks class is used to check consistency of a msc model with one aadl model
 */
class AadlSystemChecks : public QObject
{
    Q_OBJECT
public:
    explicit AadlSystemChecks(QObject *parent = nullptr);
    ~AadlSystemChecks();

    void setMscCore(msc::MSCEditorCore *mscCore);
    void setIvCore(QSharedPointer<aadlinterface::IVEditorCore> ivCore);
    const QSharedPointer<aadlinterface::IVEditorCore> &ivCore() const;
    bool hasIvCore() const;

    QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceNames() const;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceRelations() const;
    bool checkInstance(const msc::MscInstance *instance) const;
    QStringList functionsNames() const;

    QVector<QPair<msc::MscChart *, msc::MscMessage *>> checkMessages() const;
    bool checkMessage(const msc::MscMessage *message) const;
    QStringList connectionNames() const;
    bool connectionExists(QString name, const QString &sourceName, const QString &targetName) const;
    QStringList connectionNamesFromTo(QString sourceName, QString targetName) const;

Q_SIGNALS:
    void mscCoreChanged();
    void ivCoreChanged();

private:
    aadl::AADLObjectsModel *aadlModel() const;
    aadl::AADLObjectFunction *correspondingFunction(const MscInstance *instance) const;
    bool correspond(const aadl::AADLObject *aadlObj, const msc::MscInstance *instance) const;
    bool correspond(const aadl::AADLObjectFunction *aadlFunc, const msc::MscInstance *instance) const;
    bool hasAncestor(aadl::AADLObjectFunction *func, const QVector<aadl::AADLObjectFunction *> allFunctions) const;
    bool hasDescendant(aadl::AADLObjectFunction *func, const QVector<aadl::AADLObjectFunction *> allFunctions) const;
    bool isAncestor(aadl::AADLObjectFunction *func, aadl::AADLObjectFunction *otherFunc) const;

    aadl::AADLObjectConnection *correspondingConnection(const MscMessage *message) const;
    bool correspond(const aadl::AADLObjectConnection *connection, const msc::MscMessage *message) const;

    QPointer<msc::MSCEditorCore> m_mscCore;
    QSharedPointer<aadlinterface::IVEditorCore> m_ivCore;
};

}
