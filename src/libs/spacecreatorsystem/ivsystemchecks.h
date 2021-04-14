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

#include "systemchecks.h"

#include <QObject>
#include <QPair>
#include <QPointer>
#include <QSharedPointer>
#include <QStringList>
#include <QVector>
#include <QWeakPointer>

namespace ivm {
class IVObject;
class IVConnection;
class IVFunction;
class IVModel;
}

namespace ive {
class IVEditorCore;
}

namespace msc {
class MscChart;
class MSCEditorCore;
class MscInstance;
class MscMessage;
class MscMessageDeclaration;
class MscModel;
}

namespace scs {

/*!
   \brief The IvSystemChecks class is used to check consistency of a msc model with one aadl model
 */
class IvSystemChecks : public msc::SystemChecks
{
    Q_OBJECT
public:
    explicit IvSystemChecks(QObject *parent = nullptr);
    ~IvSystemChecks();

    void setMscCore(msc::MSCEditorCore *mscCore);
    void setIvCore(QSharedPointer<ive::IVEditorCore> ivCore);
    const QSharedPointer<ive::IVEditorCore> &ivCore() const;

    QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceNames() const override;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceRelations() const override;
    bool checkInstance(const msc::MscInstance *instance) const override;
    QStringList functionsNames() const override;

    QVector<QPair<msc::MscChart *, msc::MscMessage *>> checkMessages() const override;
    bool checkMessage(const msc::MscMessage *message) const override;
    QStringList connectionNames() const override;
    QStringList connectionNamesFromTo(const QString &sourceName, const QString &targetName) const override;

    ivm::IVFunction *correspondingFunction(const msc::MscInstance *instance) const;
    bool correspond(const ivm::IVObject *aadlObj, const msc::MscInstance *instance) const override;
    bool correspond(const ivm::IVFunction *aadlFunc, const msc::MscInstance *instance) const override;
    bool correspond(const ivm::IVConnection *connection, const msc::MscMessage *message) const override;

    QVector<msc::MscMessageDeclaration *> allConnectionsAsDeclaration() const override;

    Qt::CaseSensitivity stringSensitivity() const override;

    bool hasValidSystem() const override;

Q_SIGNALS:
    void mscCoreChanged();

private:
    ivm::IVModel *aadlModel() const;
    bool hasAncestor(ivm::IVFunction *func, const QVector<ivm::IVFunction *> allFunctions) const;
    bool hasDescendant(ivm::IVFunction *func, const QVector<ivm::IVFunction *> allFunctions) const;
    bool isAncestor(ivm::IVFunction *func, ivm::IVFunction *otherFunc) const;

    QPointer<msc::MSCEditorCore> m_mscCore;
    QSharedPointer<ive::IVEditorCore> m_ivCore;
    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
};

}
