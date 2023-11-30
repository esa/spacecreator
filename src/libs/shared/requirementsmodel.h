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
along with this program. If not, see
<https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "commandsstackbase.h"
#include "requirementsmodelbase.h"

#include <QAbstractTableModel>
#include <QList>

namespace shared {
class PropertyTemplateConfig;
class VEObject;

/*!
 * \brief Model to hold requirements for a Qt view to operate on a VE object directly.
 * \note The data is changed in the model via undo directly after the click
 */
class RequirementsModel : public requirement::RequirementsModelBase
{
    Q_OBJECT

public:
    explicit RequirementsModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void setDataObject(VEObject *obj);
    void setCommandMacro(cmd::CommandsStackBase::Macro *macro);
    void setPropertyTemplateConfig(PropertyTemplateConfig *dynPropConfig);

    void setAttributeName(const QString &name);

protected:
    cmd::CommandsStackBase::Macro *m_cmdMacro { nullptr };
    PropertyTemplateConfig *m_propTemplatesConfig { nullptr };
    VEObject *m_dataObject { nullptr };
    QString m_attributeName = "requests_ids";
};

} // namespace shared
