/*
  Copyright (C) 2020-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "abstractvisualizationmodel.h"
#include "common.h"

#include <QPointer>
#include <QStandardItemModel>

namespace ivm {
class IVObject;
class IVModel;
class IVConnection;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class IVVisualizationModelBase : public shared::AbstractVisualizationModel
{
    Q_OBJECT
public:
    explicit IVVisualizationModelBase(
            ivm::IVModel *ivModel, cmd::CommandsStack *commandsStack, QObject *parent = nullptr);
    enum ItemRole
    {
        IdRole = Qt::UserRole + 1,
        TypeRole
    };

protected:
    void updateItemData(QStandardItem *item, shared::VEObject *obj) override;
    QStandardItem *createItem(shared::VEObject *obj) override;

private Q_SLOTS:
    void updateConnectionItem(ivm::IVConnection *connection);
};

class IVVisualizationModel : public IVVisualizationModelBase
{
    Q_OBJECT
public:
    explicit IVVisualizationModel(ivm::IVModel *ivModel, cmd::CommandsStack *commandsStack, QObject *parent = nullptr);

    void updateItemData(QStandardItem *item, shared::VEObject *object) override;
    QStandardItem *createItem(shared::VEObject *obj) override;

private Q_SLOTS:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};

} // namespace ive
