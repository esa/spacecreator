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

#include "hwlibraryoption.h"

#include "config/dvhwlibrarywidget.h"
#include "spacecreatorpluginconstants.h"

namespace spctr {

HwLibraryOption::HwLibraryOption()
{
    setId(Constants::SETTINGS_DV_HW_LIBRARY_ID);
    setDisplayName(tr("Deployment HW Library"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *HwLibraryOption::widget()
{
    if (!m_widget) {
        m_widget = new dve::DVHWLibraryWidget;
    }
    return m_widget;
}

void HwLibraryOption::apply()
{
    m_widget->save();
    /// @todo trigger reload
}

void HwLibraryOption::finish()
{
    delete m_widget;
}

}
