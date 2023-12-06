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

#include "mscrequirementsdialog.h"

#include "mscentity.h"
#include "requirementsmanager.h"
#include "requirementsmodelbase.h"
#include "ui/requirementswidget.h"
#include "ui_mscrequirementsdialog.h"

namespace msc {
using namespace requirement;

MscRequirementsDialog::MscRequirementsDialog(const QUrl &requirementsUrl, msc::MscEntity *entity, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MscRequirementsDialog)
    , m_entity(entity)
{
    ui->setupUi(this);

    m_reqManager = new RequirementsManager(RequirementsManager::REPO_TYPE::GITLAB, this);
    m_reqModel = new RequirementsModelBase(this);
    m_reqModel->setSelectedRequirements(RequirementsModelBase::convert(m_entity->requirements()));
    connect(m_reqManager, &RequirementsManager::listOfRequirements, m_reqModel,
            &RequirementsModelBase::addRequirements);
    connect(m_reqManager, &RequirementsManager::startingFetchingRequirements, m_reqModel,
            &RequirementsModelBase::clear);

    m_reqWidget = new RequirementsWidget(requirementsUrl.toString().toUtf8(), m_reqManager, m_reqModel, this);
    m_reqWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->contentLayout->addWidget(m_reqWidget, 0, 0, 1, 1);
}

MscRequirementsDialog::~MscRequirementsDialog()
{
    delete ui;
}

/*!
 * \brief MscRequirementsDialog::url is the URL of the project the requirements are fetched from
 */
QUrl MscRequirementsDialog::url() const
{
    return m_reqWidget->url();
}

/*!
 * \brief MscRequirementsDialog::token is the access token to fetch the requirements from the server
 * \sa url
 */
QString MscRequirementsDialog::token() const
{
    return m_reqWidget->token();
}

/*!
 * \brief MscRequirementsDialog::selectedRequirements Returns the list of IDs of requirements that are linked to the
 * entity
 */
QStringList MscRequirementsDialog::selectedRequirements() const
{
    return m_reqModel->selectedRequirements().values();
}

} // namespace msc
