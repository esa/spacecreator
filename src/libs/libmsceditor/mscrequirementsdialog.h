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

#pragma once

#include <QDialog>
#include <QPointer>
#include <QUrl>

class RequirementsWidget;
class RequirementsManager;

namespace requirement {
class RequirementsModelBase;
}

namespace msc {

class MscEntity;

namespace Ui {
class MscRequirementsDialog;
}

/*!
 * \brief The MscRequirementsDialog class is a lialog to edit the linked requirements for a single entity
 */
class MscRequirementsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MscRequirementsDialog(const QUrl &requirementsUrl, msc::MscEntity *entity, QWidget *parent = nullptr);
    ~MscRequirementsDialog();

    QUrl url() const;
    QString token() const;

    QByteArrayList selectedRequirements() const;

private:
    Ui::MscRequirementsDialog *ui;
    RequirementsWidget *m_reqWidget = nullptr;
    RequirementsManager *m_reqManager = nullptr;
    requirement::RequirementsModelBase *m_reqModel = nullptr;
    QPointer<msc::MscEntity> m_entity;
    QUrl m_reqUrl;
};

} // namespace msc
