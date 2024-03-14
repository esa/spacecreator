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

#include "requirement.h"
#include "tracecommonmodelbase.h"

#include <QList>

namespace requirement {

/*!
 * \brief Base model to hold requirements for a Qt view
 */
class RequirementsModelBase : public tracecommon::TraceCommonModelBase
{
    Q_OBJECT

public:
    enum RoleNames
    {
        ReqIfIdRole = tracecommon::TraceCommonModelBase::UserRole + 1,
    };

    enum HEADER_SECTIONS
    {
        REQUIREMENT_ID = 0,
        TITLE = 1,
        CHECKED = 2
    };

    explicit RequirementsModelBase(QObject *parent = nullptr);
    /*!
     * \brief Empties the list of requirements of the model
     */
    virtual void clearRequirements();
    /*!
     * \brief set a list of requirements in the model
     * \param requirements
     */
    virtual void setRequirements(const QList<Requirement> &requirements);
    /*!
     * \brief Append some reuqiements to the existing requirements in the model
     * \param requirements
     */
    virtual void addRequirements(const QList<requirement::Requirement> &requirements);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /*!
     * \brief List of selected requirements in the UI
     * \return requirements
     */
    virtual QStringList selectedRequirements() const;

    /*!
     * \brief List of selected requirements in the UI
     * \return requirements
     */
    virtual void setSelectedRequirements(const QStringList &selected);

    Requirement requirementFromIndex(const QModelIndex &idx);
    /*!
     * \brief Check if a requirement ID already exists in the model
     * \param requirement ID
     * \return True if the Requirement ID already exists
     */
    bool reqIfIDExists(const QString &reqIfID) const;

protected:
    QString getReqIfIdFromModelIndex(const QModelIndex &index) const;

    QList<Requirement> m_requirements;
    QStringList m_selectedRequirements;
};

} // namespace requirement
