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

#include <QString>
#include <QtGlobal>

namespace spctr {
namespace Constants {

const char DV_MIMETYPE[] = "application/dvxml";
const char IV_MIMETYPE[] = "application/xml";
const char MSC_MIMETYPE[] = "text/vnd.msc";
const char SCL_MIMETYPE[] = "text/x-scl";

const char INFO_READ_ONLY[] = "SpaceCreator.ReadOnly";

const char K_DV_EDITOR_ID[] = "SpaceCreator.DVEditor";
const char K_IV_EDITOR_ID[] = "SpaceCreator.IVEditor";
const char K_MSC_EDITOR_ID[] = "SpaceCreator.MscEditor";
const char K_SCL_EDITOR_ID[] = "SpaceCreator.SclEditor";

const char C_DVEDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "DV Editor");
const char C_IVEDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "IV Editor");
const char C_MSCEDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "Msc Editor");
const char C_SCLEDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "Scl Editor");

// Actions
const char REMOVE_SELECTED_ELEMENTS[] = "SpaceCreator.RemoveSelectedElements";

// Menu
const char MENU_ID[] = "SpaceCreator.Menu";
const char SHOW_MINIMAP_ID[] = "SpaceCreator.Minimap";
const char SHOW_E2E_ID[] = "SpaceCreator.E2EDataflow";
const char MESSAGE_DECLARATIONS_ID[] = "MscEditor.MessageDeclarations";
const char CHECK_INSTANCES_ID[] = "MscEditor.CheckInstances";
const char CHECK_MESSAGES_ID[] = "MscEditor.CheckMessages";
const char CHECK_DV_FUNCTIONS_ID[] = "DvEditor.CheckFunctions";
const char CHECK_DV_MESSAGES_ID[] = "DvEditor.CheckMessages";
const char IV_SHOW_ASN1_ID[] = "IVEditor.ShowAsn1";
const char IV_RENDER_ID[] = "IVEditor.RenderToFile";
const char IV_SHOW_COMMON_PROPS_ID[] = "IVEditor.CommonProps";
const char IV_SHOW_COLOR_SCHEME_ID[] = "IVEditor.ColorScheme";
const char IV_SHOW_DYN_CONTEXT_ID[] = "IVEditor.DynContext";
const char IV_EXPORT_SELECTED_ID[] = "IVEditor.ExportSelected";
const char IV_EXPORT_IV_TYPE_ID[] = "IVEditor.ExportComponentType";
const char MSC_HELP_ID[] = "MscEditor.Help";
const char IV_HELP_ID[] = "IVEditor.Help";
const char IV_MENU_ID[] = "InterfaceView.Tools.Menu";
const char IV_MANAGE_ARCHETYPES_ID[] = "InterfaceView.ManageArchetypes";
const char IV_SHOW_HELP_LINES_ID[] = "InterfaceView.ShowHelpLines";
const char IV_SNAP_TO_GRID_ID[] = "InterfaceView.SnapToGrid";
const char IV_SHOW_REVIEWS_ID[] = "InterfaceView.ShowReviews";
const char DV_MENU_ID[] = "DeploymentView.Tools.Menu";
const char DV_SHOW_HELP_LINES_ID[] = "DeploymentView.ShowHelpLines";
const char DV_SNAP_TO_GRID_ID[] = "DeploymentView.SnapToGrid";

// Settings
const char SETTINGS_CATEGORY_DISPLAY[] = QT_TRANSLATE_NOOP("SpaceCreator", "SpaceCreator");
const char SETTINGS_ASN1_ID[] = "A.SpaceCreator.Asn1";
const char SETTINGS_IV_COLOR_ID[] = "B.SpaceCreator.IVColor";
const char SETTINGS_IV_ACTIONS_ID[] = "C.SpaceCreator.IVActions";
const char SETTINGS_IV_PROPERTIES_ID[] = "D.SpaceCreator.IVProperties";
const char SETTINGS_DV_HW_LIBRARY_ID[] = "E.SpaceCreator.HWLibrary";
const char SETTINGS_IV_LIBRARY_ID[] = "F.SpaceCreator.IVLibrary";
const char SETTINGS_TMC_ID[] = "G.SpaceCreator.TMC";
const char SETTINGS_CATEGORY[] = "K.SpaceCreator";
const QString SETTINGS_ASN_COMPILER_KEY = { "SpaceCreator/asn1compiler" };
const QString SETTINGS_ASN_COMPILER_PARAMETER_KEY = { "SpaceCreator/asn1compilerparameter" };
const QString SETTINGS_ASN_COMPILER_TIMEOUT = { "SpaceCreator/asn1compilertimeout" };

// Context
const char SCL_BASE_CONTEXT[] = "Scl.BaseContext";

}
}
