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

#include "reviewsdialog.h"

#include "settingsmanager.h"
#include "ui_reviewsdialog.h"

#include <QSettings>
#include <QUrl>

namespace spctr {

ReviewsDialog::ReviewsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReviewsDialog)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &ReviewsDialog::saveToken);
}

ReviewsDialog::~ReviewsDialog()
{
    delete ui;
}

void ReviewsDialog::setUrl(const QUrl &reviewsUrl)
{
    QSettings settings;
    settings.beginGroup(shared::SettingsManager::spaceCreatorGroup());
    ui->reviewsWidget->setUrl(reviewsUrl);
    const QString &tokenKey = shared::SettingsManager::tokenKey(reviewsUrl);
    const QString token = settings.value(tokenKey, ui->reviewsWidget->token()).toString();
    ui->reviewsWidget->setToken(token);
    settings.endGroup();
}

QUrl ReviewsDialog::url() const
{
    return ui->reviewsWidget->url();
}

/*!
 * Saves the authentification token in the settings.
 * \note The URL has to be stored outside separately (in the model to save it in the file)
 */
void ReviewsDialog::saveToken()
{
    QSettings settings;
    settings.beginGroup(shared::SettingsManager::spaceCreatorGroup());
    const QString &tokenKey = shared::SettingsManager::tokenKey(ui->reviewsWidget->url());
    settings.setValue(tokenKey, ui->reviewsWidget->token());
    settings.endGroup();
}

} // namespace spctr
