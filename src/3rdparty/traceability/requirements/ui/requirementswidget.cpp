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
    connect(m_model, &requirement::RequirementsModelBase::rowsInserted,
            [this]() { ui->allRequirements->resizeRowsToContents(); });
    connect(ui->allRequirements->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &RequirementsWidget::modelSelectionChanged);
    connect(ui->allRequirements, &QTableView::doubleClicked, this, &RequirementsWidget::openIssueLink);
    connect(ui->refreshButton, &QPushButton::clicked, this, &RequirementsWidget::setLoginData);
    connect(ui->createRequirementButton, &QPushButton::clicked, this, &RequirementsWidget::showNewRequirementDialog);
    connect(ui->removeRequirementButton, &QPushButton::clicked, this, &RequirementsWidget::removeRequirement);
    connect(ui->urlLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    ui->tokenLineEdit->setEchoMode(QLineEdit::Password);
    connect(ui->tokenLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_textFilterModel,
            &QSortFilterProxyModel::setFilterFixedString);
    connect(m_reqManager, &RequirementsManager::projectIDChanged, this, &RequirementsWidget::requestRequirements);
    connect(m_reqManager, &RequirementsManager::requirementCreated, this, &RequirementsWidget::requestRequirements);
    connect(m_reqManager, &RequirementsManager::requirementClosed, this, &RequirementsWidget::requestRequirements);
    connect(ui->filterButton, &QPushButton::clicked, this, &RequirementsWidget::toggleShowUsedRequirements);
    connect(m_reqManager, &RequirementsManager::busyChanged, this, &RequirementsWidget::updateServerStatus);
    connect(m_reqManager, &RequirementsManager::listOfTags, this, &RequirementsWidget::fillTagBar);
    connect(m_reqManager, &RequirementsManager::connectionError, this, [this](QString error) {
        updateServerStatus();
        QMessageBox::warning(this, tr("Connection error"), tr("Connection failed for this error:\n%1").arg(error));
    });
    connect(m_reqManager, &RequirementsManager::fetchingRequirementsEnded, m_reqManager,
            &RequirementsManager::requestTags);

    bool hasCredentialsStored = loadSavedCredentials();
    if (hasCredentialsStored) {
        setLoginData();
    }

    loadSavedRequirementsTableGeometry();

    const QString urlTooltip = tr("Set the Gitlab server URL including the project path");
    ui->urlLabel->setToolTip(urlTooltip);
    ui->urlLineEdit->setToolTip(urlTooltip);
    const QString tokenTooltip = tr(
            "To create a personal access token, go to  Profile > Preferences > Access Tokens \n in your gitlab server");
    ui->tokenLabel->setToolTip(tokenTooltip);
    ui->tokenLineEdit->setToolTip(tokenTooltip);
    connect(ui->createTokenButton, &QPushButton::clicked, this, &RequirementsWidget::openTokenSettingsPage);
    ui->filterButton->setIcon(QPixmap(":/requirementsresources/icons/filter_icon.svg"));
}

