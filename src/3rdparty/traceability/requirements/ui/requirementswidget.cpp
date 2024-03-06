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

#include "requirementswidget.h"

#include "addnewrequirementdialog.h"
#include "requirementsmanager.h"
#include "requirementsmodelbase.h"
#include "ui_requirementswidget.h"
#include "widgetbar.h"

#include <QCursor>
#include <QDesktopServices>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QTableView>
#include <QToolButton>

namespace requirement {
const int kIconSize = 16;

RequirementsWidget::RequirementsWidget(
        const QString &requirementsUrl, RequirementsManager *manager, RequirementsModelBase *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequirementsWidget)
    , m_reqManager(manager)
    , m_model(model)
    , m_requirementsUrl(requirementsUrl)
    , m_widgetBar(new tracecommon::WidgetBar(this))
{
    ui->setupUi(this);
    m_textFilterModel.setDynamicSortFilter(true);
    m_textFilterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_textFilterModel.setFilterKeyColumn(-1);
    m_textFilterModel.setSourceModel(m_model);

    m_tagFilterModel.setDynamicSortFilter(true);
    m_tagFilterModel.setSourceModel(&m_textFilterModel);

    m_checkedModel.setFilterKeyColumn(RequirementsModelBase::CHECKED);
    m_checkedModel.setSourceModel(&m_tagFilterModel);

    ui->allRequirements->setModel(&m_tagFilterModel);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);

    ui->allRequirements->setColumnWidth(1, width() - ui->allRequirements->width());

    ui->allRequirements->horizontalHeader()->setStretchLastSection(true);
    ui->allRequirements->setSortingEnabled(true);
    ui->removeRequirementButton->setEnabled(false);
    connect(m_model, &requirement::RequirementsModelBase::rowsInserted, this,
            [this]() { ui->allRequirements->resizeRowsToContents(); });
    connect(ui->allRequirements->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &RequirementsWidget::modelSelectionChanged);
    connect(ui->allRequirements, &QTableView::doubleClicked, this, &RequirementsWidget::openIssueLink);
    connect(ui->refreshButton, &QPushButton::clicked, this, &RequirementsWidget::setLoginData);
    connect(ui->createRequirementButton, &QPushButton::clicked, this, &RequirementsWidget::showNewRequirementDialog);
    connect(ui->removeRequirementButton, &QPushButton::clicked, this, &RequirementsWidget::removeRequirement);
    connect(ui->credentialWidget, &tracecommon::CredentialWidget::urlChanged, this,
            &RequirementsWidget::onChangeOfCredentials);
    connect(ui->credentialWidget, &tracecommon::CredentialWidget::tokenChanged, this,
            &RequirementsWidget::onChangeOfCredentials);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_textFilterModel,
            &QSortFilterProxyModel::setFilterFixedString);
    connect(m_reqManager, &RequirementsManager::projectIDChanged, this, &RequirementsWidget::updateProjectReady);
    connect(m_reqManager, &RequirementsManager::requirementCreated, this, &RequirementsWidget::requestRequirements);
    connect(m_reqManager, &RequirementsManager::requirementClosed, this, &RequirementsWidget::requestRequirements);
    connect(ui->filterButton, &QPushButton::clicked, this, &RequirementsWidget::toggleShowUsedRequirements);
    connect(m_reqManager, &RequirementsManager::busyChanged, this, &RequirementsWidget::updateServerStatus);
    connect(m_reqManager, &RequirementsManager::listOfTags, this, &RequirementsWidget::fillTagBar);
    connect(m_reqManager, &RequirementsManager::connectionError, this, [this](const QString &error) {
        updateServerStatus();
        QMessageBox::warning(this, tr("Connection error"), tr("Connection failed for this error:\n%1").arg(error));
    });
    connect(m_reqManager, &RequirementsManager::fetchingRequirementsEnded, m_reqManager,
            &RequirementsManager::requestTags);

    loadSavedRequirementsTableGeometry();

    ui->filterButton->setIcon(QPixmap(":/tracecommonresources/icons/filter_icon.svg"));
    ui->verticalLayout->insertWidget(0, m_widgetBar);
}

bool RequirementsWidget::loadSavedRequirementsTableGeometry()
{
    QSettings settings;
    settings.beginGroup("SpaceCreator");
    auto AllRequirementsHeaderState = settings.value("AllRequirementsHeaderState");
    settings.endGroup();
    if (AllRequirementsHeaderState.isValid()) {
        ui->allRequirements->horizontalHeader()->restoreState(AllRequirementsHeaderState.toByteArray());
        return true;
    }
    return false;
}

RequirementsWidget::~RequirementsWidget()
{
    QSettings settings;
    settings.beginGroup("SpaceCreator");
    QByteArray AllRequirementsHeaderState = ui->allRequirements->horizontalHeader()->saveState();
    settings.setValue("AllRequirementsHeaderState", AllRequirementsHeaderState);
    settings.endGroup();
    delete ui;
}

/*!
 * Returns the URL to fetch the requirements from
 */
QUrl RequirementsWidget::url() const
{
    return ui->credentialWidget->url();
}
/*!
 * \brief RequirementsWidget::setUrl sets the url to fetch the requirements from
 * \param url
 */
void RequirementsWidget::setUrl(const QUrl &url)
{
    ui->credentialWidget->setUrl(url.toString());
}

/*!
 * \brief RequirementsWidget::token Returns the token to authenticate for fetching the requirements
 */
QString RequirementsWidget::token() const
{
    return ui->credentialWidget->token();
}

/*!
 * \brief RequirementsWidget::setToken sets the Token to authenticate for fetching the requirements
 * \param token
 */
void RequirementsWidget::setToken(const QString &token)
{
    ui->credentialWidget->setToken(token);
}

