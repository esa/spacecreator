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

#pragma once

#include "aadlplugin_global.h"

#include <extensionsystem/iplugin.h>

class QAction;

namespace aadlinterface {
class IVEditorCore;
}

namespace AadlPlugin {

class AadlEditorFactory;

class AadlPluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "AadlPlugin.json")

public:
    AadlPluginPlugin();
    ~AadlPluginPlugin();

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

public Q_SLOTS:
    void showAsn1Dialog();
    void showMinimap(bool visible);
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

    aadlinterface::IVEditorCore *ivPlugin(const QString &fileName) const;

private:
    AadlEditorFactory *m_factory = nullptr;
    QAction *m_asn1DialogAction = nullptr;
    QAction *m_minimapWidgetAction = nullptr;
};

}
