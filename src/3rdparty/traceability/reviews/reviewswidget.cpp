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

#include "addnewreviewdialog.h"
#include "review.h"
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
    ui->removeReviewButton->setEnabled(false);

    connect(ui->refreshButton, &QPushButton::clicked, this, &ReviewsWidget::setLoginData);
    connect(ui->credentialWidget, &tracecommon::CredentialWidget::urlChanged, this,
            &ReviewsWidget::onChangeOfCredentials);
    connect(ui->credentialWidget, &tracecommon::CredentialWidget::tokenChanged, this,
            &ReviewsWidget::onChangeOfCredentials);
    connect(ui->allReviews, &QTableView::doubleClicked, this, &ReviewsWidget::openIssueLink);
    connect(ui->createReviewButton, &QPushButton::clicked, this, &ReviewsWidget::showNewReviewDialog);
    connect(ui->removeReviewButton, &QPushButton::clicked, this, &ReviewsWidget::removeReview);
}

ReviewsWidget::~ReviewsWidget()
{
    delete ui;
}

/*!
 * Hides the create/remove review button
 * \param show when set to false, the buttons are hidden
 */
void ReviewsWidget::showButtons(bool show)
{
    ui->createReviewButton->setVisible(show);
    ui->removeReviewButton->setVisible(show);
}

void ReviewsWidget::setManager(ReviewsManager *manager)
{
    m_reviewsManager = manager;
    connect(m_reviewsManager, &ReviewsManager::projectIDChanged, this, &ReviewsWidget::updateProjectReady);
    connect(m_reviewsManager, &ReviewsManager::busyChanged, this, &ReviewsWidget::updateServerStatus);
    connect(m_reviewsManager, &ReviewsManager::reviewAdded, this, &ReviewsWidget::reviewAdded);
    connect(m_reviewsManager, &ReviewsManager::reviewAdded, this, &ReviewsWidget::requestReviews);
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

    const bool connectionOk = (m_reviewsManager->hasValidProjectID());
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

void ReviewsWidget::updateProjectReady()
{
    if (!m_reviewsManager) {
        ui->createReviewButton->setEnabled(false);
        return;
    }

    ui->createReviewButton->setEnabled(m_reviewsManager->hasValidProjectID());
    requestReviews();
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
    if (m_reviewsManager && m_reviewsManager->hasValidProjectID()) {
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

void ReviewsWidget::showNewReviewDialog() const
{
    QScopedPointer<AddNewReviewDialog> dialog(new AddNewReviewDialog(m_model.get()));
    dialog->setModal(true);
    const auto ret = dialog->exec();
    if (ret == QDialog::Accepted) {
        m_reviewsManager->createReview(
                dialog->title(), dialog->reviewId(), dialog->description(), dialog->testMethod());
    }
}

void ReviewsWidget::removeReview()
{
    const auto &currentIndex = ui->allReviews->selectionModel()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove review"), tr("Are you sure you want to remove the selected review?"),
            QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        const Review &review = m_model->reviewFromIndex(currentIndex);
        const bool ok = m_reviewsManager->removeReview(review);
        if (ok) {
            Q_EMIT reviewRemoved(review.m_id);
        }
    }
}

} // namespace reviews
