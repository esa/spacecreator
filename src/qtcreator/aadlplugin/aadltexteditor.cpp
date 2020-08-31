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

#include "aadltexteditor.h"

#include "aadleditordocument.h"
#include "aadlmainwidget.h"
#include "aadlpluginconstants.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"

#include <QFileInfo>
#include <utils/qtcassert.h>

namespace AadlPlugin {

AadlTextEditor::AadlTextEditor()
{
    addContext(AadlPlugin::Constants::K_AADL_EDITOR_ID);
    addContext(AadlPlugin::Constants::C_AADL_EDITOR);
}

void AadlTextEditor::finalizeInitialization()
{
    // Revert to saved/load externally modified files.
    auto document = qobject_cast<const AadlEditorDocument *>(textDocument());
    connect(document, &AadlEditorDocument::reloadRequested,
            [this](QString *errorString, const QString &fileName) { open(errorString, fileName, fileName); });
}

bool AadlTextEditor::open(QString *errorString, const QString &fileName, const QString & /*realFileName*/)
{
    Q_UNUSED(errorString)
    auto document = qobject_cast<AadlEditorDocument *>(textDocument());
    AadlMainWidget *designWidget = document->designWidget();
    QTC_ASSERT(designWidget, return false);

    if (fileName.isEmpty())
        return true;

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();

    if (!designWidget->load(absfileName)) {
        return false;
    }

    document->syncXmlFromDesignWidget();
    document->setFilePath(Utils::FileName::fromString(absfileName));

    return true;
}

QString AadlTextEditor::fileName() const
{
    aadlinterface::IVEditorCore *plugin = ivPlugin();
    if (plugin && plugin->document()) {
        return plugin->document()->path();
    }

    return {};
}

aadlinterface::IVEditorCore *AadlTextEditor::ivPlugin() const
{
    auto document = qobject_cast<AadlEditorDocument *>(textDocument());
    AadlMainWidget *designWidget = document->designWidget();
    QTC_ASSERT(designWidget, return {});
    QTC_ASSERT(designWidget->ivPlugin(), return {});
    return designWidget->ivPlugin();
}

}
