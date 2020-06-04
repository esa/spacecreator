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

#include "aadleditorfactory.h"
#include "iveditor.h"
#include "sharedlibrary.h"

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/designmode.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>

namespace AadlPlugin {

AadlPlugin::AadlPlugin()
{
    shared::initSharedLibrary();
    aadlinterface::initIvEditor();
}

AadlPlugin::~AadlPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool AadlPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    (void)new AadlEditorFactory(this);

    return true;
}

void AadlPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
    Core::DesignMode::setDesignModeIsRequired();
}

ExtensionSystem::IPlugin::ShutdownFlag AadlPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

}
