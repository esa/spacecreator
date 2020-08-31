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

#include "mscpluginplugin.h"

#include "aadlchecks.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mscchart.h"
#include "msceditor.h"
#include "msceditordata.h"
#include "msceditorfactory.h"
#include "mscinstance.h"
#include "msclibrary.h"
#include "mscplugin.h"
#include "mscpluginconstants.h"
#include "sharedlibrary.h"

#include <QMessageBox>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/designmode.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

void initMscResources()
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    msc::initMscLibrary();
    msc::initMscEditor();
}

using namespace Core;

namespace MscPlugin {

MscPluginPlugin::MscPluginPlugin()
{
    initMscResources();
}

MscPluginPlugin::~MscPluginPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool MscPluginPlugin::initialize(const QStringList &arguments, QString *errorString)
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

    m_messageDeclarationAction = new QAction(tr("Message declarations ..."), this);
    Core::Command *messageDeclCmd =
            Core::ActionManager::registerAction(m_messageDeclarationAction, Constants::MESSAGE_DECLARATIONS_ID);
    connect(m_messageDeclarationAction, &QAction::triggered, this, &MscPluginPlugin::showMessageDeclarations);

    m_messageDeclarationAction->setEnabled(false);
    connect(editorManager, &Core::EditorManager::currentEditorChanged, this, [&](Core::IEditor *editor) {
        if (editor && editor->document()) {
            const bool isMsc = editor->document()->filePath().toString().endsWith(".msc", Qt::CaseInsensitive);
            m_messageDeclarationAction->setEnabled(isMsc);
        }
    });

    auto action = new QAction(tr("Check instances"), this);
    Core::Command *listAadlCmd = Core::ActionManager::registerAction(
            action, Constants::CHECK_INSTANCES_ID, Core::Context(Core::Constants::C_GLOBAL));
    connect(action, &QAction::triggered, this, &MscPluginPlugin::checkInstances);

    auto showMinimapAction = new QAction(tr("Show minimap"), this);
    showMinimapAction->setCheckable(true);
    Core::Command *showMinimapCmd = Core::ActionManager::registerAction(
            showMinimapAction, Constants::SHOW_MINIMAP_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(showMinimapAction, &QAction::triggered, this, &MscPluginPlugin::setMinimapVisible);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("MscPlugin"));
    menu->addAction(messageDeclCmd);
    menu->addAction(listAadlCmd);
    menu->addAction(showMinimapCmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    m_factory = new MscEditorFactory(this);

    return true;
}

void MscPluginPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    DesignMode::setDesignModeIsRequired();
}

ExtensionSystem::IPlugin::ShutdownFlag MscPluginPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void MscPluginPlugin::showMessageDeclarations()
{
    m_factory->editorData()->editMessageDeclarations(Core::ICore::mainWindow());
}

void MscPluginPlugin::checkInstances()
{
    aadlinterface::IVEditorCore *ivp = ivPlugin();
    if (!ivp) {
        return;
    }

    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result;
    for (msc::MSCPlugin *mplugin : m_factory->editorData()->mscPlugins()) {
        mplugin->aadlChecker()->setIvPlugin(ivp);
        result += mplugin->aadlChecker()->checkInstances();
    }

    if (result.isEmpty()) {
        QMessageBox::information(nullptr, tr("All instaces are ok"), tr("All instaces are ok"));
    } else {
        QString text;
        for (auto item : result) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
        QMessageBox::information(nullptr, tr("Non conforming instance names"), text);
    }
}

void MscPluginPlugin::setMinimapVisible(bool visible)
{
    m_factory->editorData()->setMinimapVisible(visible);
}

/*!
   Returns the AadlPlugin
 */
ExtensionSystem::IPlugin *MscPluginPlugin::aadlPlugin() const
{
    ExtensionSystem::PluginManager *manager = ExtensionSystem::PluginManager::instance();
    QList<ExtensionSystem::PluginSpec *> plugins = manager->plugins();
    for (ExtensionSystem::PluginSpec *pluginSpec : plugins) {
        if (pluginSpec->name() == "AadlPlugin") {
            return pluginSpec->plugin();
        }
    }
    return nullptr;
}

aadlinterface::IVEditorCore *MscPluginPlugin::ivPlugin() const
{
    QStringList aadlFiles = m_factory->editorData()->aadlFiles();
    if (aadlFiles.empty()) {
        qWarning() << "No AADL file in the projec";
        return nullptr;
    }

    IPlugin *plugin = aadlPlugin();
    if (!plugin) {
        qWarning() << "AadlPlugin is not found or loaded";
        return nullptr;
    }

    aadlinterface::IVEditorCore *ivp = nullptr;
    bool ok = QMetaObject::invokeMethod(plugin, "ivPlugin", Qt::DirectConnection,
            Q_RETURN_ARG(aadlinterface::IVEditorCore *, ivp), Q_ARG(QString, aadlFiles.first()));
    if (!ok) {
        qWarning() << "Unable to call 'ivPlugin' from the AadlPlugin";
        return nullptr;
    }
    if (ivp == nullptr) {
        qWarning() << "No IVEditorPlugin for file (maybe the file is not open)" << aadlFiles.first();
        return nullptr;
    }

    return ivp;
}

}
