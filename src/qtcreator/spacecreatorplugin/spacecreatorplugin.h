/*
   Copyright (C) 2018 - 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "options/asn1sccoption.h"
#include "options/coloroption.h"
#include "options/dynamicactionoption.h"
#include "options/hwlibraryoption.h"
#include "options/ivlibraryoption.h"
#include "options/propertyoptions.h"
#include "spacecreatorplugin_global.h"

#include <QSharedPointer>
#include <QVector>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>

class QAction;

namespace ProjectExplorer {
class Project;
}

namespace ive {
class IVEditorCore;
}

namespace msc {
class MSCEditorCore;
}

namespace shared {
class EditorCore;
}

namespace spctr {

class IVEditorFactory;
class DVEditorFactory;
class MscEditorFactory;
class SpaceCreatorProjectManager;

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

private:
    void addHelp();

    MscEditorFactory *m_mscFactory = nullptr;
    IVEditorFactory *m_ivFactory = nullptr;
    DVEditorFactory *m_dvFactory = nullptr;
    SpaceCreatorProjectManager *m_projectsManager = nullptr;

    Asn1SccOption m_asnCommandOptions;
    ColorOption m_ivColorOptions;
    DynamicActionOption m_ivDynActionOptions;
    PropertyOptions m_propertyOptions;
    HwLibraryOption m_hwLibraryOptions;
    IVLibraryOption m_ivLibraryOptions;
};

}
