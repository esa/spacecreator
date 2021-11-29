/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#pragma once

#include "sedspluginconstants.h"

#include <QSharedPointer>
#include <QStandardItemModel>
#include <QStringList>
#include <QVector>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>

void updateModelWithFunctionNames(QStandardItemModel &model, const QStringList &ivFunctionsNames);

namespace spctr {

class IVEditorFactory;
class DVEditorFactory;
class MscEditorFactory;
class SpaceCreatorProjectManager;

class SedsPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "SedsPlugin.json")

public:
    SedsPlugin();
    ~SedsPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:    
    void addSedsImportExport();
    Core::ActionContainer *createActionContainerInTools(const QString &title);
    static void importInterfaceView();
    static void importSdl();
    static void importAsn1();
    static void exportAsn1();
    static void exportInterfaceView();
};

}
