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

#include <QAbstractTableModel>
#include <QList>

namespace requirement {

/**
 * @brief Model to hold requirements for a Qt view
 */
class RequirementsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum RoleNames {
        IssueLinkRole = Qt::UserRole + 1
    };

    enum HEADER_SECTIONS{
        REQUIREMENT_ID = 0,
        DESCRIPTION = 1,
        CHECKED = 2
    };

    explicit RequirementsModel(QObject *parent = nullptr);

    void clear();

    /**
     * Replaces the set of existing requirements with the given one
     */
    void setRequirements(const QList<Requirement> &requirements);
    /**
     * Appends the given @p requiremnets to the existing ones
     */
    void addRequirements(const QList<requirement::Requirement> &requirements);

    void setSelectedRequirementsIDs(const QStringList &requirementIDs);
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
    QString getIdFromModelIndex(const QModelIndex &index) const;

private:
    QList<Requirement> m_requirements;
    QSet<QString> m_selected_requirements;
};

} // namespace requirement
