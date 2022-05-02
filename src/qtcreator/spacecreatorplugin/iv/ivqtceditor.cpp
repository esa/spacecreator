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

#include "ivqtceditor.h"

#include "commands/cmdchangeasn1file.h"
#include "commandsstack.h"
#include "endtoend/endtoendview.h"
#include "modelchecking/modelcheckingwindow.h"
#include "interfacedocument.h"
#include "iveditordocument.h"
#include "ivmainwidget.h"
#include "mainmodel.h"
#include "msceditorcore.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QToolBar>
#include <QUndoCommand>
#include <editormanager/editormanager.h>
#include <utils/qtcassert.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

namespace spctr {

IVQtCEditor::IVQtCEditor(SpaceCreatorProjectManager *projectManager)
    : Core::IEditor()
    , m_document(new IVEditorDocument(projectManager, this))
    , m_editorWidget(new IVMainWidget)
    , m_projectManager(projectManager)
{
    setContext(Core::Context(spctr::Constants::K_IV_EDITOR_ID));
    setWidget(m_editorWidget);

    connect(m_document, &spctr::IVEditorDocument::ivDataLoaded, this,
            [this](const QString &, IVEditorCorePtr data) { m_editorWidget->init(data); });
    connect(m_editorWidget, &IVMainWidget::requestE2EDataflow, this, &IVQtCEditor::showCurrentE2EDataflow);
    connect(m_editorWidget, &IVMainWidget::requestModelCheckingWindow, this, &IVQtCEditor::showCurrentModelCheckingWindow);
}

IVQtCEditor::~IVQtCEditor()
{
    m_editorWidget->deleteLater();
}

Core::IDocument *IVQtCEditor::document() const
{
    return m_document;
}

IVEditorDocument *IVQtCEditor::ivDocument() const
{
    return m_document;
}

IVEditorCorePtr IVQtCEditor::ivPlugin() const
{
    return m_document->ivEditorCore();
}

QWidget *IVQtCEditor::toolBar()
{
    IVEditorCorePtr ivCore = m_document->ivEditorCore();
    if (m_toolbar == nullptr && !ivCore.isNull()) {
        m_toolbar = new QToolBar;
        m_toolbar->addAction(ivCore->actionUndo());
        m_toolbar->addAction(ivCore->actionRedo());
    }

    return m_toolbar;
}

void IVQtCEditor::showCurrentModelCheckingWindow()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectManager->project(ivEditor->ivPlugin());
        if (project) {
            ivEditor->showModelCheckingWindow(ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString());
        }
    }
}

void IVQtCEditor::showModelCheckingWindow(const QString projectDir)
{
    if (ivPlugin().isNull()) {
        return;
    }

    IVEditorCorePtr plugin = ivPlugin();
    if (m_modelCheckingWindow.isNull()) {
        m_modelCheckingWindow = new ive::ModelCheckingWindow(plugin->document(), projectDir, nullptr);
        m_modelCheckingWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(plugin->document(), &QObject::destroyed, m_modelCheckingWindow.data(), &QObject::deleteLater);
    }
    m_modelCheckingWindow->show();
    m_modelCheckingWindow->raise();
}

void IVQtCEditor::showCurrentE2EDataflow()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectManager->project(ivEditor->ivPlugin());
        if (project) {
            ivEditor->showE2EDataflow(project->allMscFiles());
        }
    }
}

void IVQtCEditor::showE2EDataflow(const QStringList &mscFiles)
{
    if (ivPlugin().isNull()) {
        return;
    }

    IVEditorCorePtr plugin = ivPlugin();
    if (m_endToEndView.isNull()) {
        m_endToEndView = new ive::EndToEndView(plugin->document(), nullptr);
        m_endToEndView->setAttribute(Qt::WA_DeleteOnClose);
        std::function<msc::MscModel *(QString fileName)> fetcher = [this](QString fileName) -> msc::MscModel * {
            if (m_projectManager) {
                MSCEditorCorePtr core = m_projectManager->mscData(fileName);
                if (core) {
                    return core->mainModel()->mscModel();
                }
            }
            return {};
        };
        m_endToEndView->setMscDataFetcher(fetcher);
        m_endToEndView->setMscFiles(mscFiles);
        connect(plugin->document(), &QObject::destroyed, m_endToEndView.data(), &QObject::deleteLater);
    }
    m_endToEndView->show();
    m_endToEndView->raise();
}

}
