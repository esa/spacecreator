/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "requirementsmodel.h"

#include "commands/cmdentityattributeschange.h"
#include "entityattribute.h"
#include "veobject.h"

namespace requirement {

RequirementsModel::RequirementsModel(QObject *parent)
    : RequirementsModelBase(parent)
{
}

QVariant RequirementsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_requirements.size()) {
        return QVariant();
    }

    if (role == Qt::CheckStateRole && index.column() == CHECKED) {
        auto requirementID = getIdFromModelIndex(index);
        return (m_dataObject->requestsIDs().contains(requirementID)) ? Qt::Checked : Qt::Unchecked;
    }

    return RequirementsModelBase::data(index, role);
}

bool RequirementsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::CheckStateRole && index.column() == CHECKED) {
        auto requirementID = getIdFromModelIndex(index);
        auto checked = value.toBool();
        QStringList selectedRequirements = m_dataObject->requestsIDs();
        if (checked) {
            selectedRequirements << requirementID;
            m_selectedRequirements.insert(requirementID);
        } else {
            selectedRequirements.removeOne(requirementID);
            m_selectedRequirements.remove(requirementID);
        }

        bool ok = m_cmdMacro->push(new shared::cmd::CmdEntityAttributesChange(m_propTemplatesConfig, m_dataObject,
                { EntityAttribute { m_attributeName, QVariant::fromValue<QString>(selectedRequirements.join(",")),
                        EntityAttribute::Type::Attribute } }));
        emit dataChanged(index, index, { role });
        return ok;
    }

    return RequirementsModelBase::setData(index, value, role);
}

void RequirementsModel::setDataObject(shared::VEObject *obj)
{
    m_dataObject = obj;
}

void RequirementsModel::setCommandMacro(shared::cmd::CommandsStackBase::Macro *macro)
{
    m_cmdMacro = macro;
}

void RequirementsModel::setPropertyTemplateConfig(shared::PropertyTemplateConfig *dynPropConfig)
{
    m_propTemplatesConfig = dynPropConfig;
}

/*!
 * \brief setAttributeName Set the anme of the requirements attribute of the object
 * \note Default is "requests_ids"
 */
void RequirementsModel::setAttributeName(const QString &name)
{
    m_attributeName = name;
}

} // namespace requirement
