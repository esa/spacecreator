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

#include "msctexteditor.h"

#include "mainwidget.h"
#include "msceditordocument.h"
#include "mscpluginconstants.h"

#include <QFileInfo>
#include <utils/qtcassert.h>

namespace MscPlugin {

MscTextEditor::MscTextEditor()
{
    addContext(MscPlugin::Constants::K_MSC_EDITOR_ID);
    addContext(MscPlugin::Constants::C_MSC_EDITOR);
}

void MscTextEditor::finalizeInitialization()
{
    // Revert to saved/load externally modified files.
    auto document = qobject_cast<const MscEditorDocument *>(textDocument());
    connect(document, &MscEditorDocument::reloadRequested,
            [this](QString *errorString, const QString &fileName) { open(errorString, fileName, fileName); });
}

bool MscTextEditor::open(QString *errorString, const QString &fileName, const QString & /*realFileName*/)
{
    auto document = qobject_cast<MscEditorDocument *>(textDocument());
    MainWidget *designWidget = document->designWidget();
    QTC_ASSERT(designWidget, return false);

    if (fileName.isEmpty())
        return true;

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();

    if (!designWidget->load(absfileName)) {
        *errorString = designWidget->errorMessage();
        return false;
    }

    document->syncXmlFromDesignWidget();
    document->setFilePath(Utils::FileName::fromString(absfileName));

    return true;
}

}
