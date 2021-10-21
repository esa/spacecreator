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

#include "mscmainwidget.h"

#include "chartlayoutmanager.h"
#include "commands/cmdsetasn1file.h"
#include "mainmodel.h"
#include "mscappwidget.h"
#include "mscmodel.h"
#include "spacecreatorprojectimpl.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <coreplugin/editormanager/editormanager.h>

namespace spctr {

/*!
   \brief MscEditorCore::MainWidget::MainWidget Is the main widget for the whole MSC plugin in QtCreator
 */

MscMainWidget::MscMainWidget(QWidget *parent)
    : QWidget(parent)
{
}

MscMainWidget::~MscMainWidget() { }

bool MscMainWidget::init(MSCEditorCorePtr plugin, SpaceCreatorProjectImpl *project)
{
    m_project = project;
    m_plugin = plugin;
    init();
    return true;
}

MSCEditorCorePtr MscMainWidget::mscCore() const
{
    Q_ASSERT(!m_plugin.isNull());
    return m_plugin;
}

void MscMainWidget::openAsn1Dialog()
{
    QString dir = QFileInfo(m_plugin->mainModel()->currentFilePath()).absolutePath();
    QString asn1File = QFileDialog::getOpenFileName(this, tr("Select a asn.1 file"), dir, "ASN.1 files (*.asn *.asn1)");
    if (!asn1File.isEmpty()) {
        QFileInfo fi(asn1File);
        msc::MscModel *model = m_plugin->mainModel()->mscModel();
        m_plugin->mainModel()->undoStack()->push(new msc::cmd::CmdSetAsn1File(model, fi.fileName(), "ASN.1"));
    }
}

void MscMainWidget::init()
{
    if (m_plugin.isNull()) {
        return;
    }

    auto layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    msc::MscAppWidget *mainWidget = m_plugin->mainwidget();
    mainWidget->showAsn1View(false);
    mainWidget->setParent(this);
    layout->addWidget(mainWidget);

    if (!mainWidget->chartView()) {
        m_plugin->mainModel()->chartViewModel().clearScene();
        m_plugin->mainModel()->chartViewModel().updateLayout();
        m_plugin->chartView()->setZoom(100);
    }
    connect(mainWidget, &msc::MscAppWidget::showAsn1File, this,
            [&](const QString &asnFilename) { Core::EditorManager::instance()->openEditor(asnFilename); });
    connect(mainWidget, &msc::MscAppWidget::selectAsn1, this, &spctr::MscMainWidget::openAsn1Dialog);
    connect(mainWidget, &msc::MscAppWidget::showInterfaceView, this, [&]() {
        if (!m_project) {
            return;
        }
        const QStringList ivFiles = m_project->allIVFiles();
        if (!ivFiles.isEmpty()) {
            Core::EditorManager::instance()->openEditor(ivFiles.first());
        }
    });
}

}
