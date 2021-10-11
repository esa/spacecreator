/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

namespace shared {
namespace cmd {
class CommandsStackBase;
} // namespace cmd
} // namespace shared

namespace dvm {
class DVFunction;
class DVModel;
class DVObject;
} // namespace dvm

namespace dve {

class DVTreeViewModel : public shared::AbstractVisualizationModel
{
    Q_OBJECT
public:
    enum Columns
    {
        Name = 0,
        Implementation
    };
    Q_ENUM(Columns)
    enum Roles
    {
        DVObjectRole = Qt::UserRole + 321
    };
    Q_ENUM(Roles)

    explicit DVTreeViewModel(
            dvm::DVModel *dvModel, shared::cmd::CommandsStackBase *commandsStack, QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

protected:
    void updateItemData(QStandardItem *item, shared::VEObject *object) override;
    QList<QStandardItem *> createItems(shared::VEObject *obj) override;

private Q_SLOTS:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

private:
    void setName(dvm::DVObject *obj, QStandardItem *item);
    void updateImplementation(dvm::DVFunction *fn);
    QStandardItem *implementationItem(dvm::DVFunction *fn);
};

} // namespace dve
