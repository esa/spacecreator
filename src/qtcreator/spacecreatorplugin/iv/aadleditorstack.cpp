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

#include "iveditorstack.h"

#include "iveditordocument.h"
#include "qtciveditor.h"
#include "iveditorcore.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/modemanager.h>
#include <utils/qtcassert.h>

namespace spctr {

IVEditorStack::IVEditorStack(QWidget *parent)
    : QStackedWidget(parent)
{
    setObjectName("IVEditorStack");
}

void IVEditorStack::add(QtCIVEditor *editor, QWidget *w)
{
    connect(Core::ModeManager::instance(), &Core::ModeManager::currentModeAboutToChange, this,
            &IVEditorStack::modeAboutToChange);

    m_editors.append(editor);
    addWidget(w);
    connect(editor, &QtCIVEditor::destroyed, this, &IVEditorStack::removeIVTextEditor);
}

void IVEditorStack::removeIVTextEditor(QObject *xmlEditor)
{
    const int i = m_editors.indexOf(static_cast<QtCIVEditor *>(xmlEditor));
    QTC_ASSERT(i >= 0, return );

    QWidget *widget = this->widget(i);
    if (widget) {
        removeWidget(widget);
        widget->deleteLater();
    }
    m_editors.removeAt(i);
}

bool IVEditorStack::setVisibleEditor(Core::IEditor *xmlEditor)
{
    const int i = m_editors.indexOf(static_cast<QtCIVEditor *>(xmlEditor));
    QTC_ASSERT(i >= 0, return false);

    if (i != currentIndex()) {
        setCurrentIndex(i);
    }

    return true;
}

QSharedPointer<ive::IVEditorCore> IVEditorStack::ivPlugin(const QString &fileName) const
{
    for (QtCIVEditor *editor : m_editors) {
        if (editor->fileName() == fileName) {
            return editor->ivPlugin();
        }
    }
    return nullptr;
}

QWidget *IVEditorStack::widgetForEditor(QtCIVEditor *xmlEditor)
{
    const int i = m_editors.indexOf(xmlEditor);
    QTC_ASSERT(i >= 0, return nullptr);

    return widget(i);
}

void IVEditorStack::modeAboutToChange(Core::Id m)
{
    // Sync the editor when entering edit mode
    if (m == Core::Constants::MODE_EDIT) {
        for (auto editor : qAsConst(m_editors)) {
            if (auto document = qobject_cast<IVEditorDocument *>(editor->textDocument())) {
                document->syncXmlFromDesignWidget();
            }
        }
    }
}

}
