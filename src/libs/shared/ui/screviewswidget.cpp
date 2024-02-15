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
along with this program. If not, see
<https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "screviewswidget.h"

#include "settingsmanager.h"

#include <QSettings>
#include <QUrl>

namespace shared {
namespace ui {

SCReviewsWidget::SCReviewsWidget(QWidget *parent)
    : reviews::ReviewsWidget(parent)
{
    connect(this, &reviews::ReviewsWidget::reviewsCredentialsChanged, this, &SCReviewsWidget::onCredentialsChange);
}

void SCReviewsWidget::setUrl(const QUrl &url)
{
    reviews::ReviewsWidget::setUrl(url);

    if (url.isEmpty()) {
        return;
    }

    QSettings settings;
    settings.beginGroup(SettingsManager::spaceCreatorGroup());
    const QString &storedToken = settings.value(SettingsManager::tokenKey(url.host())).toString();
    settings.endGroup();

    if (storedToken.isEmpty()) {
        return;
    }

    setToken(storedToken);

    onCredentialsChange(url, storedToken);
}

void SCReviewsWidget::onCredentialsChange(const QUrl &newUrl, const QString &newToken)
{
    if (!newUrl.isValid()) {
        return;
    }

    QUrl reviewsUrl = url();

    const QString &newUrlHostKey = SettingsManager::tokenKey(newUrl.host());
    QSettings settings;
    settings.beginGroup(SettingsManager::spaceCreatorGroup());
    const QString &storedToken = settings.value(newUrlHostKey).toString();

    // If anything changed
    if ((storedToken != newToken) || (newUrl != reviewsUrl)) {
        settings.setValue(newUrlHostKey, newToken);
        reviewsUrl = newUrl.toString();
    }
    settings.endGroup();

    Q_EMIT reviewsUrlChanged(reviewsUrl);

    if (!reviewsUrl.isEmpty() && !newToken.isEmpty()) {
        setLoginData();
    }
}

}
}
