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

#pragma once

#include "spacecreatorplugin_global.h"

#include <QSharedPointer>
#include <QVector>
#include <extensionsystem/iplugin.h>

class QAction;

namespace aadlinterface {
class IVEditorCore;
}

namespace msc {
class MSCEditorCore;
}

namespace shared {
class EditorCore;
}

namespace spctr {

class AadlEditorFactory;
class AadlModelStorage;
class MscSystemChecks;
class MscEditorFactory;
class MscModelStorage;

class SpaceCreatorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "SpaceCreatorPlugin.json")

public:
    SpaceCreatorPlugin();
    ~SpaceCreatorPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private Q_SLOTS:
    void showMessageDeclarations();
    void setMinimapVisible(bool visible);
    void showAsn1Dialog();
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();
    void checkAsnFileRename();
    void saveIfNotOpen(shared::EditorCore *core);

private:
    bool isOpenInEditor(shared::EditorCore *core) const;

    MscEditorFactory *m_mscFactory = nullptr;
    AadlEditorFactory *m_aadlFactory = nullptr;
    QAction *m_messageDeclarationAction = nullptr;
    QAction *m_asn1DialogAction = nullptr;
    AadlModelStorage *m_aadlStorage = nullptr;
    MscModelStorage *m_mscStorage = nullptr;
    QStringList m_asnFiles;
    MscSystemChecks *m_checks = nullptr;
};

}
