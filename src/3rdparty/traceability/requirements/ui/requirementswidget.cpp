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

namespace requirement {
const int kIconSize = 16;

RequirementsWidget::RequirementsWidget(
        const QByteArray &requirementsUrl, RequirementsManager *manager, RequirementsModelBase *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequirementsWidget)
    , m_reqManager(manager)
    , m_model(model)
    , m_requirementsUrl(requirementsUrl)
{
    ui->setupUi(this);
    m_filterModel.setDynamicSortFilter(true);
    m_filterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel.setFilterKeyColumn(-1);
    m_filterModel.setSourceModel(m_model);
    m_checkedModel.setFilterKeyColumn(RequirementsModelBase::CHECKED);
    m_checkedModel.setSourceModel(&m_filterModel);
    ui->allRequirements->setModel(&m_filterModel);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->allRequirements->horizontalHeader()->setStretchLastSection(false);

    connect(ui->allRequirements, &QTableView::doubleClicked, this, &RequirementsWidget::openIssueLink);
    connect(ui->refreshButton, &QPushButton::clicked, this, &RequirementsWidget::setLoginData);
    connect(ui->createRequirementButton, &QPushButton::clicked, this, &RequirementsWidget::showNewRequirementDialog);
    connect(ui->urlLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    connect(ui->tokenLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(m_reqManager, &RequirementsManager::projectIDChanged, this, &RequirementsWidget::requestRequirements);
    connect(m_reqManager, &RequirementsManager::requirementCreated, this, &RequirementsWidget::requestRequirements);
    connect(ui->filterButton, &QPushButton::clicked, this, &RequirementsWidget::toggleShowUsedRequirements);
    connect(m_reqManager, &RequirementsManager::busyChanged, this, &RequirementsWidget::updateServerStatus);
    connect(m_reqManager, &RequirementsManager::connectionError, this, [this](QString error) {
        updateServerStatus();
        QMessageBox::warning(this, tr("Connection error"), tr("Connection failed for this error:\n%1").arg(error));
    });

    bool hasCredentialsStored = loadSavedCredentials();
    if (hasCredentialsStored) {
        setLoginData();
    }

    const QString urlTooltip = tr("Set the Gitlab server URL including the project path");
    ui->urlLabel->setToolTip(urlTooltip);
    ui->urlLineEdit->setToolTip(urlTooltip);
    const QString tokenTooltip = tr(
            "To create a personal access token, go to  Profile > Preferences > Access Tokens \n in your gitlab server");
    ui->tokenLabel->setToolTip(tokenTooltip);
    ui->tokenLineEdit->setToolTip(tokenTooltip);
    connect(ui->createTokenButton, &QPushButton::clicked, this, &RequirementsWidget::openTokenSettingsPage);
    connect(ui->filterButton, &QPushButton::clicked, this, &RequirementsWidget::toggleShowUsedRequirements);
    ui->filterButton->setIcon(QPixmap(":/requirementsresources/icons/filter_icon.svg"));
}

bool RequirementsWidget::loadSavedCredentials()
{
    QSettings settings;
    auto gitlabToken = settings.value(tokenKey(QUrl(m_requirementsUrl).host())).toString();
    if (m_requirementsUrl.isEmpty() || gitlabToken.isEmpty()) {
        return false;
    }

    setUrl(m_requirementsUrl);
    setToken(gitlabToken);
    return true;
}
RequirementsWidget::~RequirementsWidget()
{
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
    QString newUrlHostKey = tokenKey(QUrl(ui->urlLineEdit->text()).host());
    QString oldUrlHostKey = tokenKey(QUrl(m_requirementsUrl).host());
    QSettings settings;
    if (!ui->urlLineEdit->text().isEmpty()) {
        QString gitlabToken = settings.value(newUrlHostKey).toString();
        if ((gitlabToken != ui->tokenLineEdit->text()) || (ui->urlLineEdit->text() != m_requirementsUrl)) {
            settings.remove(oldUrlHostKey);
            settings.setValue(newUrlHostKey, ui->tokenLineEdit->text());
            m_requirementsUrl = ui->urlLineEdit->text().toUtf8();
            ui->createTokenButton->setEnabled(true);
        }
    } else {
        settings.remove(oldUrlHostKey);
        m_requirementsUrl.clear();
        setToken("");
        ui->createTokenButton->setEnabled(false);
    }
    emit requirementsUrlChanged(m_requirementsUrl);
}

void RequirementsWidget::requestRequirements()
{
    m_reqManager->requestAllRequirements();
}

void RequirementsWidget::setLoginData()
{
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

    const bool connectionOk = !m_reqManager->projectID().isEmpty();
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
        ui->allRequirements->setModel(&m_filterModel);
        ui->filterButton->setIcon(QPixmap(":/requirementsresources/icons/filter_icon.svg"));
    } else {
        ui->allRequirements->setModel(&m_checkedModel);
        ui->filterButton->setIcon(QPixmap(":/requirementsresources/icons/disable_filter_icon.svg"));
    }
}

void RequirementsWidget::showNewRequirementDialog() const
{
    QScopedPointer<AddNewRequirementDialog> dialog(new AddNewRequirementDialog());
    dialog->setModal(true);
    auto ret = dialog->exec();
    if (ret == QDialog::Accepted) {
        m_reqManager->createRequirement(dialog->title(), dialog->reqIfId(), dialog->description());
    }
}
}
