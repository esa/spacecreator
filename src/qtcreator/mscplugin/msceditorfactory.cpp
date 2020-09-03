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

#include "msceditorcore.h"
#include "msceditordata.h"
#include "mscpluginconstants.h"

#include <QGuiApplication>
#include <coreplugin/fileiconprovider.h>

namespace MscPlugin {

MscEditorFactory::MscEditorFactory(QObject *parent)
    : IEditorFactory(parent)
{
    setId(Constants::K_MSC_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("MscEditor", Constants::C_MSCEDITOR_DISPLAY_NAME));
    addMimeType(MscPlugin::Constants::MSC_MIMETYPE);

    Core::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "msc");
}

Core::IEditor *MscEditorFactory::createEditor()
{
    return editorData()->createEditor();
}

MscEditorData *MscEditorFactory::editorData() const
{
    if (!m_editorData) {
        m_editorData = new MscEditorData(const_cast<MscEditorFactory *>(this));
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        m_editorData->fullInit();
        connect(m_editorData, &MscEditorData::mscDataLoaded, this, &MscEditorFactory::mscDataLoaded);
        QGuiApplication::restoreOverrideCursor();
    }

    return m_editorData;
}

}
