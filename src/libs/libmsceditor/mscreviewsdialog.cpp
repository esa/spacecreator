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

#include "mscreviewsdialog.h"

#include "commands/cmdupdateentityreviews.h"
#include "componentreviewsproxymodel.h"
#include "msccommandsstack.h"
#include "mscentity.h"
#include "reviewsmanager.h"
#include "settingsmanager.h"
#include "shared/ui/screviewswidget.h"
#include "ui_mscreviewsdialog.h"

#include <QSettings>

using namespace reviews;
namespace msc {

MscReviewsDialog::MscReviewsDialog(
        const QUrl &reviewsUrl, msc::MscEntity *entity, MscCommandsStack *undoStack, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MscReviewsDialog)
    , m_entity(entity)
    , m_undoStack(undoStack)
{
    ui->setupUi(this);

    m_reviewsManager = new ReviewsManager(ReviewsManager::REPO_TYPE::GITLAB, this);
    m_reviewsModel = new shared::ComponentReviewsProxyModel(m_reviewsManager, this);
    m_reviewsModel->setAcceptableIds(m_entity->reviews());

    m_reviewsWidget = new ::shared::ui::SCReviewsWidget(this);
    m_reviewsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->contentLayout->addWidget(m_reviewsWidget, 0, 0, 1, 1);
    m_reviewsWidget->setManager(m_reviewsManager);
    m_reviewsWidget->setModel(m_reviewsModel);
    connect(this, &QDialog::accepted, this, &MscReviewsDialog::saveToken);
    connect(m_reviewsManager, &reviews::ReviewsManager::listOfReviews, m_reviewsModel,
            &reviews::ReviewsModelBase::addReviews);
    connect(m_reviewsWidget, &shared::ui::SCReviewsWidget::reviewAdded, this, &MscReviewsDialog::addReviewId);
    connect(m_reviewsWidget, &shared::ui::SCReviewsWidget::reviewRemoved, this, [this](const QString &reviewId) {
        // Remove without undo - as the review is gone on the server
        m_entity->removeReviewID(reviewId);
    });

    connect(m_reviewsWidget, &shared::ui::SCReviewsWidget::reviewsUrlChanged, this, [this](QUrl reviewsUrl) {
        if (reviewsUrl.isValid() && m_reviewsWidget->token().isEmpty()) {
            setUrl(reviewsUrl);
        }
    });
    setUrl(reviewsUrl);
}

MscReviewsDialog::~MscReviewsDialog()
{
    delete ui;
}

void MscReviewsDialog::setUrl(const QUrl &reviewsUrl)
{
    QSettings settings;
    settings.beginGroup(shared::SettingsManager::spaceCreatorGroup());
    m_reviewsWidget->setUrl(reviewsUrl);
    const QString &tokenKey = shared::SettingsManager::tokenKey(reviewsUrl);
    const QString token = settings.value(tokenKey, m_reviewsWidget->token()).toString();
    m_reviewsWidget->setToken(token);
    settings.endGroup();

    if (!token.isEmpty()) {
        m_reviewsWidget->setLoginData();
    }
}

/*!
 * \brief MscReviewsDialog::url is the URL of the project the reviews are fetched from
 */
QUrl MscReviewsDialog::url() const
{
    return m_reviewsWidget->url();
}

/*!
 * \brief MscReviewsDialog::token is the access token to fetch the reviews from the server
 * \sa url
 */
QString MscReviewsDialog::token() const
{
    return m_reviewsWidget->token();
}

void MscReviewsDialog::saveToken()
{
    QSettings settings;
    settings.beginGroup(shared::SettingsManager::spaceCreatorGroup());
    const QString &tokenKey = shared::SettingsManager::tokenKey(m_reviewsWidget->url());
    settings.setValue(tokenKey, m_reviewsWidget->token());
    settings.endGroup();
}

void MscReviewsDialog::addReviewId(const reviews::Review &review)
{
    QStringList reviews = m_entity->reviews();
    reviews.append(review.m_id);
    m_undoStack->push(new cmd::CmdUpdateEntityReviews(m_entity, reviews));
    m_reviewsModel->setAcceptableIds(m_entity->reviews());
}

} // namespace msc
