/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QtGlobal>

namespace spctr {
namespace Constants {

const char MSC_MIMETYPE[] = "text/vnd.msc";

const char K_MSC_EDITOR_ID[] = "MscEditor.Editor";
const char C_MSC_EDITOR[] = "Msc Editor";

const char INFO_READ_ONLY[] = "MscEditor.ReadOnly";

const char C_MSCEDITOR[] = "Qt5.MscEditor";
const char C_MSCEDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "Msc Editor");

const char MESSAGE_DECLARATIONS_ID[] = "MscPlugin.MessageDeclarations";
const char CHECK_INSTANCES_ID[] = "MscPlugin.CheckInstances";
const char CHECK_MESSAGES_ID[] = "MscPlugin.CheckMessages";
const char SHOW_MINIMAP_ID[] = "MscPlugin.Minimap";
const char MENU_ID[] = "MscPlugin.Menu";

}
}
