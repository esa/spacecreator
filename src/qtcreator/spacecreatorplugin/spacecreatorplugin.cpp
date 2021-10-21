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

#include "spacecreatorplugin.h"

#include "context/action/actionsmanager.h"
#include "dv/dveditorfactory.h"
#include "dv/dvqtceditor.h"
#include "dveditor.h"
#include "dvlibrary.h"
#include "dvsystemchecks.h"
#include "interfacedocument.h"
#include "iv/iveditordata.h"
#include "iv/iveditorfactory.h"
#include "iv/ivqtceditor.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "msc/msceditordata.h"
#include "msc/msceditorfactory.h"
#include "msc/mscqtceditor.h"
#include "msceditor.h"
#include "msceditorcore.h"
#include "msclibrary.h"
#include "mscsystemchecks.h"
#include "sharedlibrary.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>

void initSpaceCreatorResources()
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    msc::initMscLibrary();
    msc::initMscEditor();
    ive::initIVEditor();
    ivm::initIVLibrary();
    dve::initDvEditor();
    dvm::initDVLibrary();
}

using namespace Core;

namespace spctr {

SpaceCreatorPlugin::SpaceCreatorPlugin()
{
    initSpaceCreatorResources();
}

SpaceCreatorPlugin::~SpaceCreatorPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool SpaceCreatorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_projectsManager = new SpaceCreatorProjectManager(this);

    // MSC
    m_messageDeclarationAction =
            new QAction(QIcon(QLatin1String(":/icons/message_declarations.svg")), tr("Message declarations ..."), this);
    m_messageDeclarationAction->setEnabled(false);
    Core::Command *messageDeclCmd =
            Core::ActionManager::registerAction(m_messageDeclarationAction, Constants::MESSAGE_DECLARATIONS_ID);
    connect(m_messageDeclarationAction, &QAction::triggered, this, &SpaceCreatorPlugin::showMessageDeclarations);

    m_checkInstancesAction =
            new QAction(QIcon(QLatin1String(":/sharedresources/check_yellow.svg")), tr("Check instances"), this);
    Core::Command *checkInstancesCmd = Core::ActionManager::registerAction(
            m_checkInstancesAction, Constants::CHECK_INSTANCES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(m_checkInstancesAction, &QAction::triggered, this, &SpaceCreatorPlugin::checkInstancesForCurrentEditor);

    m_checkMessagesAction =
            new QAction(QIcon(QLatin1String(":/sharedresources/check_blue.svg")), tr("Check messages"), this);
    Core::Command *checkMessagesCmd = Core::ActionManager::registerAction(
            m_checkMessagesAction, Constants::CHECK_MESSAGES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(m_checkMessagesAction, &QAction::triggered, this, &SpaceCreatorPlugin::checkMesagesForCurrentEditor);

    m_showMinimapAction =
            new QAction(QIcon(QLatin1String(":/sharedresources/icons/minimap.svg")), tr("Show minimap"), this);
    m_showMinimapAction->setCheckable(true);
    Core::Command *showMinimapCmd = Core::ActionManager::registerAction(
            m_showMinimapAction, Constants::SHOW_MINIMAP_ID, Core::Context(Core::Constants::C_EDIT_MODE));
    connect(m_showMinimapAction, &QAction::triggered, this, &SpaceCreatorPlugin::setMinimapVisible);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("SpaceCreator"));
    menu->addAction(showMinimapCmd);
    menu->addSeparator();
    menu->addAction(messageDeclCmd);
    menu->addAction(checkInstancesCmd);
    menu->addAction(checkMessagesCmd);
    menu->addSeparator();
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, this,
            &spctr::SpaceCreatorPlugin::updateActions);
    updateActions();

    menu->menu()->setEnabled(true);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    std::function<QStringList()> deploymentFilesCallback = [this]() {
        if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
            SpaceCreatorProjectImpl *project = m_projectsManager->project(ivEditor->ivPlugin());
            if (project) {
                return project->allDVFiles();
            }
        }
        return QStringList();
    };
    ive::ActionsManager::registerDeploymentFilesCallback(deploymentFilesCallback);

    QList<QAction *> mscActions;
    mscActions << m_showMinimapAction << m_checkInstancesAction << m_checkMessagesAction;
    m_mscFactory = new MscEditorFactory(m_projectsManager, mscActions, this);
    m_ivFactory = new IVEditorFactory(m_projectsManager, this);
    m_dvFactory = new DVEditorFactory(m_projectsManager, this);

    addHelp();

    return true;
}

void SpaceCreatorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag SpaceCreatorPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

/*!
   Opens the message declaration dialog for the current msc file
 */
void SpaceCreatorPlugin::showMessageDeclarations()
{
    if (auto mscEditor = qobject_cast<spctr::MscQtCEditor *>(Core::EditorManager::currentEditor())) {
        mscEditor->mscEditorCore()->openMessageDeclarationEditor(Core::ICore::mainWindow());
    }
}

void SpaceCreatorPlugin::setMinimapVisible(bool visible)
{
    m_mscFactory->editorData()->setMinimapVisible(visible);
}

/*!
   Update the actiones enabled/disabled to represent the current editor
 */
void SpaceCreatorPlugin::updateActions()
{
    bool isMsc = false;
    bool isIV = false;
    Core::IEditor *editor = Core::EditorManager::currentEditor();
    if (editor && editor->document()) {
        isMsc = editor->document()->filePath().toString().endsWith(".msc", Qt::CaseInsensitive);
        isIV = editor->document()->filePath().toString().endsWith("interfaceview.xml", Qt::CaseInsensitive);
    }
    m_messageDeclarationAction->setEnabled(isMsc);
    m_showMinimapAction->setEnabled(isIV || isMsc);
}

void SpaceCreatorPlugin::checkInstancesForCurrentEditor()
{
    if (auto mscEditor = qobject_cast<spctr::MscQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectsManager->project(mscEditor->mscEditorCore());
        if (project) {
            project->mscChecks()->checkInstances();
        }
    }
}

void SpaceCreatorPlugin::checkMesagesForCurrentEditor()
{
    if (auto mscEditor = qobject_cast<spctr::MscQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectsManager->project(mscEditor->mscEditorCore());
        if (project) {
            project->mscChecks()->checkMessages();
        }
    }
}

void SpaceCreatorPlugin::addHelp()
{
    Core::ActionContainer *actions = Core::ActionManager::actionContainer(Core::Constants::M_HELP);
    Core::Context allContexts(
            Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    auto mscHelpAction = new QAction(tr("MSC editor help"), this);
    connect(mscHelpAction, &QAction::triggered, this, []() {
        msc::MSCEditorCore core;
        core.showHelp();
    });
    Core::Command *showMscHelp =
            Core::ActionManager::registerAction(mscHelpAction, Constants::MSC_HELP_ID, allContexts);
    actions->addAction(showMscHelp);

    auto iveHelpAction = new QAction(tr("Interface editor help"), this);
    connect(iveHelpAction, &QAction::triggered, this, []() {
        ive::IVEditorCore core;
        core.showHelp();
    });
    Core::Command *showIveHelp = Core::ActionManager::registerAction(iveHelpAction, Constants::IV_HELP_ID, allContexts);
    actions->addAction(showIveHelp);
}

}
