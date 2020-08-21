/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

namespace AadlPlugin {
namespace Constants {

const char AADL_MIMETYPE[] = "application/xml";

const char K_AADL_EDITOR_ID[] = "AadlEditor.Editor";
const char C_AADL_EDITOR[] = "AADL Editor";

const char INFO_READ_ONLY[] = "AadlEditor.ReadOnly";

const char C_AADLEDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "AADL Editor");

const char AADL_SHOW_ASN1_ID[] = "AadlEditor.MessageDeclarations";
const char AADL_SHOW_MINIMAP_ID[] = "AadlEditor.Minimap";
const char AADL_SHOW_COMMON_PROPS_ID[] = "AadlEditor.CommonProps";
const char AADL_SHOW_COLOR_SCHEME_ID[] = "AadlEditor.ColorScheme";
const char AADL_SHOW_DYN_CONTEXT_ID[] = "AadlEditor.DynContext";
const char AADL_MENU_ID[] = "AadlEditor.Menu";
}
}
