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

#include <qboxlayout.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qtoolbar.h>

namespace sedsplugin {

SedsPluginWindow::SedsPluginWindow()
{
    setWindowTitle("SEDS plugin test window");

    if (!loadPlugin()) {
        QMessageBox::information(this, "ERROR", "plugin could not be loaded");
    }
}

bool SedsPluginWindow::loadPlugin()
{
    qDebug() << QCoreApplication::applicationDirPath();
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    pluginsDir.cd("plugins");

    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        qDebug() << fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            m_sedsPlugin = qobject_cast<spctr::SedsPlugin *>(plugin);
            if (m_sedsPlugin != nullptr) {
                // m_sedsPlugin->initialize({ "" }, nullptr);
                return true;
            }
            pluginLoader.unload();
        }
    }

    return false;
}

} // namespace sedsplugin
