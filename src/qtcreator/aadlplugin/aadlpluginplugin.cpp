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

#include "aadlpluginplugin.h"

#include "aadleditordata.h"
#include "aadleditorfactory.h"
#include "aadlpluginconstants.h"
#include "iveditor.h"
#include "sharedlibrary.h"

#include <QAction>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/designmode.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>

namespace AadlPlugin {

AadlPluginPlugin::AadlPluginPlugin()
{
    shared::initSharedLibrary();
    aadlinterface::initIvEditor();
}

AadlPluginPlugin::~AadlPluginPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool AadlPluginPlugin::initialize(const QStringList &arguments, QString *errorString)
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

    m_asn1DialogAction = new QAction(tr("Show ASN1 dialog ..."), this);
    Core::Command *showAsn1Cmd = Core::ActionManager::registerAction(m_asn1DialogAction, Constants::AADL_SHOW_ASN1_ID);
    connect(m_asn1DialogAction, &QAction::triggered, this, &AadlPluginPlugin::showAsn1Dialog);

    m_asn1DialogAction->setEnabled(false);
    connect(editorManager, &Core::EditorManager::currentEditorChanged, this, [&](Core::IEditor *editor) {
        if (editor && editor->document()) {
            const bool isAadl = editor->document()->filePath().toString().endsWith(".xml", Qt::CaseInsensitive);
            m_asn1DialogAction->setEnabled(isAadl);
        }
    });

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::AADL_MENU_ID);
    menu->menu()->setTitle(tr("AADLPlugin"));
    menu->addAction(showAsn1Cmd);
    menu->menu()->setEnabled(true);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    m_factory = new AadlEditorFactory(this);

    return true;
}

void AadlPluginPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    Core::DesignMode::setDesignModeIsRequired();
}

ExtensionSystem::IPlugin::ShutdownFlag AadlPluginPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void AadlPluginPlugin::showAsn1Dialog()
{
    m_factory->editorData()->showAsn1Dialog();
}

}
