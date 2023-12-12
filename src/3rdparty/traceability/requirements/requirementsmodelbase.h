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
#include <QSet>

namespace requirement {

/*!
 * \brief Base model to hold requirements for a Qt view
 */
class RequirementsModelBase : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum RoleNames
    {
        IssueLinkRole = Qt::UserRole + 1,
        ReqIfIdRole,
        IssueIdRole,
        TagsRole,
        DetailDescriptionRole,
    };

    enum HEADER_SECTIONS
    {
        REQUIREMENT_ID = 0,
        TITLE = 1,
        CHECKED = 2
    };

    explicit RequirementsModelBase(QObject *parent = nullptr);

    virtual void clear();

    virtual void setRequirements(const QList<Requirement> &requirements);
    virtual void addRequirements(const QList<requirement::Requirement> &requirements);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual const QSet<QString> &selectedRequirements() const;
    virtual void setSelectedRequirements(const QSet<QString> &selected);

    static QSet<QString> convert(const QStringList &list);

    Requirement requirementFromIndex(const QModelIndex &idx);

protected:
    QString getReqIfIdFromModelIndex(const QModelIndex &index) const;

    QList<Requirement> m_requirements;
    QSet<QString> m_selectedRequirements;
};

} // namespace requirement
