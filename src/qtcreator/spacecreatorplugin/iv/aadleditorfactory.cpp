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

#include "aadleditorfactory.h"

#include "aadleditordata.h"
#include "modelstorage.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QGuiApplication>
#include <coreplugin/fileiconprovider.h>

namespace spctr {

AadlEditorFactory::AadlEditorFactory(
        SpaceCreatorProjectManager *projectManager, const QList<QAction *> &ivActions, QObject *parent)
    : IEditorFactory(parent)
    , m_editorData(new AadlEditorData(projectManager, ivActions))
{
    setId(spctr::Constants::K_AADL_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("AADL Editor", spctr::Constants::C_AADLEDITOR_DISPLAY_NAME));
    addMimeType(spctr::Constants::AADL_MIMETYPE);

    Core::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "xml");
}

Core::IEditor *AadlEditorFactory::createEditor()
{
    return m_editorData->createEditor();
}

AadlEditorData *AadlEditorFactory::editorData() const
{
    return m_editorData.get();
}

}
