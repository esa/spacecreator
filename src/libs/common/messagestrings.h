/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include <QString>

/// messages for General Messages GUI
namespace GenMsg {
const QString msgInfo = "INFO: %1";
const QString msgWarning = "WARNING: %1";
const QString msgError = "ERROR: %1";
const QString fileToImportNotSelected = "File to import not selected";
const QString fileHasBeenOverriden = "%1 file has been overriden";
const QString fileHasNotBeenOverriden = "%1 file has not been overriden";
const QString projectIsntOpened = "There is not open project";
const QString toMuchMatLabModelWorkspaceFiles = "Only one workspace file is allowed";
const QString executingMatlabCommand = "Executing MatLab command : %1";
const QString matlabCommandHasExecuted = "MatLab command has executed successfully";
const QString matlabCommandHasFailed = "MatLab command has failed";
const QString ivFileNotSelected = "InterfaceView file not selected";
const QString ivNoFunctionsInIv = "InterfaceView does not contain functions which could be exported";
const QString ivNoFunctionsSelected = "No functions selected to export";
const QString ivNoFunctionBlockName = "No function block name was given";
const QString ivEditorCoreCouldNotBeRead = "Current IV editor core could not be read";
const QString ivEditorCoreNoDocument = "No document in current IV editor core";
const QString ivModelDoesntContainIvFunction = "IV model does not contain IV function";
const QString ivModelShouldContainOnlyOneIvFunction = "Simulink-derived IV model should contain only one IV function";
const QString ivFunctionWillNotBeImported = "%1 - names are the same, Function will not be imported";
const QString ivFunctionSourceCouldNotBeRead = "Source IV function could not be read";
const QString ivFunctionDestinationCouldNotBeRead = "Destination IV function could not be created";
const QString filesExported = "File(s) exported";
const QString filesImported = "File(s) imported";
const QString functionsImported = "Function(s) imported";
const QString ivModelNotRead = "IV model could not be read";
const QString ivModelsNotMerged = "IV models could not be merged";
const QString ivTmpModelNotRead = QString("Temporary %1").arg(ivModelNotRead);
}
