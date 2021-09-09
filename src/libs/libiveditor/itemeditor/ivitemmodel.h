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

#include "interface/veitemmodel.h"
#include "ivmodel.h"

namespace ivm {
class IVObject;
class IVInterfaceGroup;
} // namespace ivm

namespace shared {
class DelayedSignal;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class IVFunctionGraphicsItem;

/*!
   \class IVItemModel is the model represents the scene graph of the currently selected/visible
   IV model. It handles all changes with existing entities and reflects them on graphic scene.
 */

class IVItemModel : public shared::ui::VEItemModel
{
    Q_OBJECT
public:
    enum class Roles
    {
        IdRole = Qt::UserRole,
        ItemVisible,
    };
    Q_ENUM(Roles);

    explicit IVItemModel(ivm::IVModel *model, cmd::CommandsStack *commandsStack, QObject *parent = nullptr);
    ~IVItemModel() override;

    void changeRootItem(shared::Id id);
    ivm::IVModel *objectsModel() const override;

private Q_SLOTS:
    void onObjectRemoved(shared::Id objectId) override;
    void onObjectAdded(shared::Id objectId) override;

    void onRootObjectChanged(shared::Id rootId);
    void onConnectionAddedToGroup(ivm::IVConnection *connection);
    void onConnectionRemovedFromGroup(ivm::IVConnection *connection);

    void scheduleInterfaceTextUpdate();
    void updateInterfaceTexts();

private:
    shared::ui::VEInteractiveObject *createItem(shared::Id objectId) override;
    void initItem(shared::ui::VEInteractiveObject *item) override;

    IVFunctionGraphicsItem *rootItem() const;
    void setupInnerGeometry(ivm::IVObject *obj) const;

    void updateInterfaceOnConnectionAdd(ivm::IVConnection *connection, ivm::IVInterfaceGroup *connectionGroupEndPoint);
    void updateInterfaceOnConnectionRemove(
            ivm::IVConnection *connection, ivm::IVInterfaceGroup *connectionGroupEndPoint);

private:
    shared::DelayedSignal *m_textUpdate = nullptr;
};

} // namespace ive
