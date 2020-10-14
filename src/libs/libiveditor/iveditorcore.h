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

#include "aadlobjectiface.h"
#include "editorcore.h"
#include "ui/graphicsviewbase.h"

namespace aadlinterface {

class InterfaceDocument;

class IVEditorCore : public shared::EditorCore
{
public:
    explicit IVEditorCore(QObject *parent = 0);
    ~IVEditorCore();

    aadlinterface::InterfaceDocument *document() const;

    void setPluginActive(bool active) override;

    shared::ui::GraphicsViewBase *chartView() override;
    void addToolBars(QMainWindow *window) override;
    void addMenuFileActions(QMenu *menu, QMainWindow *window) override;
    void addMenuEditActions(QMenu *menu, QMainWindow *window) override;
    void addMenuViewActions(QMenu *menu, QMainWindow *window) override;
    void addMenuHelpActions(QMenu *menu, QMainWindow *window) override;

    QToolBar *docToolBar() { return m_docToolBar; }

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionSaveSceneRender() { return m_actionSaveSceneRender; }
    QAction *actionShowAsnDialog() { return m_actionShowAsnDialog; }

    bool addFunction(const QString &name);
    bool addConnection(QString name, const QString &fromInstanceName, const QString &toInstanceName);

    bool renameAsnFile(const QString &oldName, const QString &newName) override;

    QString filePath() const override;
    bool save() override;

private:
    aadl::AADLObjectIface *getInterface(
            const QString &ifName, aadl::AADLObjectIface::IfaceType ifType, aadl::AADLObjectFunction *parentFunction);

    aadlinterface::InterfaceDocument *m_document { nullptr };

    QToolBar *m_docToolBar;

    QAction *m_actionSaveSceneRender { nullptr };
    QAction *m_actionShowAsnDialog { nullptr };
};

}
