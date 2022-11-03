/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "libraries.h"

#include <QWidget>
#include <QRegularExpression>

#include <asn1acnconstants.h>
#include <tr.h>

#include "librarieswidget.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::OptionsPages;

Libraries::Libraries(Settings::LibrariesPtr settings)
    : m_settings(settings)
    , m_widget(nullptr)
{
    setId(Constants::LIBRARIES_SETTINGS_ID);
    setDisplayName(Tr::tr("Libraries"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(Tr::tr(Constants::SETTINGS_CATEGORY_DISPLAY));
    setCategoryIcon(
        Utils::Icon({{":/asn1acn/images/options_category.png", Utils::Theme::PanelTextColorDark}},
                    Utils::Icon::Tint));
}

#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
bool Libraries::matches(const QString &searchKeyWord) const
{
    const QStringList keywords{"asn1", "asn.1", "acn", "libraries", "components"};
    for (const auto &keyword : keywords)
        if (keyword.contains(searchKeyWord, Qt::CaseInsensitive))
            return true;
    return Core::IOptionsPage::matches(searchKeyWord);
}
#else
bool Libraries::matches(const QString &searchKeyWord) const
{
    const QStringList keywords{"asn1", "asn.1", "acn", "libraries", "components"};
    for (const auto &keyword : keywords)
        if (keyword.contains(searchKeyWord, Qt::CaseInsensitive))
            return true;
    return Core::IOptionsPage::matches(QRegularExpression(searchKeyWord));
}
#endif

QWidget *Libraries::widget()
{
    if (!m_widget) {
        m_widget = new LibrariesWidget;
        m_widget->setDetectedLibPaths(m_settings->detectedLibPaths());
        m_widget->setManualLibPaths(m_settings->manualLibPaths());
    }
    return m_widget;
}

void Libraries::apply()
{
    if (!m_widget)
        return;
    m_settings->setManualLibPaths(m_widget->manualLibPaths());
    m_settings->changed();
    Settings::save(m_settings);
}

void Libraries::finish()
{
    delete m_widget;
}
