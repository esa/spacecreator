/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvactionhandler.h"

#include "editorcore.h"
#include "spacecreatorpluginconstants.h"

#include <QMenu>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <utils/icon.h>
#include <utils/utilsicons.h>

namespace spctr {

DVActionHandler::DVActionHandler(QObject *parent)
    : ActionHandler(spctr::Constants::K_DV_EDITOR_ID, parent)
{
    Core::Context contexts;
    contexts.add(spctr::Constants::K_DV_EDITOR_ID);

    auto viewMenu = Core::ActionManager::createMenu(Constants::DV_MENU_ID);
    auto helpLinesAction = shared::EditorCore::createHelpLinesAction(this);
    auto helpLinesCommand =
            Core::ActionManager::registerAction(helpLinesAction, Constants::DV_SHOW_HELP_LINES_ID, contexts);
    viewMenu->addAction(helpLinesCommand);

    auto gridSnapAction = shared::EditorCore::createSnapToGridAction(this);
    auto gridSnapCommand = Core::ActionManager::registerAction(gridSnapAction, Constants::DV_SNAP_TO_GRID_ID, contexts);
    viewMenu->addAction(gridSnapCommand);

    auto toolsMenu = Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    QMenu *menu = viewMenu->menu();
    menu->setTitle(tr("Deployment view"));
    menu->setEnabled(true);
    toolsMenu->addMenu(viewMenu);
}

}
