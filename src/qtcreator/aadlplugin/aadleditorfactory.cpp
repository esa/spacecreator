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
#include "aadlpluginconstants.h"

#include <QGuiApplication>
#include <coreplugin/fileiconprovider.h>

namespace AadlPlugin {

AadlEditorFactory::AadlEditorFactory(QObject *parent)
    : IEditorFactory(parent)
{
    setId(AadlPlugin::Constants::K_AADL_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("AADL Editor", AadlPlugin::Constants::C_AADLEDITOR_DISPLAY_NAME));
    addMimeType(AadlPlugin::Constants::AADL_MIMETYPE);

    Core::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "xml");
}

Core::IEditor *AadlEditorFactory::createEditor()
{
    return editorData()->createEditor();
}

AadlEditorData *AadlEditorFactory::editorData() const
{
    if (!m_editorData) {
        m_editorData = new AadlEditorData(const_cast<AadlEditorFactory *>(this));
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        m_editorData->fullInit();
        QGuiApplication::restoreOverrideCursor();
    }
    return m_editorData;
}

}