bool RequirementsWidget::loadSavedCredentials()
{
    QSettings settings;
    settings.beginGroup("SpaceCreator");
    auto gitlabToken = settings.value(tokenKey(QUrl(m_requirementsUrl).host())).toString();
    settings.endGroup();
    if (m_requirementsUrl.isEmpty() || gitlabToken.isEmpty()) {
        return false;
    }

    setUrl(m_requirementsUrl);
    setToken(gitlabToken);
    return true;
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
 * \brief RequirementsWidget::setUrl Sets the URL to fetch the requiremenst from
 */
void RequirementsWidget::setUrl(const QString &url)
{
    ui->urlLineEdit->setText(url);
}

/*!
 * \brief RequirementsWidget::url Returns the URL to fetch the requiremenst from
 */
QUrl RequirementsWidget::url() const
{
    return QUrl(ui->urlLineEdit->text());
}

/*!
 * \brief RequirementsWidget::setToken Sets the token to authenticate for fetching the requirements
 * \note The token is stored in the settings and used from there when fetching the data
 */
void RequirementsWidget::setToken(const QString &token)
{
    ui->tokenLineEdit->setText(token);
}

/*!
 * \brief RequirementsWidget::token Returns the token to authenticate for fetching the requirements
 */
QString RequirementsWidget::token() const
{
    return ui->urlLineEdit->text();
}

void RequirementsWidget::onChangeOfCredentials()
{
    const QUrl newUrl(ui->urlLineEdit->text(), QUrl::StrictMode);
    if (!newUrl.isValid()) {
        ui->createTokenButton->setEnabled(false);
        return;
    }

    ui->createTokenButton->setEnabled(true);

    QString newUrlHostKey = tokenKey(newUrl.host());
    QSettings settings;
    settings.beginGroup("SpaceCreator");
    const QString gitlabToken = settings.value(newUrlHostKey).toString();

    // If token is empty, try to find the token in the settings
    if (ui->tokenLineEdit->text().isEmpty() && !gitlabToken.isEmpty()) {
        ui->tokenLineEdit->setText(gitlabToken);
    }

    // If anything changed
    if ((gitlabToken != ui->tokenLineEdit->text()) || (ui->urlLineEdit->text() != m_requirementsUrl)) {
        settings.setValue(newUrlHostKey, ui->tokenLineEdit->text());
        m_requirementsUrl = ui->urlLineEdit->text().toUtf8();
    }
    settings.endGroup();

    emit requirementsUrlChanged(m_requirementsUrl);

    if (!m_requirementsUrl.isEmpty() && !ui->tokenLineEdit->text().isEmpty()) {
        setLoginData();
    }
}

void RequirementsWidget::requestRequirements()
{
    m_reqManager->requestAllRequirements();
}

void RequirementsWidget::setLoginData()
{
    if (m_reqManager->isBusy()) {
        return;
    }

    ui->serverStatusLabel->setPixmap({});

    if (ui->urlLineEdit->text().isEmpty() || ui->tokenLineEdit->text().isEmpty()) {
        ui->serverStatusLabel->setPixmap({});
        return;
    }

    if (ui->urlLineEdit->text() == m_reqManager->projectUrl() && ui->tokenLineEdit->text() == m_reqManager->token()) {
        m_reqManager->requestAllRequirements();
        return;
    }

    ui->serverStatusLabel->setToolTip(tr("Checking connection to the server"));
    m_reqManager->setCredentials(ui->urlLineEdit->text(), ui->tokenLineEdit->text());
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

    const bool connectionOk = (m_reqManager->projectID() != -1);
    if (connectionOk) {
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/requirementsresources/icons/check_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server is ok"));
    } else {
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/requirementsresources/icons/uncheck_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server failed"));
    }
}

void RequirementsWidget::openIssueLink(const QModelIndex &index)
{
    QDesktopServices::openUrl(index.data(RequirementsModelBase::RoleNames::IssueLinkRole).toString());
}

void RequirementsWidget::openTokenSettingsPage()
{
    QUrl url(ui->urlLineEdit->text());
    url.setPath("/-/profile/personal_access_tokens");
    QDesktopServices::openUrl(url);
}

void RequirementsWidget::toggleShowUsedRequirements()
{
    if (ui->allRequirements->model() == &m_checkedModel) {
        ui->allRequirements->setModel(&m_tagFilterModel);
        ui->filterButton->setIcon(QPixmap(":/requirementsresources/icons/filter_icon.svg"));
    } else {
        ui->allRequirements->setModel(&m_checkedModel);
        ui->filterButton->setIcon(QPixmap(":/requirementsresources/icons/disable_filter_icon.svg"));
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
            auto button = new QToolButton(ui->tagFilterBar);
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
            ui->tagFilterBar->addWidget(button);
        }
    }
}

bool RequirementsWidget::tagButtonExists(const QString &tag) const
{
    for (const QToolButton *button : m_tagButtons) {
        if (button->text() == tag) {
            return true;
        }
    }
    return false;
}

void RequirementsWidget::showNewRequirementDialog() const
{
    QScopedPointer<AddNewRequirementDialog> dialog(new AddNewRequirementDialog());
    dialog->setModal(true);
    auto ret = dialog->exec();
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
        auto currentIndex = ui->allRequirements->selectionModel()->currentIndex();
        if (currentIndex.isValid()) {
            Requirement requirement = m_model->requirementFromIndex(currentIndex);
            m_reqManager->removeRequirement(requirement);
        }
    }
}

void RequirementsWidget::modelSelectionChanged(const QItemSelection &selected, const QItemSelection & /*unused*/)
{
    bool enabled = (selected.indexes().count() > 0);
    ui->removeRequirementButton->setEnabled(enabled);
}

}
