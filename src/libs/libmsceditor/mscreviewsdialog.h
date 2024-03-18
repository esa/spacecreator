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

#include <QDialog>
#include <QPointer>
#include <QUrl>

namespace shared {
class ComponentReviewsProxyModel;
namespace ui {
class SCReviewsWidget;
}
}

namespace reviews {
class Review;
class ReviewsManager;
}

namespace msc {
class MscCommandsStack;
class MscEntity;

namespace Ui {
class MscReviewsDialog;
}

/*!
 * \brief The MscReviewsDialog class is a lialog to edit the linked reviews for a single entity
 */
class MscReviewsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MscReviewsDialog(const QUrl &reviewsUrl, msc::MscEntity *entity, msc::MscCommandsStack *undoStack,
            QWidget *parent = nullptr);
    ~MscReviewsDialog();

    void setUrl(const QUrl &reviewsUrl);
    QUrl url() const;
    QString token() const;

private Q_SLOTS:
    void saveToken();
    void addReviewId(const reviews::Review &review);

private:
    Ui::MscReviewsDialog *ui;
    shared::ui::SCReviewsWidget *m_reviewsWidget = nullptr;
    reviews::ReviewsManager *m_reviewsManager = nullptr;
    shared::ComponentReviewsProxyModel *m_reviewsModel = nullptr;
    QPointer<msc::MscEntity> m_entity;
    QPointer<msc::MscCommandsStack> m_undoStack;
};

} // namespace msc
