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
#include "aadlmodelstorage.h"
#include "mscmodelstorage.h"
#include "spacecreatorpluginconstants.h"

#include <QGuiApplication>
#include <coreplugin/fileiconprovider.h>

namespace spctr {

AadlEditorFactory::AadlEditorFactory(AadlModelStorage *aadlStorage, MscModelStorage *mscStorage, QObject *parent)
    : IEditorFactory(parent)
    , m_aadlStorage(aadlStorage)
    , m_mscStorage(mscStorage)
{
    setId(spctr::Constants::K_AADL_EDITOR_ID);
    setDisplayName(QCoreApplication::translate("AADL Editor", spctr::Constants::C_AADLEDITOR_DISPLAY_NAME));
    addMimeType(spctr::Constants::AADL_MIMETYPE);

    Core::FileIconProvider::registerIconOverlayForSuffix(":/projectexplorer/images/fileoverlay_scxml.png", "xml");
}

Core::IEditor *AadlEditorFactory::createEditor()
{
    return editorData()->createEditor();
}

AadlEditorData *AadlEditorFactory::editorData() const
{
    if (!m_editorData) {
        m_editorData = new AadlEditorData(m_aadlStorage, m_mscStorage, const_cast<AadlEditorFactory *>(this));
        connect(m_editorData, &AadlEditorData::aadlDataLoaded, this, &AadlEditorFactory::aadlDataLoaded);
    }
    return m_editorData;
}

}
