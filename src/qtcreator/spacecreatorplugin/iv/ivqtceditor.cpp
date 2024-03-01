
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

#include "dvappmodel.h"
#include "endtoend/endtoendview.h"
#include "interfacedocument.h"
#include "ivappwidget.h"
#include "iveditordocument.h"
#include "ivmainwidget.h"
#include "mainmodel.h"
#include "modelchecking/modelcheckingwindow.h"
#include "mqmchecker/mbsequalitymodelcheckerwindow.h"
#include "msceditorcore.h"
#include "mscmodel.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"
#include "dvcore/dvmodel.h"
#include "qmessagebox.h"

#include <QToolBar>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <editormanager/editormanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>


#include <QDebug>

namespace spctr {

IVQtCEditor::IVQtCEditor(SpaceCreatorProjectManager *projectManager)
    : QtCEditor()
    , m_document(new IVEditorDocument(projectManager, this))
    , m_editorWidget(new IVMainWidget)
    , m_projectManager(projectManager)
{
    setContext(Core::Context(spctr::Constants::K_IV_EDITOR_ID));
    setWidget(m_editorWidget);

    connect(m_document, &spctr::IVEditorDocument::ivDataLoaded, this, [this](const QString &, IVEditorCorePtr data) {
        m_editorWidget->init(data);
        auto command = Core::ActionManager::command(Constants::IV_MANAGE_ARCHETYPES_ID);
        IVEditorCorePtr plugin = m_editorWidget->ivPlugin();
        auto ivWidget = qobject_cast<ive::IVAppWidget *>(plugin->mainwidget());
        connect(command->action(), &QAction::triggered, ivWidget, &ive::IVAppWidget::showArchetypeManager,
                Qt::UniqueConnection);
    });
    connect(m_editorWidget, &IVMainWidget::requestE2EDataflow, this, &IVQtCEditor::showCurrentE2EDataflow);
    connect(m_editorWidget, &IVMainWidget::requestModelCheckingWindow, this,
            &IVQtCEditor::showCurrentModelCheckingWindow);

    connect(m_editorWidget, &IVMainWidget::requestMBSEQualityModelChekerWindow, this,
            &IVQtCEditor::showCurrentMBSEQualityModelCheckerWindow);


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

shared::EditorCore *IVQtCEditor::editorCore() const
{
    return m_document->ivEditorCore().get();
}

void IVQtCEditor::showCurrentModelCheckingWindow()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectManager->project(ivEditor->ivPlugin());
        if (project) {
            ivEditor->showModelCheckingWindow(
                    ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString());
        }
    }
}

void IVQtCEditor::showCurrentMBSEQualityModelCheckerWindow()
{
    if (auto ivEditor = qobject_cast<spctr::IVQtCEditor *>(Core::EditorManager::currentEditor())) {
        SpaceCreatorProjectImpl *project = m_projectManager->project(ivEditor->ivPlugin());
        if (project) {

                    ivEditor->showMBSEQualityModelCheckerWindow(
                    ProjectExplorer::ProjectTree::currentProject()->projectDirectory().toString(),project->allDVFiles(),project->allMscFiles());

        }
    }
}
void IVQtCEditor::showMBSEQualityModelCheckerWindow(const QString projectDir,const QStringList &dvFiles, const QStringList &mscFiles )
{
    if (ivPlugin().isNull()) {
        return;
    }

    IVEditorCorePtr plugin = ivPlugin();

    if (m_mbseQualityModelCheckerWindow.isNull())
    {
        m_mbseQualityModelCheckerWindow = new ive::MBSEQualityModelCheckerWindow(plugin->document(), projectDir, nullptr);
        m_mbseQualityModelCheckerWindow->setAttribute(Qt::WA_DeleteOnClose);

        if(!dvFiles.isEmpty()){m_mbseQualityModelCheckerWindow->DvModelsPaths(dvFiles);} // register all posible dv diagrams paths/ names
        // register DV model elements
        for (QString fileName: dvFiles){
            if (m_projectManager)
            {
                DVEditorCorePtr core =m_projectManager->dvData(fileName);

                if (core)
                {
                    m_mbseQualityModelCheckerWindow->DvModelStore( core->appModel()->objectsModel()->allObjectsByType<dvm::DVPartition>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVFunction>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVNode>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVMessage>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVConnection>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVDevice>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVPort>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVSystemFunction>(),
                                                                    core->appModel()->objectsModel()->allObjectsByType<dvm::DVSystemInterface>());

                }
            }
        }

        // store the number of msc
        for (QString fileName: mscFiles)
        {
            if (m_projectManager)
            {
                QFileInfo info (fileName );
                QDir dir =info.dir();// if the .msc is stored in the root of the project then it is used to check metrics, otherwise diagram is not taken into acount in metrics computation
                if (dir==projectDir)
                {
                    MSCEditorCorePtr core = m_projectManager->mscData(fileName);
                    if (core) {
                        m_mbseQualityModelCheckerWindow->setMscDocuments(core->mainModel()->mscModel()->allDocuments());

                    }
                }
            }
        }

        m_mbseQualityModelCheckerWindow->callCheckMetrics(true);

            connect(plugin->document(), &QObject::destroyed, m_mbseQualityModelCheckerWindow.data(), &QObject::deleteLater);
            connect(m_mbseQualityModelCheckerWindow, &ive::MBSEQualityModelCheckerWindow::visibleChanged,
                    m_editorWidget->ivPlugin()->actionLaunchMBSEQualityModelCheckerWindow(), &QAction::setChecked);
            connect(m_editorWidget->ivPlugin()->actionLaunchMBSEQualityModelCheckerWindow(), &QAction::toggled, m_mbseQualityModelCheckerWindow,
                    &ive::MBSEQualityModelCheckerWindow::setVisible);



    }
int msgBoxAnw=  m_mbseQualityModelCheckerWindow->showCheckingMessage();
switch (msgBoxAnw) {
  case QMessageBox::Yes:

    m_mbseQualityModelCheckerWindow->show();
    m_mbseQualityModelCheckerWindow->raise();

    break;


case QMessageBox::Cancel:
    m_mbseQualityModelCheckerWindow->show();
    m_mbseQualityModelCheckerWindow->hide();
    m_mbseQualityModelCheckerWindow->close();

    break;

default:
    // should never be reached
    break;
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
        m_modelCheckingWindow->callTasteGens(true);
        m_modelCheckingWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(plugin->document(), &QObject::destroyed, m_modelCheckingWindow.data(), &QObject::deleteLater);
        connect(m_modelCheckingWindow, &ive::ModelCheckingWindow::visibleChanged,
                m_editorWidget->ivPlugin()->actionLaunchModelCheckingWindow(), &QAction::setChecked);
        connect(m_editorWidget->ivPlugin()->actionLaunchModelCheckingWindow(), &QAction::toggled, m_modelCheckingWindow,
                &ive::ModelCheckingWindow::setVisible);
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
