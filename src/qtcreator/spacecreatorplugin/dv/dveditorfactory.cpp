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

#include "dveditordata.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QGuiApplication>
#include <coreplugin/fileiconprovider.h>

namespace spctr {

DVEditorFactory::DVEditorFactory(
        SpaceCreatorProjectManager *projectManager, const QList<QAction *> &dvActions, QObject *parent)
    : IEditorFactory()
    , m_editorData(new DVEditorData(projectManager, dvActions))
{
    setId(spctr::Constants::K_DV_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("DV Editor", spctr::Constants::C_DVEDITOR_DISPLAY_NAME));
    addMimeType(spctr::Constants::DV_MIMETYPE);
#if QTC_VERSION == 415
    setEditorCreator(std::bind(&DVEditorFactory::createEditor, this));
#endif
    Core::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "xml");
}

#if QTC_VERSION == 48
Core::IEditor *spctr::DVEditorFactory::createEditor()
{
    return m_editorData->createEditor();
}
#endif

} // namespace spctr
