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

class ReviewsModel;
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

    void setManager(ReviewsManager *manager);
    void setModel(ReviewsModel *model);

    QUrl url() const;
    void setUrl(const QUrl &url);
    QString token() const;
    void setToken(const QString &token);

public Q_SLOTS:
    void setLoginData();
    void updateServerStatus();

Q_SIGNALS:
    void reviewsUrlChanged(QUrl reviewsUrl);
    void reviewsCredentialsChanged(QUrl url, QString token);

protected Q_SLOTS:
    void onChangeOfCredentials();
    void requestReviews();

private:
    Ui::ReviewsWidget *ui;
    QPointer<ReviewsManager> m_reviewsManager;
    QPointer<ReviewsModel> m_model;
};

} // namespace reviews
