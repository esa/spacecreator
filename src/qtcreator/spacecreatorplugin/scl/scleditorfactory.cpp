/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include "scleditorfactory.h"

#include <texteditor/basehoverhandler.h>
#include <texteditor/texteditoractionhandler.h>

#include "spacecreatorpluginconstants.h"

namespace spctr {

using namespace TextEditor;

SclDocument::SclDocument()
{
    setId(Constants::K_SCL_EDITOR_ID);
}

SclEditorWidget::SclEditorWidget(QWidget *parent ) :
    TextEditor::TextEditorWidget(parent)
{

}

SclEditor::SclEditor()
{
    addContext(Constants::SCL_BASE_CONTEXT);
}

SclEditorFactory::SclEditorFactory()
{
    setId(Constants::K_SCL_EDITOR_ID);
    setDisplayName(Constants::C_SCLEDITOR_DISPLAY_NAME);
    addMimeType(Constants::SCL_MIMETYPE);

    setDocumentCreator([]() { return new SclDocument; });
    setEditorWidgetCreator([]() { return new SclEditorWidget; });
    setEditorCreator([]() { return new SclEditor; });

    addHoverHandler(new TextEditor::BaseHoverHandler);

    setCodeFoldingSupported(true);
    setMarksVisible(true);
    setParenthesesMatchingEnabled(true);
    setUseGenericHighlighter(true);

    setEditorActionHandlers(TextEditorActionHandler::Format
                            | TextEditorActionHandler::UnCommentSelection
                            | TextEditorActionHandler::UnCollapseAll);
}

} // namespace spctr
