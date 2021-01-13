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

#pragma once

#include "spacecreatorplugin_global.h"

#include <QPointer>
#include <QSharedPointer>
#include <editormanager/ieditor.h>

class QToolBar;

namespace msc {
class MSCEditorCore;
}

namespace spctr {

class MscEditorDocument;
class MscMainWidget;
class MscModelStorage;

class MscQtCEditor : public Core::IEditor
{
    Q_OBJECT

public:
    MscQtCEditor(MscModelStorage *mscStorage);
    ~MscQtCEditor();

    Core::IDocument *document() const override;
    MscEditorDocument *mscDocument() const;
    QSharedPointer<msc::MSCEditorCore> mscEditorCore() const;

    QWidget *toolBar() override;

private:
    MscEditorDocument *m_document = nullptr;
    QPointer<QToolBar> m_toolbar = nullptr;
    MscMainWidget *m_editorWidget = nullptr;
};

}
