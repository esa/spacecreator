/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "editorcore.h"

class QUndoStack;
class QToolBar;

namespace shared {
class CommandLineParser;
namespace ui {
class GraphicsViewBase;
}
}

namespace deploymentinterface {
class DeploymentModel;
namespace cmd {
class CommandsStack;
}

class DVEditorCore : public shared::EditorCore
{
    Q_OBJECT
public:
    explicit DVEditorCore(QObject *parent = nullptr);
    ~DVEditorCore() override;

    void setPluginActive(bool active) override;
    void addMenuFileActions(QMenu *menu, QMainWindow *window) override;
    void addMenuEditActions(QMenu *menu, QMainWindow *window) override;
    void addMenuHelpActions(QMenu *menu, QMainWindow *window) override;
    void addToolBars(QMainWindow *window) override;

    shared::ui::GraphicsViewBase *chartView() override;
    QToolBar *toolBar();

    QUndoStack *undoStack() const override;

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;
    bool renameAsnFile(const QString &oldName, const QString &newName) override;

    QString filePath() const override;
    bool save() override;

private:
    struct DeploymentInterfacePrivate;
    std::unique_ptr<DeploymentInterfacePrivate> d;
};

} // namespace deploymentinterface
