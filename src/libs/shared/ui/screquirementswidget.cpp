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

#include "screquirementswidget.h"

#include "requirementsmanager.h"
#include "settingsmanager.h"

#include <QHeaderView>
#include <QSettings>
#include <QUrl>

namespace shared {
namespace ui {

SCRequirementsWidget::SCRequirementsWidget(const QString &requirementsUrl, requirement::RequirementsManager *manager,
        requirement::RequirementsModelBase *model, QWidget *parent)
    : requirement::RequirementsWidget(parent)
{
    if (!requirementsUrl.isEmpty()) {
        manager->setCredentials(requirementsUrl, manager->token());
    }
    setManager(manager);
    setModel(model);

    connect(this, &requirement::RequirementsWidget::requirementsCredentialsChanged, this,
            &SCRequirementsWidget::onCredentialsChange);

    const bool hasCredentialsStored = loadSavedCredentials();
    if (hasCredentialsStored) {
        setLoginData();
    }

    loadSavedRequirementsTableGeometry();
}

SCRequirementsWidget::~SCRequirementsWidget()
{
    QSettings settings;
    settings.beginGroup(shared::SettingsManager::spaceCreatorGroup());
    QByteArray allRequirementsHeaderState = horizontalTableHeader()->saveState();
    settings.setValue("AllRequirementsHeaderState", allRequirementsHeaderState);
    settings.endGroup();
}

bool SCRequirementsWidget::loadSavedRequirementsTableGeometry()
{
    QSettings settings;
    settings.beginGroup(shared::SettingsManager::spaceCreatorGroup());
    auto allRequirementsHeaderState = settings.value("AllRequirementsHeaderState");
    settings.endGroup();
    if (allRequirementsHeaderState.isValid()) {
        horizontalTableHeader()->restoreState(allRequirementsHeaderState.toByteArray());
        return true;
    }
    return false;
}

bool SCRequirementsWidget::loadSavedCredentials()
{
    QSettings settings;
    settings.beginGroup(SettingsManager::spaceCreatorGroup());
    const auto &storedToken = settings.value(SettingsManager::tokenKey(QUrl(m_requirementsUrl).host())).toString();
    settings.endGroup();

    if (m_requirementsUrl.isEmpty() || storedToken.isEmpty()) {
        return false;
    }

    setUrl(m_requirementsUrl);
    setToken(storedToken);

    return true;
}

void SCRequirementsWidget::onCredentialsChange(const QUrl &newUrl, const QString &newToken)
{
    if (!newUrl.isValid()) {
        return;
    }

    const QString &newUrlHostKey = SettingsManager::tokenKey(newUrl.host());
    QSettings settings;
    settings.beginGroup(SettingsManager::spaceCreatorGroup());
    const QString &storedToken = settings.value(newUrlHostKey).toString();

    // If anything changed
    if ((storedToken != newToken) || (newUrl.toString() != m_requirementsUrl)) {
        settings.setValue(newUrlHostKey, newToken);
        m_requirementsUrl = newUrl.toString();
    }
    settings.endGroup();

    Q_EMIT requirementsUrlChanged(m_requirementsUrl);

    if (!m_requirementsUrl.isEmpty() && !newToken.isEmpty()) {
        setLoginData();
    }
}

}
}
