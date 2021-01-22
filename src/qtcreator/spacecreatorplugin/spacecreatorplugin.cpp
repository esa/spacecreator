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

#include "aadlmodelstorage.h"
#include "aadlsystemchecks.h"
#include "asn1library.h"
#include "context/action/actionsmanager.h"
#include "deploymentmodelstorage.h"
#include "dv/deploymenteditorfactory.h"
#include "interface/interfacedocument.h"
#include "iv/aadleditordata.h"
#include "iv/aadleditorfactory.h"
#include "iv/aadlqtceditor.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "msc/msceditordata.h"
#include "msc/msceditorfactory.h"
#include "msc/mscqtceditor.h"
#include "msceditor.h"
#include "msceditorcore.h"
#include "msclibrary.h"
#include "mscmodelstorage.h"
#include "mscsystemchecks.h"
#include "sharedlibrary.h"
#include "spacecreatorpluginconstants.h"

#include <QAction>
#include <QMessageBox>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/designmode.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <editormanager/documentmodel.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projecttree.h>
#include <projectexplorer/session.h>
#include <utils/fileutils.h>

void initMscResources()
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    Asn1Acn::initAsn1Library();
    msc::initMscLibrary();
    msc::initMscEditor();
    ive::initIvEditor();
}

using namespace Core;

