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

#include "dveditorfactory.h"

#include "common/actionhandler.h"
#include "dvqtceditor.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QGuiApplication>
#include <utils/fsengine/fileiconprovider.h>

namespace spctr {

DVEditorFactory::DVEditorFactory(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : IEditorFactory()
    , m_actionHandler(new ActionHandler(spctr::Constants::K_DV_EDITOR_ID, this))
    , m_projectManager(projectManager)
{
    setId(spctr::Constants::K_DV_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("DV Editor", spctr::Constants::C_DVEDITOR_DISPLAY_NAME));
    addMimeType(spctr::Constants::DV_MIMETYPE);
    setEditorCreator(std::bind(&DVEditorFactory::createDVEditor, this));
    Utils::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "xml");
}

Core::IEditor *DVEditorFactory::createDVEditor()
{
    return new DVQtCEditor(m_projectManager);
}

} // namespace spctr
