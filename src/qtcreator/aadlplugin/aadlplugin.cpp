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

#include "aadlplugin.h"

#include "aadleditordata.h"
#include "aadleditorfactory.h"
#include "aadlpluginconstants.h"
#include "asn1library.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "sharedlibrary.h"

#include <QAction>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/designmode.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>

namespace spctr {

AADLPlugin::AADLPlugin()
{
    Asn1Acn::initAsn1Library();
    aadlinterface::initIvEditor();
    shared::initSharedLibrary();
}

AADLPlugin::~AADLPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool AADLPlugin::initialize(const QStringList &arguments, QString *errorString)
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
    connect(m_asn1DialogAction, &QAction::triggered, this, &AADLPlugin::showAsn1Dialog);

    m_minimapWidgetAction = new QAction(tr("Show minimap"), this);
    m_minimapWidgetAction->setCheckable(true);
    Core::Command *showMinimapCmd = Core::ActionManager::registerAction(
            m_minimapWidgetAction, Constants::AADL_SHOW_MINIMAP_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(m_minimapWidgetAction, &QAction::triggered, this, &AADLPlugin::showMinimap);

    m_asn1DialogAction->setEnabled(false);
    connect(editorManager, &Core::EditorManager::currentEditorChanged, this, [&](Core::IEditor *editor) {
        if (editor && editor->document()) {
            const bool isAadl =
                    editor->document()->filePath().toString().endsWith("interfaceeditor.xml", Qt::CaseInsensitive);
            m_asn1DialogAction->setEnabled(isAadl);
        }
    });

    QAction *actCommonProps = new QAction(tr("Common Properties"), this);
    Core::Command *showCommonPropsCmd = Core::ActionManager::registerAction(
            actCommonProps, Constants::AADL_SHOW_COMMON_PROPS_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(actCommonProps, &QAction::triggered, this, &AADLPlugin::onAttributesManagerRequested);

    QAction *actColorScheme = new QAction(tr("Color Scheme"), this);
    Core::Command *showColorSchemeCmd = Core::ActionManager::registerAction(
            actColorScheme, Constants::AADL_SHOW_COLOR_SCHEME_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(actColorScheme, &QAction::triggered, this, &AADLPlugin::onColorSchemeMenuInvoked);

    QAction *actDynContext = new QAction(tr("Context Actions"), this);
    Core::Command *showDynContextCmd = Core::ActionManager::registerAction(
            actDynContext, Constants::AADL_SHOW_DYN_CONTEXT_ID, Core::Context(Core::Constants::C_DESIGN_MODE));
    connect(actDynContext, &QAction::triggered, this, &AADLPlugin::onDynContextEditorMenuInvoked);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::AADL_MENU_ID);
    menu->menu()->setTitle(tr("AADLPlugin"));
    menu->addAction(showAsn1Cmd);
    menu->addAction(showMinimapCmd);
    menu->addAction(showCommonPropsCmd);
    menu->addAction(showColorSchemeCmd);
    menu->addAction(showDynContextCmd);
    menu->menu()->setEnabled(true);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    m_factory = new AadlEditorFactory(this);
    connect(m_factory, &AadlEditorFactory::aadlDataLoaded, this, &AADLPlugin::aadlDataLoaded);

    return true;
}

void AADLPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    Core::DesignMode::setDesignModeIsRequired();
}

ExtensionSystem::IPlugin::ShutdownFlag AADLPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void AADLPlugin::showAsn1Dialog()
{
    m_factory->editorData()->showAsn1Dialog();
}

void AADLPlugin::showMinimap(bool visible)
{
    m_factory->editorData()->showMinimap(visible);
}

void AADLPlugin::onAttributesManagerRequested()
{
    m_factory->editorData()->onAttributesManagerRequested();
}

void AADLPlugin::onColorSchemeMenuInvoked()
{
    m_factory->editorData()->onColorSchemeMenuInvoked();
}

void AADLPlugin::onDynContextEditorMenuInvoked()
{
    m_factory->editorData()->onDynContextEditorMenuInvoked();
}

/*!
   Returns the IV plugin for the given file
 */
QSharedPointer<aadlinterface::IVEditorCore> AADLPlugin::ivPlugin(const QString &fileName) const
{
    return m_factory->editorData()->ivPlugin(fileName);
}

}
