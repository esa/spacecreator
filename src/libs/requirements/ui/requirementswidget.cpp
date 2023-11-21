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

#include "requirementsmanager.h"
#include "requirementsmodel.h"
#include "ui_requirementswidget.h"

#include <QDesktopServices>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QTableView>

using namespace requirement;

namespace {
const int kIconSize = 16;
}

RequirementsWidget::RequirementsWidget(
        const QByteArray &requirementsUrl, RequirementsManager *manager, RequirementsModelBase *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequirementsWidget)
    , mReqManager(manager)
    , m_model(model)
    , m_requirementsUrl(requirementsUrl)
{
    ui->setupUi(this);
    m_filterModel.setDynamicSortFilter(true);
    m_filterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel.setFilterKeyColumn(-1);
    m_filterModel.setSourceModel(m_model);
    m_checkedModel.setFilterKeyColumn(RequirementsModel::CHECKED);
    m_checkedModel.setSourceModel(&m_filterModel);
    ui->allRequirements->setModel(&m_filterModel);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->allRequirements->horizontalHeader()->setStretchLastSection(false);

    connect(ui->allRequirements, &QTableView::doubleClicked, this, &RequirementsWidget::openIssueLink);
    connect(ui->refreshButton, &QPushButton::clicked, this, &RequirementsWidget::onLoginUpdate);
    connect(ui->urlLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    connect(ui->tokenLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);

    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(mReqManager, &RequirementsManager::requestedProjectID, this, [this](const QString &projectID) {
        static const QString anyAssignee("");
        static const QString anyAuthor("");
        mReqManager->requestRequirements(projectID, anyAssignee, anyAuthor);
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/requirementsresources/icons/check_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server is ok"));
    });

    connect(mReqManager, &RequirementsManager::connectionError, this, [this](QString error) {
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/requirementsresources/icons/uncheck_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server failed"));
        QMessageBox::warning(this, tr("Connection error"), tr("Connection failed for this error:\n%1").arg(error));
    });

    loadSavedCredentials();
    onLoginUpdate();

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

void RequirementsWidget::loadSavedCredentials()
{
    setUrl(m_requirementsUrl);
    QSettings settings;
    auto gitlabToken = settings.value(m_requirementsUrl + "__token").toString();
    setToken(gitlabToken);
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
    QSettings settings;
    if (!ui->urlLineEdit->text().isEmpty()) {
        QString gitlabToken = settings.value(ui->urlLineEdit->text() + "__token").toString();
        if ((gitlabToken != ui->tokenLineEdit->text()) || (ui->urlLineEdit->text() != m_requirementsUrl)) {
            settings.remove(m_requirementsUrl + "__token");
            settings.setValue(ui->urlLineEdit->text() + "__token", ui->tokenLineEdit->text());
            m_requirementsUrl = ui->urlLineEdit->text().toUtf8();
            ui->createTokenButton->setEnabled(true);
        }
    } else {
        settings.remove(m_requirementsUrl + "__token");
        m_requirementsUrl.clear();
        setToken("");
        ui->createTokenButton->setEnabled(false);
    }
    emit requirementsUrlChanged(m_requirementsUrl);
}

void RequirementsWidget::onLoginUpdate()
{
    ui->serverStatusLabel->setPixmap({});

    QUrl api_url;
    api_url.setScheme("https");
    api_url.setHost(QUrl(ui->urlLineEdit->text()).host());
    api_url.setPath("/api/v4/");

    if (!api_url.isValid() || ui->tokenLineEdit->text().isEmpty()) {
        return;
    }

    mReqManager->setCredentials(api_url.toString(), ui->tokenLineEdit->text());

    auto projectName = QUrl(ui->urlLineEdit->text()).path().split("/").last();
    mReqManager->requestProjectID(projectName);

    ui->serverStatusLabel->setToolTip(tr("Checking connection to the server"));
}

void RequirementsWidget::openIssueLink(const QModelIndex &index)
{
    QDesktopServices::openUrl(index.data(RequirementsModel::RoleNames::IssueLinkRole).toString());
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
