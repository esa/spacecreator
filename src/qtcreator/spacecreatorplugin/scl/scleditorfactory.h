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

#pragma once

#include <texteditor/texteditor.h>
#include <texteditor/textdocument.h>

namespace spctr {

class SclDocument : public TextEditor::TextDocument
{
    Q_OBJECT

public:
    explicit SclDocument();
};

class SclEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    explicit SclEditorWidget(QWidget *parent = nullptr);
};

class SclEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit SclEditor();
};

class SclEditorFactory : public TextEditor::TextEditorFactory
{
public:
    explicit SclEditorFactory();
};

} // namespace spctr
