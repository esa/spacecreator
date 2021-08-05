/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstack.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "propertieslistmodel.h"

namespace shared {
class PropertyTemplateConfig;
}

namespace ive {

class IVPropertiesListModel : public shared::PropertiesListModel
{
    Q_OBJECT

public:
    explicit IVPropertiesListModel(
            cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);
    ~IVPropertiesListModel() override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = DataRole) override;

    ivm::IVObject *entity() const override;
    static ivm::meta::Props::Token tokenFromIndex(const QModelIndex &index);

protected:
    QPair<QString, QVariant> prepareDataForUpdate(const QModelIndex &index, const QVariant &value, int role) const;
    bool isEditable(const QModelIndex &index) const override;
};

class FunctionPropertiesListModel : public IVPropertiesListModel
{
public:
    explicit FunctionPropertiesListModel(
            cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = DataRole) override;
    ivm::IVFunction *entity() const override;

protected:
    bool isEditable(const QModelIndex &index) const override;
};

class InterfacePropertiesListModel : public IVPropertiesListModel
{
public:
    explicit InterfacePropertiesListModel(
            cmd::CommandsStack::Macro *macro, shared::PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = DataRole) override;
    ivm::IVInterface *entity() const override;

protected:
    bool isEditable(const QModelIndex &index) const override;
};

} // namespace ive
