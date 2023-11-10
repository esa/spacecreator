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

#include "requirement.h"
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

RequirementsWidget::RequirementsWidget(QByteArray requirementsUrl, QStringList requirementsIDs, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequirementsWidget)
    , mReqManager(RequirementsManager::REPO_TYPE::GITLAB)
    , m_requirementsUrl(requirementsUrl)

{
    ui->setupUi(this);
    m_filterModel.setDynamicSortFilter(true);
    m_filterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel.setFilterKeyColumn(-1);
    m_filterModel.setSourceModel(&m_model);
    ui->allRequirements->setModel(&m_filterModel);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->allRequirements->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->allRequirements->horizontalHeader()->setStretchLastSection(false);
    m_model.setSelectedRequirementsIDs(requirementsIDs);
    connect(ui->allRequirements, &QTableView::doubleClicked, this, &RequirementsWidget::openIssueLink);
    connect(&m_model, &requirement::RequirementsModel::dataChanged,
            [this](const QModelIndex &topLeft, const QModelIndex &BottomRight, const QList<int> &roles) {
                if (topLeft != BottomRight) {
                    return;
                }

                if (topLeft.column() == requirement::RequirementsModel::CHECKED && roles.contains(Qt::CheckStateRole)) {
                    auto index = m_model.index(topLeft.row(), requirement::RequirementsModel::REQUIREMENT_ID);
                    auto requirementID = m_model.data(index, Qt::DisplayRole).toString();
                    auto checked = m_model.data(topLeft, Qt::CheckStateRole).toBool();

                    emit requirementSelected(requirementID, checked);
                }
            });
    connect(ui->refreshButton, &QPushButton::clicked, this, &RequirementsWidget::onLoginUpdate);
    connect(ui->urlLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);
    connect(ui->tokenLineEdit, &QLineEdit::editingFinished, this, &RequirementsWidget::onChangeOfCredentials);

    connect(ui->filterLineEdit, &QLineEdit::textChanged, &m_filterModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(ui->clearFilterButton, &QPushButton::clicked, ui->filterLineEdit, &QLineEdit::clear);
    connect(&mReqManager, &RequirementsManager::requestedProjectID, this, [this](const QString &projectID) {
        static const QString anyAssignee("");
        static const QString anyAuthor("");
        mReqManager.requestRequirements(projectID, anyAssignee, anyAuthor);
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/requirementsresources/icons/check_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server is ok"));
    });

    connect(&mReqManager, &RequirementsManager::listOfRequirements, &m_model,
            &requirement::RequirementsModel::addRequirements);
    connect(&mReqManager, &RequirementsManager::connectionError, this, [this](QString error) {
        ui->serverStatusLabel->setPixmap(
                QPixmap(":/requirementsresources/icons/uncheck_icon.svg").scaled(kIconSize, kIconSize));
        ui->serverStatusLabel->setToolTip(tr("Connection to the server failed"));
        QMessageBox::warning(this, tr("Connection error"), tr("Connection failed for this errror:\n%1").arg(error));
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

void RequirementsWidget::setUrl(const QString &url)
{
    ui->urlLineEdit->setText(url);
}

void RequirementsWidget::setToken(const QString &token)
{
    ui->tokenLineEdit->setText(token);
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
    m_model.clear();
    QUrl api_url;
    api_url.setScheme("https");
    api_url.setHost(QUrl(ui->urlLineEdit->text()).host());
    api_url.setPath("/api/v4/");

    mReqManager.setCredentials(api_url.toString(), ui->tokenLineEdit->text());

    auto projectName = QUrl(ui->urlLineEdit->text()).path().split("/").last();
    mReqManager.requestProjectID(projectName);

    ui->serverStatusLabel->setPixmap({});
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
