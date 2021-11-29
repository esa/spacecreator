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

#include <QStandardItemModel>
#include <QStringList>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>

namespace spctr {

class SedsPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "SedsPlugin.json")

public:
    SedsPlugin();
    ~SedsPlugin() override;

    auto initialize(const QStringList &arguments, QString *errorString) -> bool override;
    auto extensionsInitialized() -> void override;
    auto aboutToShutdown() -> ShutdownFlag override;

private:
    auto addSedsImportExport() -> void;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto importInterfaceView() -> void;
    auto importSdl() -> void;
    auto importAsn1() -> void;
    auto exportAsn1() -> void;
    auto exportInterfaceView() -> void;
    auto updateModelWithFunctionNames(QStandardItemModel &model, const QStringList &ivFunctionsNames) -> void;
};

}