namespace spctr {

SpaceCreatorPlugin::SpaceCreatorPlugin()
{
    initMscResources();
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

    auto editorManager = Core::EditorManager::instance();

    m_aadlStorage = new AadlModelStorage(this);
    connect(m_aadlStorage, &spctr::AadlModelStorage::editedExternally, this, &spctr::SpaceCreatorPlugin::saveIfNotOpen);
    m_mscStorage = new MscModelStorage(this);
    connect(m_mscStorage, &spctr::MscModelStorage::editedExternally, this, &spctr::SpaceCreatorPlugin::saveIfNotOpen);
    m_deploymentStorage = new DeploymentModelStorage(this);

    m_checks = new MscSystemChecks(this);
    m_checks->setMscStorage(m_mscStorage);
    m_checks->setAadlStorage(m_aadlStorage);
    m_aadlStorage->setChecker(m_checks);

    connect(m_mscStorage, &spctr::MscModelStorage::coreAdded, this, [this](QSharedPointer<msc::MSCEditorCore> mscCore) {
        mscCore->aadlChecker()->setIvCore(m_checks->ivCore());
    });

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
    connect(m_checkInstancesAction, &QAction::triggered, m_checks, &MscSystemChecks::checkInstances);

    m_checkMessagesAction = new QAction(QIcon(QLatin1String(":/icons/check_blue.svg")), tr("Check messages"), this);
    Core::Command *checkMessagesCmd = Core::ActionManager::registerAction(
            m_checkMessagesAction, Constants::CHECK_MESSAGES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(m_checkMessagesAction, &QAction::triggered, m_checks, &MscSystemChecks::checkMessages);

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

    // AADL
    m_asn1DialogAction = new QAction(
            QIcon(QLatin1String(":/tab_interface/toolbar/icns/asn1.png")), tr("Show ASN1 dialog ..."), this);
    Core::Command *showAsn1Cmd = Core::ActionManager::registerAction(m_asn1DialogAction, Constants::AADL_SHOW_ASN1_ID);
    ive::ActionsManager::registerAction(Q_FUNC_INFO, m_asn1DialogAction, "Asn1", "Edit the ASN1 file");
    connect(m_asn1DialogAction, &QAction::triggered, this, &SpaceCreatorPlugin::showAsn1Dialog);
    m_asn1DialogAction->setEnabled(false);

    m_actionSaveSceneRender =
            new QAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/render.svg")), tr("Render Scene..."), this);
    Core::Command *renderCmd = Core::ActionManager::registerAction(m_actionSaveSceneRender, Constants::AADL_RENDER_ID);
    ive::ActionsManager::registerAction(
            Q_FUNC_INFO, m_actionSaveSceneRender, "Render", "Save current scene complete render.");
    connect(m_actionSaveSceneRender, &QAction::triggered, this,
            [this]() { m_checks->ivCore()->onSaveRenderRequested(); });

    connect(editorManager, &Core::EditorManager::currentEditorChanged, this, [&](Core::IEditor *editor) {
        if (editor && editor->document()) {
            const bool isMsc = editor->document()->filePath().toString().endsWith(".msc", Qt::CaseInsensitive);
            const bool isAadl =
                    editor->document()->filePath().toString().endsWith("interfaceview.xml", Qt::CaseInsensitive);
            m_messageDeclarationAction->setEnabled(isMsc);
            m_asn1DialogAction->setEnabled(isAadl);
            m_actionSaveSceneRender->setEnabled(isAadl);
            m_showMinimapAction->setEnabled(isAadl || isMsc);
            m_showE2EDataflow->setEnabled(isAadl);
        }
    });

    menu->addSeparator();
    menu->addAction(showAsn1Cmd);
    menu->addAction(renderCmd);
    menu->menu()->setEnabled(true);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    // asn connection
    connect(ProjectExplorer::ProjectTree::instance(), &ProjectExplorer::ProjectTree::currentProjectChanged, this,
            [this](ProjectExplorer::Project *project) {
                if (project) {
                    m_asnFiles = m_checks->allAsn1Files();
                    connect(project, &ProjectExplorer::Project::fileListChanged, this,
                            &spctr::SpaceCreatorPlugin::checkAsnFileRename, Qt::UniqueConnection);
                }
            });

    // QtCreator stuff
    connect(ProjectExplorer::SessionManager::instance(), &ProjectExplorer::SessionManager::aboutToRemoveProject, this,
            &spctr::SpaceCreatorPlugin::clearProjectData);

    QList<QAction *> mscActions;
    mscActions << m_showMinimapAction << m_checkInstancesAction << m_checkMessagesAction;
    m_mscFactory = new MscEditorFactory(m_mscStorage, mscActions, this);
    QList<QAction *> ivActions;
    ivActions << m_asn1DialogAction << m_showMinimapAction << m_showE2EDataflow << m_actionSaveSceneRender;
    m_aadlFactory = new AadlEditorFactory(m_aadlStorage, m_mscStorage, ivActions, this);
    m_deploymentFactory = new DeploymentEditorFactory(this);

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
    m_aadlFactory->editorData()->showMinimap(visible);
}

void SpaceCreatorPlugin::showE2EDataflow()
{
    if (auto aadlEditor = qobject_cast<spctr::AadlQtCEditor *>(Core::EditorManager::currentEditor())) {
        aadlEditor->showE2EDataflow(m_checks->allMscFiles());
    }
}

void SpaceCreatorPlugin::showAsn1Dialog()
{
    if (auto aadlEditor = qobject_cast<spctr::AadlQtCEditor *>(Core::EditorManager::currentEditor())) {
        aadlEditor->showAsn1Dialog();
    }
}

/*!
   Checks if one asn1 file was renamed. If yes, update the filename in all msc and aadl files.
 */
void SpaceCreatorPlugin::checkAsnFileRename()
{
    QStringList asnFiles = m_checks->allAsn1Files();

    QStringList newAsnFiles;
    for (const QString &file : asnFiles) {
        if (!m_asnFiles.contains(file)) {
            newAsnFiles.append(file);
        }
    }
    QStringList lostAsnFiles;
    for (const QString &file : m_asnFiles) {
        if (!asnFiles.contains(file)) {
            lostAsnFiles.append(file);
        }
    }

    if (newAsnFiles.size() == 1 && lostAsnFiles.size() == 1) {
        for (QSharedPointer<msc::MSCEditorCore> &mscCore : m_checks->allMscCores()) {
            mscCore->renameAsnFile(lostAsnFiles[0], newAsnFiles[0]);
        }
        m_checks->ivCore()->renameAsnFile(lostAsnFiles[0], newAsnFiles[0]);
    }

    m_asnFiles = asnFiles;
}

/*!
   \brief SpaceCreatorPlugin::saveIfNotOpen
   \param core
 */
void SpaceCreatorPlugin::saveIfNotOpen(shared::EditorCore *core)
{
    if (!isOpenInEditor(core)) {
        core->save();
    }
}

/*!
   Clears all data of the given project
 */
void SpaceCreatorPlugin::clearProjectData(ProjectExplorer::Project *project)
{
    if (!project) {
        return;
    }

    const Utils::FileNameList files = project->files(ProjectExplorer::Project::AllFiles);
    for (const Utils::FileName &fileName : files) {
        m_aadlStorage->remove(fileName.toString());
        m_mscStorage->remove(fileName.toString());
    }
}

/*!
   Returns true, if the file of the given /p core is open in an editor
 */
bool SpaceCreatorPlugin::isOpenInEditor(shared::EditorCore *core) const
{
    QList<Core::IDocument *> openDocuments = Core::DocumentModel::openedDocuments();
    for (Core::IDocument *doc : openDocuments) {
        if (doc->filePath().toString() == core->filePath()) {
            return true;
        }
    }
    return false;
}
}
