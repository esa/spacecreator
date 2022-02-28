/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 Fondazione Bruno Kessler - FBK
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

#include "compastapluginconstants.h"

#include <extensionsystem/iplugin.h>
#include <QStringList>
#include <coreplugin/actionmanager/actioncontainer.h>

namespace spctr {

class CompastaPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "CompastaPlugin.json")

public:
    CompastaPlugin();
    ~CompastaPlugin() override;

    auto initialize(const QStringList &arguments, QString *errorString) -> bool override;
    auto extensionsInitialized() -> void override;

private:
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto addCompastaMenu() -> void;
    auto verificationGUI() -> void;
    auto editFaults() -> void;
    auto editContracts() -> void;
    auto editRequirements() -> void;
    auto editProperties() -> void;
    auto openFile(const QString fname) -> void;
};

}
