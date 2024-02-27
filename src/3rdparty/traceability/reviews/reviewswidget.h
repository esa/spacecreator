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

#pragma once

#include <QPointer>
#include <QWidget>

namespace reviews {

class Review;
class ReviewsModelBase;
class ReviewsManager;

namespace Ui {
class ReviewsWidget;
}

/*!
 * Widget for listing all reviews
 */
class ReviewsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReviewsWidget(QWidget *parent = nullptr);
    ~ReviewsWidget();

    void showButtons(bool show);

    void setManager(ReviewsManager *manager);
    void setModel(ReviewsModelBase *model);

    QUrl url() const;
    virtual void setUrl(const QUrl &url);
    QString token() const;
    virtual void setToken(const QString &token);

public Q_SLOTS:
    void setLoginData();
    void updateServerStatus();

Q_SIGNALS:
    void reviewsUrlChanged(QUrl reviewsUrl);
    void reviewsCredentialsChanged(QUrl url, QString token);
    void reviewAdded(const Review &review);
    void reviewRemoved(const QString &reviewId);

protected Q_SLOTS:
    void onChangeOfCredentials();
    void requestReviews();
    void openIssueLink(const QModelIndex &index);
    void showNewReviewDialog() const;
    void removeReview();

protected:
    Ui::ReviewsWidget *ui;
    QPointer<ReviewsManager> m_reviewsManager;
    QPointer<ReviewsModelBase> m_model;
};

} // namespace reviews
