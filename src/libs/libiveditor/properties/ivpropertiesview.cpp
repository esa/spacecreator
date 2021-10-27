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

#include "ivpropertiesview.h"

#include "contextparametersmodel.h"
#include "ifaceparametersmodel.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "propertieslistmodel.h"

#include <QPushButton>

namespace ive {

ContextParametersView::ContextParametersView(QWidget *widget)
    : shared::PropertiesViewBase({ ContextParametersModel::Column::Type }, widget)
{
}

void ContextParametersView::updateButtons()
{
    if (!m_model || !m_model->entity())
        return;

    if (auto fn = qobject_cast<const ivm::IVFunction *>(m_model->entity())) {
        const bool disabled = fn->inheritsFunctionType();
        addButton()->setDisabled(disabled);
        deleteButton()->setDisabled(disabled);
    }
}

IfaceParametersView::IfaceParametersView(QWidget *widget)
    : shared::PropertiesViewBase({ IfaceParametersModel::Column::Type, IfaceParametersModel::Column::Encoding,
                                         IfaceParametersModel::Column::Direction },
              widget)
{
}

void IfaceParametersView::updateButtons()
{
    if (!m_model || !m_model->entity())
        return;

    if (auto iface = qobject_cast<const ivm::IVInterface *>(m_model->entity())) {
        if (iface->isClone()) {
            addButton()->setDisabled(true);
            deleteButton()->setDisabled(true);
        } else if (iface->isRequired()) {
            if (auto ri = iface->as<const ivm::IVInterfaceRequired *>()) {
                if (ri->hasPrototypePi()) {
                    addButton()->setDisabled(true);
                    deleteButton()->setDisabled(true);
                }
            }
        } else if (iface->kind() == ivm::IVInterface::OperationKind::Cyclic) {
            addButton()->setDisabled(true);
            deleteButton()->setDisabled(true);
        } else if (iface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
            addButton()->setDisabled(iface->params().size() >= 1);
        }
    }
}

void IfaceParametersView::setModel(shared::PropertiesModelBase *model)
{
    shared::PropertiesViewBase::setModel(model);
    if (!m_model || !m_model->entity())
        return;

    if (auto iface = m_model->entity()->as<ivm::IVInterface *>()) {
        connect(iface, &ivm::IVInterface::paramsChanged, this, &IfaceParametersView::updateButtons,
                Qt::UniqueConnection);
    }
}

} // namespace ive
