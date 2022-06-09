/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 Fondazione Bruno Kessler - FBK
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

#include "compastaplugin.h"
#include "slimsnippets.h"

#include <QAction>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <coreplugin/editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <exception>
#include <memory>
#include <messagemanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <utils/fileutils.h>
#include <cstdlib>
#include <cstdio>

using namespace Core;
using namespace Slim::Internal::Completion;

// messages for General Messages GUI
namespace GenMsg {
const QString msgInfo = "INFO (COMPASTA Plugin): %1";
const QString msgWarning = "WARNING (COMPASTA Plugin): %1";
const QString msgError = "ERROR (COMPASTA Plugin): %1";
}
// messages for Events
namespace EvtMsg {
const QString openVerificationGUIClicked = "Open Verification GUI Clicked";
const QString editFaultsClicked = "Edit Faults Clicked";
const QString editContractsClicked = "Edit Contracts Clicked";
const QString editRequirementsClicked = "Edit Requirements Clicked";
const QString editPropertiesClicked = "Edit Properties Clicked";
}

namespace spctr {

CompastaPlugin::CompastaPlugin()
{
    // Do not remove
}

CompastaPlugin::~CompastaPlugin()
{
    // Do not remove
}

auto CompastaPlugin::initialize(const QStringList &arguments, QString *errorString) -> bool
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addCompastaMenu();

    SlimSnippets::registerGroup();

    return true;
}

auto CompastaPlugin::extensionsInitialized() -> void
{
    // Do not remove
}

auto CompastaPlugin::addCompastaMenu() -> void
{
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsCompasta = createActionContainerInTools(tr("COM&PASTA Tools"));

    const auto editFaultsAction = new QAction(tr("Edit &Faults"), this);
    connect(editFaultsAction, &QAction::triggered, [=]() { this->editFaults(); });
    Command *const editFaultsMenuItem = ActionManager::registerAction(editFaultsAction, Constants::COMPASTA_EDIT_FAULTS_ID, allContexts);
    acToolsCompasta->addAction(editFaultsMenuItem);

    const auto editContractsAction = new QAction(tr("Edit &Contracts"), this);
    connect(editContractsAction, &QAction::triggered, [=]() { this->editContracts(); });
    Command *const editContractsMenuItem = ActionManager::registerAction(editContractsAction, Constants::COMPASTA_EDIT_CONTRACTS_ID, allContexts);
    acToolsCompasta->addAction(editContractsMenuItem);

    const auto editRequirementsAction = new QAction(tr("Edit &Requirements"), this);
    connect(editRequirementsAction, &QAction::triggered, [=]() { this->editRequirements(); });
    Command *const editRequirementsMenuItem = ActionManager::registerAction(editRequirementsAction, Constants::COMPASTA_EDIT_REQUIREMENTS_ID, allContexts);
    acToolsCompasta->addAction(editRequirementsMenuItem);

    const auto editPropertiesAction = new QAction(tr("Edit &Properties"), this);
    connect(editPropertiesAction, &QAction::triggered, [=]() { this->editProperties(); });
    Command *const editPropertiesMenuItem = ActionManager::registerAction(editPropertiesAction, Constants::COMPASTA_EDIT_PROPERTIES_ID, allContexts);
    acToolsCompasta->addAction(editPropertiesMenuItem);

    acToolsCompasta->addSeparator();

    const auto verificationGUIAction = new QAction(tr("&Open Verification GUI"), this);
    connect(verificationGUIAction, &QAction::triggered, [=]() { this->verificationGUI(); });
    Command *const verificationGUIMenuItem = ActionManager::registerAction(verificationGUIAction, Constants::COMPASTA_OPEN_VERIFICATION_GUI_ID, allContexts);
    acToolsCompasta->addAction(verificationGUIMenuItem);
}

auto CompastaPlugin::createActionContainerInTools(const QString &title) -> ActionContainer *
{
    ActionContainer *const container = ActionManager::createMenu(Constants::M_TOOLS_COMPASTA);
    QMenu *const compastaMenu = container->menu();
    compastaMenu->setTitle(title);
    compastaMenu->setEnabled(true);

    ActionContainer *const tools = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(container);

    return container;
}

auto CompastaPlugin::projectDir() -> QString
{
    QString prjdir = ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString();
    return prjdir;
}

auto CompastaPlugin::verificationGUI() -> void
{
    MessageManager::write(GenMsg::msgInfo.arg(EvtMsg::openVerificationGUIClicked));
    const QString prog = "compasta-verification-gui";
    QString prjdir = this->projectDir();
    QString command = prog + " " + prjdir;
    system(command.toStdString().c_str());
}

auto CompastaPlugin::editFaults() -> void
{
    MessageManager::write(GenMsg::msgInfo.arg(EvtMsg::editFaultsClicked));
    QString prjdir = this->projectDir();
    QString filename = QString("faults.slim");
    this->openSlimFile(prjdir, filename);
}

auto CompastaPlugin::editContracts() -> void
{
    MessageManager::write(GenMsg::msgInfo.arg(EvtMsg::editContractsClicked));
    QString prjdir = this->projectDir();
    QString filename = QString("contracts.slim");
    this->openSlimFile(prjdir, filename);
}

auto CompastaPlugin::editRequirements() -> void
{
    MessageManager::write(GenMsg::msgInfo.arg(EvtMsg::editRequirementsClicked));
    QString prjdir = this->projectDir();
    QString filename = QString("requirements.slim");
    this->openSlimFile(prjdir, filename);
}

auto CompastaPlugin::editProperties() -> void
{
    MessageManager::write(GenMsg::msgInfo.arg(EvtMsg::editPropertiesClicked));
    QString prjdir = this->projectDir();
    QString filename = QString("properties.slim");
    this->openSlimFile(prjdir, filename);
}

auto CompastaPlugin::createSlimSkeleton(const QString prjdir, const QString filename) -> void
{
    const QString prog = "compasta-create-slim-skeleton";
    QString command = prog + " " + prjdir + " " + filename;
    system(command.toStdString().c_str());
}

auto CompastaPlugin::openSlimFile(const QString prjdir, const QString filename) -> void
{
    if (!QFileInfo::exists(prjdir + "/" + filename)) {
        // CREATE SLIM SKELETON
        this->createSlimSkeleton(prjdir, filename);
    }
    QFile fp(prjdir + "/" + filename);
    // CREATE FILE IF DOESN'T EXIST (I.E. WASN'T GENERATED)
    fp.open(QIODevice::ReadWrite);
    fp.close();
    // OPEN FILE IN QT CREATOR
    EditorManager::openEditor(filename);
}

} // namespace spctr
