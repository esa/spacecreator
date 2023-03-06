/****************************************************************************
**
** Copyright (C) 2022 Fondazione Bruno Kessler - FBK
**
** This file is part of COMPASTA Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "slimsnippets.h"
#include "slimconstants.h"

#include <QCoreApplication>
#include <QLatin1String>

#include <texteditor/snippets/snippetprovider.h>
#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>

#include "tr.h"

using namespace Slim::Internal::Completion;

static void decorateEditor(TextEditor::TextEditorWidget *editor)
{
    editor->textDocument()->setMimeType(Slim::Constants::SLIM_MIMETYPE);
}

void SlimSnippets::registerGroup()
{
    TextEditor::SnippetProvider::registerGroup(QLatin1String(Constants::SLIM_SNIPPETS_GROUP_ID),
                                               Tr::tr("Slim"),
                                               &decorateEditor);
}
