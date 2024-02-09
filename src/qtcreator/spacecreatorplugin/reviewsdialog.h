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
#include <QUrl>

namespace reviews {

class ReviewsModel;
class ReviewsManager;
}

namespace spctr {

namespace Ui {
class ReviewsDialog;
}

class ReviewsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewsDialog(QWidget *parent = nullptr);
    ~ReviewsDialog();

    void setUrl(const QUrl &reviewsUrl);
    QUrl url() const;

private Q_SLOTS:
    void saveToken();

private:
    Ui::ReviewsDialog *ui;

    reviews::ReviewsManager *m_reviewsManager = nullptr;
    reviews::ReviewsModel *m_reviewsModel = nullptr;
};

} // namespace spctr
