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

#include "msceditorfactory.h"

#include "common/actionhandler.h"
#include "mscqtceditor.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QGuiApplication>
#include <utils/fsengine/fileiconprovider.h>

namespace spctr {

MscEditorFactory::MscEditorFactory(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : IEditorFactory()
    , m_actionHandler(new ActionHandler(spctr::Constants::K_MSC_EDITOR_ID, projectManager, this))
    , m_projectManager(projectManager)
{
    setId(Constants::K_MSC_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("MscEditor", Constants::C_MSCEDITOR_DISPLAY_NAME));
    addMimeType(spctr::Constants::MSC_MIMETYPE);

    setEditorCreator(std::bind(&MscEditorFactory::createMSCEditor, this));
    Utils::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "msc");
}

Core::IEditor *MscEditorFactory::createMSCEditor()
{
    return new MscQtCEditor(m_projectManager);
}

}
