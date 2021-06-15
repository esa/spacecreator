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
#include "dveditor.h"
#include "interface/interfacedocument.h"
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

    m_checkInstancesAction = new QAction(QIcon(QLatin1String(":/icons/check_yellow.svg")), tr("Check instances"), this);
    Core::Command *checkInstancesCmd = Core::ActionManager::registerAction(
            m_checkInstancesAction, Constants::CHECK_INSTANCES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(m_checkInstancesAction, &QAction::triggered, this, &SpaceCreatorPlugin::checkInstancesForCurrentEditor);

    m_checkMessagesAction = new QAction(QIcon(QLatin1String(":/icons/check_blue.svg")), tr("Check messages"), this);
    Core::Command *checkMessagesCmd = Core::ActionManager::registerAction(
            m_checkMessagesAction, Constants::CHECK_MESSAGES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(m_checkMessagesAction, &QAction::triggered, this, &SpaceCreatorPlugin::checkMesagesForCurrentEditor);

    m_showMinimapAction =
            new QAction(QIcon(QLatin1String(":/sharedresources/icons/minimap.svg")), tr("Show minimap"), this);
    m_showMinimapAction->setCheckable(true);
    Core::Command *showMinimapCmd = Core::ActionManager::registerAction(
            m_showMinimapAction, Constants::SHOW_MINIMAP_ID, Core::Context(Core::Constants::C_EDIT_MODE));
    connect(m_showMinimapAction, &QAction::triggered, this, &SpaceCreatorPlugin::setMinimapVisible);

    m_showE2EDataflow = new QAction(
            QIcon(QLatin1String(":/tab_interface/toolbar/icns/end_to_end.png")), tr("Show end to end dataflow"), this);
    Core::Command *showE2ECmd = Core::ActionManager::registerAction(
            m_showE2EDataflow, Constants::SHOW_E2E_ID, Core::Context(Core::Constants::C_EDIT_MODE));
    connect(m_showE2EDataflow, &QAction::triggered, this, &SpaceCreatorPlugin::showE2EDataflow);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("SpaceCreator"));
    menu->addAction(showMinimapCmd);
    menu->addAction(showE2ECmd);
    menu->addSeparator();
    menu->addAction(messageDeclCmd);
    menu->addAction(checkInstancesCmd);
    menu->addAction(checkMessagesCmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    // IV
    m_asn1DialogAction = new QAction(
            QIcon(QLatin1String(":/tab_interface/toolbar/icns/asn1.png")), tr("Show ASN1 dialog ..."), this);
    Core::Command *showAsn1Cmd = Core::ActionManager::registerAction(m_asn1DialogAction, Constants::IV_SHOW_ASN1_ID);
    ive::ActionsManager::registerAction(Q_FUNC_INFO, m_asn1DialogAction, "Asn1", "Edit the ASN1 file");
    connect(m_asn1DialogAction, &QAction::triggered, this, &SpaceCreatorPlugin::showAsn1Dialog);
    m_asn1DialogAction->setEnabled(false);

    m_actionSaveSceneRender =
            new QAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/render.svg")), tr("Render Scene..."), this);
    Core::Command *renderCmd = Core::ActionManager::registerAction(m_actionSaveSceneRender, Constants::IV_RENDER_ID);
    ive::ActionsManager::registerAction(
            Q_FUNC_INFO, m_actionSaveSceneRender, "Render", "Save current scene complete render.");
    connect(m_actionSaveSceneRender, &QAction::triggered, this, []() {
        if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
            ivEditor->ivPlugin()->onSaveRenderRequested();
        }
    });

    m_exportSelectedIV = new QAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/export_selected.svg")),
            tr("Export selected entity"), this);
    Core::Command *exportElectedCmd = Core::ActionManager::registerAction(
            m_exportSelectedIV, Constants::IV_EXPORT_SELECTED_ID, Core::Context(Core::Constants::C_EDIT_MODE));
    connect(m_exportSelectedIV, &QAction::triggered, this, &SpaceCreatorPlugin::exportSelectedIV);

    m_exportIVType = new QAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/export_component_type.svg")),
            tr("Export component type"), this);
    Core::Command *exporttypeCmd = Core::ActionManager::registerAction(
            m_exportIVType, Constants::IV_EXPORT_IV_TYPE_ID, Core::Context(Core::Constants::C_EDIT_MODE));
    connect(m_exportIVType, &QAction::triggered, this, &SpaceCreatorPlugin::exportComponentType);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, this,
            &spctr::SpaceCreatorPlugin::updateActions);
    updateActions();

    menu->addSeparator();
    menu->addAction(showAsn1Cmd);
    menu->addAction(renderCmd);
    menu->addAction(exportElectedCmd);
    menu->addAction(exporttypeCmd);
    menu->menu()->setEnabled(true);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    QList<QAction *> mscActions;
    mscActions << m_showMinimapAction << m_checkInstancesAction << m_checkMessagesAction;
    m_mscFactory = new MscEditorFactory(m_projectsManager, mscActions, this);
    QList<QAction *> ivActions;
    ivActions << m_asn1DialogAction << m_showMinimapAction << m_showE2EDataflow << m_exportSelectedIV << m_exportIVType
              << m_actionSaveSceneRender;
    m_ivFactory = new IVEditorFactory(m_projectsManager, ivActions, this);
    QList<QAction *> dvActions;
    m_dvFactory = new DVEditorFactory(m_projectsManager, dvActions, this);

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
    m_ivFactory->editorData()->showMinimap(visible);
}

void SpaceCreatorPlugin::showE2EDataflow()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectsManager->project(ivEditor->ivPlugin());
        if (project) {
            ivEditor->showE2EDataflow(project->allMscFiles());
        }
    }
}

void SpaceCreatorPlugin::showAsn1Dialog()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        ivEditor->showAsn1Dialog();
    }
}

void SpaceCreatorPlugin::exportSelectedIV()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectsManager->project(ivEditor->ivPlugin());
        if (project) {
            ivEditor->ivPlugin()->document()->exportSelectedFunctions();
        }
    }
}

void SpaceCreatorPlugin::exportComponentType()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectsManager->project(ivEditor->ivPlugin());
        if (project) {
            ivEditor->ivPlugin()->document()->exportSelectedType();
        }
    }
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
    m_asn1DialogAction->setEnabled(isIV);
    m_actionSaveSceneRender->setEnabled(isIV);
    m_showMinimapAction->setEnabled(isIV || isMsc);
    m_showE2EDataflow->setEnabled(isIV);
    m_exportSelectedIV->setEnabled(isIV);
    m_exportIVType->setEnabled(isIV);
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
