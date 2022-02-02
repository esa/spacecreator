/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "sedspluginwindow.h"

#include "sedsplugin.h"

#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <qboxlayout.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qtoolbar.h>

namespace sedsplugin {

SedsPluginWindow::SedsPluginWindow()
{
    setWindowTitle("SEDS plugin test window");

    // const char M_TOOLS_SEDS[] = "QtCreator.Menu.Tools.SEDS";
    // Core::ActionManager::createMenu("QtCreator.Menu");
    // Core::ActionManager::createMenu("QtCreator.Menu.Tools");
    qDebug() << "title: " << this->windowTitle();
    qDebug() << "win id: " << this->winId();

    // Initialize the file menu
    auto menu = menuBar()->addMenu(tr("File"));
    menu->addSeparator();
    menu = menuBar()->addMenu(tr("Edit"));
    menu->addSeparator();
    menu = menuBar()->addMenu(tr("&Help"));

    menu = menuBar()->addMenu(tr("Tools"));

    // Core::ActionManager::instance();

    // for (const auto &command : Core::ActionManager::commands()) {
    //     qDebug() << "command id" << command->id();
    //     qDebug() << command->objectName();
    //     for (const auto &c : command->context()) {
    //         qDebug() << c.name();
    //         qDebug() << c.toString();
    //     }
    // }

    // for (const auto &child : Core::ActionManager::instance()->children()) {
    //     qDebug() << child->objectName();
    // }

    // Core::ActionManager::createMenu("Menu.Tools");

    if (!loadPlugin()) {
        QMessageBox::information(this, "ERROR", "plugin could not be loaded");
    }

    // auto *const currentDocument = Core::EditorManager::currentDocument();
    // ive::ActionsManager::populateMenu(menu, nullptr, nullptr);
}

bool SedsPluginWindow::loadPlugin()
{
    for (const auto &path : QCoreApplication::libraryPaths()) {
        if (path.contains("plugin")) {
            QDir pluginsDir(path);

            const QStringList entries = pluginsDir.entryList(QDir::Files);
            for (const QString &fileName : entries) {
                QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
                QObject *plugin = pluginLoader.instance();
                if (plugin) {
                    m_sedsPlugin = qobject_cast<spctr::SedsPlugin *>(plugin);
                    if (m_sedsPlugin != nullptr) {
                        m_sedsPlugin->dumpObjectInfo();
                        m_sedsPlugin->dumpObjectTree();
                        QString errorStr = "x";
                        m_sedsPlugin->initialize({ "tttt" }, &errorStr);
                        return true;
                    }
                    pluginLoader.unload();
                }
            }
        }
    }

    return false;
}

} // namespace sedsplugin
