/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlobjectfunctiontype.h"

#include <QObject>

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionPrivate;
class AADLObjectFunction : public AADLObjectFunctionType
{
    Q_OBJECT
public:
    explicit AADLObjectFunction(const QString &title = QString(), QObject *parent = nullptr);
    ~AADLObjectFunction() override;

    AADLObject::AADLObjectType aadlType() const override;

    void setAttr(const QString &name, const QVariant &val) override;
    void postInit() override;

    enum class ClonedIfacesPolicy
    {
        Keep = 0,
        Kill
    };
    void setInstanceOf(AADLObjectFunctionType *fnType, ClonedIfacesPolicy killClones = ClonedIfacesPolicy::Kill);

Q_SIGNALS:
    void attrChanged_instanceOf(const QString &functionType);

private Q_SLOTS:
    void onFunctionTypeRemoved();
    void onFunctionTypeUntyped(bool nowIsType);
    void onFunctionTypeRenamed(const QString &newName);

private:
    const std::unique_ptr<AADLObjectFunctionPrivate> d;

    void setFunctionType(const QString &functionTypeName);
    void setFunctionTypeAttr(const QString &functionTypeName);

    void connectToFunctionType();
    void disconnectFromFunctionType();

    void cloneInterfaces();
    void uncloneInterfaces(ClonedIfacesPolicy killClones);

    void cloneInterface(AADLObjectIface *iface);
    void uncloneInterface(AADLObjectIface *iface, ClonedIfacesPolicy killClones);
};

typedef QVector<AADLObjectFunction *> AADLFunctionsVector;

} // ns aadl
} // ns taste3
