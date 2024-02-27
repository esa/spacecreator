/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "reviewswidget.h"

#include "reviewsmanager.h"
#include "reviewsmodelbase.h"
#include "ui_reviewswidget.h"

#include <QDesktopServices>
#include <QMessageBox>

namespace reviews {
const int kIconSize = 16;

ReviewsWidget::ReviewsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReviewsWidget)
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::clicked, this, &ReviewsWidget::setLoginData);
    connect(ui->credentialWidget, &tracecommon::CredentialWidget::urlChanged, this,
            &ReviewsWidget::onChangeOfCredentials);
    connect(ui->credentialWidget, &tracecommon::CredentialWidget::tokenChanged, this,
            &ReviewsWidget::onChangeOfCredentials);
    connect(ui->allReviews, &QTableView::doubleClicked, this, &ReviewsWidget::openIssueLink);
}

ReviewsWidget::~ReviewsWidget()
{
    delete ui;
}

void ReviewsWidget::setManager(ReviewsManager *manager)
{
    m_reviewsManager = manager;
    connect(m_reviewsManager, &ReviewsManager::projectIDChanged, this, &ReviewsWidget::requestReviews);
    connect(m_reviewsManager, &ReviewsManager::busyChanged, this, &ReviewsWidget::updateServerStatus);
}

void ReviewsWidget::setModel(ReviewsModelBase *model)
{
    m_model = model;
    ui->allReviews->setModel(m_model);
}

/*!
 * Returns the URL to fetch the reviews from
 */
QUrl ReviewsWidget::url() const
{
    return ui->credentialWidget->url();
}

/*!
 * Sets the url to fetch the requirements from
 */
void ReviewsWidget::setUrl(const QUrl &url)
{
    ui->credentialWidget->setUrl(url.toString());
}

/*!
 * Returns the token to authenticate for fetching the reviews
 */
QString ReviewsWidget::token() const
{
    return ui->credentialWidget->token();
}

/*!
 * Sets the Token to authenticate for fetching the requirements
 */
void ReviewsWidget::setToken(const QString &token)
{
    ui->credentialWidget->setToken(token);
}

void ReviewsWidget::setLoginData()
{
    if (m_reviewsManager->isBusy()) {
        return;
    }

    ui->serverStatusLabel->setPixmap({});

    const QUrl currUrl(ui->credentialWidget->url());
    const QString currToken(ui->credentialWidget->token());

    if (currUrl.isEmpty() || currToken.isEmpty()) {
        ui->serverStatusLabel->setPixmap({});
        return;
    }

    if (currUrl == m_reviewsManager->projectUrl() && currToken == m_reviewsManager->token()) {
        m_reviewsManager->requestAllReviews();
        return;
    }

    ui->serverStatusLabel->setToolTip(tr("Checking connection to the server"));
    m_reviewsManager->setCredentials(currUrl.toString(), currToken);
}

void ReviewsWidget::updateServerStatus()
{
    if (!m_reviewsManager) {
        return;
    }

    ui->refreshButton->setEnabled(!m_reviewsManager->isBusy());
    const QCursor busyCursor(Qt::WaitCursor);
    if (m_reviewsManager->isBusy()) {
        if (ui->allReviews->cursor() != busyCursor) {
            ui->allReviews->setCursor(busyCursor);
        }
    } else {
        if (ui->allReviews->cursor() == busyCursor) {
            ui->allReviews->unsetCursor();
        }
    }

    const bool connectionOk = (m_reviewsManager->projectID() != -1);
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

void ReviewsWidget::onChangeOfCredentials()
{
    const QUrl newUrl(ui->credentialWidget->url());
    const QString newToken(ui->credentialWidget->token());
    if (!newUrl.isValid() || newToken.isEmpty()) {
        return;
    }

    Q_EMIT reviewsCredentialsChanged(newUrl, newToken);
    Q_EMIT reviewsUrlChanged(newUrl);
}

void ReviewsWidget::requestReviews()
{
    if (m_reviewsManager) {
        m_reviewsManager->requestAllReviews();
    }
}

void ReviewsWidget::openIssueLink(const QModelIndex &index)
{
    const QString data = index.data(ReviewsModelBase::RoleNames::IssueLinkRole).toString();
    const QUrl issueUrl(data);
    bool ok = QDesktopServices::openUrl(issueUrl);
    if (!ok) {
        QMessageBox::warning(this, tr("Url error"), tr("Error opening the url\n%1").arg(issueUrl.toString()));
    }
}

} // namespace reviews
