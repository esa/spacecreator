/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dynamicactionoption.h"

#include "context/action/actionsmanager.h"
#include "context/action/editor/dynactioneditorwidget.h"
#include "spacecreatorpluginconstants.h"

namespace spctr {

DynamicActionOption::DynamicActionOption()
{
    setId(Constants::SETTINGS_IV_ACTIONS_ID);
    setDisplayName(tr("Dynamic actions"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *DynamicActionOption::widget()
{
    if (!m_widget) {
        m_widget = new aadlinterface::DynActionEditorWidget;
        m_widget->init();
    }
    return m_widget;
}

void DynamicActionOption::apply()
{
    m_widget->save();
    aadlinterface::ActionsManager::reload();
}

void DynamicActionOption::finish()
{
    delete m_widget;
}

}