void RequirementsWidget::onChangeOfCredentials()
{
    m_requirementsUrl = ui->credentialWidget->url().toString();
    Q_EMIT requirementsUrlChanged(m_requirementsUrl);
    const QString newToken(ui->credentialWidget->token());
    if (m_requirementsUrl.isEmpty() || newToken.isEmpty()) {
        return;
    }
    m_reqManager->setCredentials(m_requirementsUrl, newToken);
    emit requirementsCredentialsChanged(m_requirementsUrl, newToken);
}

void RequirementsWidget::requestRequirements()
{
    if (m_reqManager && m_reqManager->hasValidProjectID()) {
        m_reqManager->requestAllRequirements();
    }
}

void RequirementsWidget::setLoginData()
{
    if (m_reqManager->isBusy()) {
        return;
    }

    ui->serverStatusLabel->setPixmap({});

    const QUrl currUrl(ui->credentialWidget->url());
    const QString currToken(ui->credentialWidget->token());

    if (currUrl.isEmpty() || currToken.isEmpty()) {
        ui->serverStatusLabel->setPixmap({});
        return;
    }
    m_model.clear();
    if (currUrl == m_reqManager->projectUrl() && currToken == m_reqManager->token()) {

        m_reqManager->requestAllRequirements();
        return;
    }

    ui->serverStatusLabel->setToolTip(tr("Checking connection to the server"));
    m_reqManager->setCredentials(currUrl.toString(), currToken);
}

void RequirementsWidget::updateServerStatus()
{
    ui->refreshButton->setEnabled(!m_reqManager->isBusy());
    const QCursor busyCursor(Qt::WaitCursor);
    if (m_reqManager->isBusy()) {
        if (ui->allRequirements->cursor() != busyCursor) {
            ui->allRequirements->setCursor(busyCursor);
        }
    } else {
        if (ui->allRequirements->cursor() == busyCursor) {
            ui->allRequirements->unsetCursor();
        }
    }

    const bool connectionOk = (m_reqManager->hasValidProjectID());
    if (connectionOk) {
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/tracecommonresources/icons/check_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server is ok"));
    } else {
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/tracecommonresources/icons/uncheck_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server failed"));
    }
}

void RequirementsWidget::updateProjectReady()
{
    if (!m_reqManager) {
        ui->createRequirementButton->setEnabled(false);
        return;
    }

    ui->createRequirementButton->setEnabled(m_reqManager->hasValidProjectID());
    requestRequirements();
}

void RequirementsWidget::openIssueLink(const QModelIndex &index)
{
    QDesktopServices::openUrl(index.data(tracecommon::TraceCommonModelBase::IssueLinkRole).toString());
}

void RequirementsWidget::toggleShowUsedRequirements()
{
    if (ui->allRequirements->model() == &m_checkedModel) {
        ui->allRequirements->setModel(&m_tagFilterModel);
        ui->filterButton->setIcon(QPixmap(":/tracecommonresources/icons/filter_icon.svg"));
    } else {
        ui->allRequirements->setModel(&m_checkedModel);
        ui->filterButton->setIcon(QPixmap(":/tracecommonresources/icons/disable_filter_icon.svg"));
    }
}

void RequirementsWidget::fillTagBar(const QStringList &tags)
{
    auto it = std::remove_if(m_tagButtons.begin(), m_tagButtons.end(), [this, tags](QToolButton *button) {
        if (!tags.contains(button->text())) {
            m_tagFilterModel.removeTag(button->text());
            button->deleteLater();
            return true;
        }
        return false;
    });
    m_tagButtons.erase(it, m_tagButtons.end());

    for (const QString &tag : tags) {
        if (!tagButtonExists(tag)) {
            auto button = new QToolButton(m_widgetBar);
            button->setText(tag);
            button->setCheckable(true);
            connect(button, &QToolButton::toggled, this, [this](bool checked) {
                auto button = dynamic_cast<QToolButton *>(sender());
                if (!button) {
                    return;
                }
                if (checked) {
                    m_tagFilterModel.addTag(button->text());
                } else {
                    m_tagFilterModel.removeTag(button->text());
                }
            });

            m_tagButtons.append(button);
            m_widgetBar->addWidget(button);
        }
    }
}

bool RequirementsWidget::tagButtonExists(const QString &tag) const
{
    return std::any_of(
            m_tagButtons.begin(), m_tagButtons.end(), [&tag](const auto *btn) { return btn->text() == tag; });
}

void RequirementsWidget::showNewRequirementDialog() const
{
    QScopedPointer<AddNewRequirementDialog> dialog(new AddNewRequirementDialog(m_model.get()));
    dialog->setModal(true);
    const auto ret = dialog->exec();
    if (ret == QDialog::Accepted) {
        m_reqManager->createRequirement(
                dialog->title(), dialog->reqIfId(), dialog->description(), dialog->testMethod());
    }
}
/*!
 * \brief removeRequirement takes a look in the selectionModel.
 *        If more than one row is selected returns, otherwise
 *        calls Requirements Manager to remove it (aka close the issue)
 */
void RequirementsWidget::removeRequirement()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove requirement"),
            tr("Are you sure you want to remove the selected requirement?"), QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        const auto &currentIndex = ui->allRequirements->selectionModel()->currentIndex();
        if (currentIndex.isValid()) {
            const Requirement &requirement = m_model->requirementFromIndex(currentIndex);
            m_reqManager->removeRequirement(requirement);
        }
    }
}

void RequirementsWidget::modelSelectionChanged(const QItemSelection &selected, const QItemSelection & /*unused*/)
{
    const bool enabled(selected.indexes().count() > 0);
    ui->removeRequirementButton->setEnabled(enabled);
}

}
