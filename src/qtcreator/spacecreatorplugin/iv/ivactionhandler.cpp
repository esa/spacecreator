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

#include "ivactionhandler.h"

#include "commands/cmdsetreviewsurl.h"
#include "commandsstack.h"
#include "editorcore.h"
#include "interfacedocument.h"
#include "ivmodel.h"
#include "reviewsdialog.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectimpl.h"

#include <QMenu>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <projectexplorer/projectmanager.h>
#include <utils/icon.h>
#include <utils/utilsicons.h>

namespace spctr {

IVActionHandler::IVActionHandler(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : ActionHandler(spctr::Constants::K_IV_EDITOR_ID, projectManager, parent)
{
    Core::Context contexts;
    contexts.add(spctr::Constants::K_IV_EDITOR_ID);

    auto interfaceViewMenu = Core::ActionManager::createMenu(Constants::IV_MENU_ID);
    auto action = new QAction(tr("Manage Archetypes"), this);
    auto command = Core::ActionManager::registerAction(action, Constants::IV_MANAGE_ARCHETYPES_ID, contexts);
    interfaceViewMenu->addAction(command);

    auto helpLinesAction = shared::EditorCore::createHelpLinesAction(this);
    auto helpLinesCommand =
            Core::ActionManager::registerAction(helpLinesAction, Constants::IV_SHOW_HELP_LINES_ID, contexts);
    interfaceViewMenu->addAction(helpLinesCommand);
    auto gridSnapAction = shared::EditorCore::createSnapToGridAction(this);
    auto gridSnapCommand = Core::ActionManager::registerAction(gridSnapAction, Constants::IV_SNAP_TO_GRID_ID, contexts);
    interfaceViewMenu->addAction(gridSnapCommand);

    interfaceViewMenu->addSeparator();

    auto reviewsAction = new QAction(tr("Show reviews ..."));
    connect(reviewsAction, &QAction::triggered, this, &IVActionHandler::showReviewsDialog);
    auto reviewsCommand = Core::ActionManager::registerAction(reviewsAction, Constants::IV_SHOW_REVIEWS_ID, contexts);
    interfaceViewMenu->addAction(reviewsCommand);

    auto toolsMenu = Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    QMenu *menu = interfaceViewMenu->menu();
    menu->setTitle(tr("Interface view"));
    menu->setEnabled(true);
    toolsMenu->addMenu(interfaceViewMenu);
}

void IVActionHandler::showReviewsDialog()
{
    SpaceCreatorProjectImpl *project = m_projectManager->currentSpaceCreatorProject();
    if (!project || !project->ivCore()) {
        return;
    }

    ivm::IVModel *model = project->ivCore()->document()->objectsModel();
    const QUrl reviewUrl = model->reviewsURL();
    ReviewsDialog dialog(project);
    dialog.setUrl(reviewUrl);
    const int ret = dialog.exec();
    if (ret == QDialog::Accepted) {
        ive::cmd::CommandsStack *commandsStack = project->ivCore()->commandsStack();
        if (dialog.url() != model->reviewsURL()) {
            commandsStack->push(new shared::cmd::CmdSetReviewsUrl(model, dialog.url()));
        }
    }
}
}
