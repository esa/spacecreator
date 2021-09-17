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

#include "ivlibraryoption.h"

#include "config/ivlibrarywidget.h"
#include "spacecreatorpluginconstants.h"

namespace spctr {

IVLibraryOption::IVLibraryOption()
{
    setId(Constants::SETTINGS_DV_HW_LIBRARY_ID);
    setDisplayName(tr("Interface View Library"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *IVLibraryOption::widget()
{
    if (!m_widget) {
        m_widget = new ive::IVLibraryWidget;
    }
    return m_widget;
}

void IVLibraryOption::apply()
{
    m_widget->save();
}

void IVLibraryOption::finish()
{
    delete m_widget;
}

} // namespace spctr
