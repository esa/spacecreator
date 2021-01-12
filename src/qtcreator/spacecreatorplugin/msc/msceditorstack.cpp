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

#include "msceditorstack.h"

#include "msceditordocument.h"
#include "mscqtceditor.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/modemanager.h>
#include <utils/qtcassert.h>

namespace spctr {

MscEditorStack::MscEditorStack(QWidget *parent)
    : QStackedWidget(parent)
{
    setObjectName("MscEditorStack");
}

void MscEditorStack::add(Core::IEditor *editor, QWidget *w)
{
    m_editors.append(editor);
    addWidget(w);
    connect(editor, &Core::IEditor::destroyed, this, &MscEditorStack::removeMscTextEditor);
}

void MscEditorStack::removeMscTextEditor(QObject *editor)
{
    const int i = m_editors.indexOf(static_cast<Core::IEditor *>(editor));
    QTC_ASSERT(i >= 0, return );

    QWidget *widget = this->widget(i);
    if (widget) {
        removeWidget(widget);
        widget->deleteLater();
    }
    m_editors.removeAt(i);
}

bool MscEditorStack::setVisibleEditor(Core::IEditor *editor)
{
    const int i = m_editors.indexOf(static_cast<Core::IEditor *>(editor));
    QTC_ASSERT(i >= 0, return false);

    if (i != currentIndex())
        setCurrentIndex(i);

    return true;
}

}
