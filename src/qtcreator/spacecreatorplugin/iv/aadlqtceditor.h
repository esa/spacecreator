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

#pragma once

#include <QPointer>
#include <QSharedPointer>
#include <editormanager/ieditor.h>

class QToolBar;

namespace aadlinterface {
class EndToEndView;
class IVEditorCore;
}

namespace spctr {

class AadlEditorDocument;
class AadlMainWidget;
class AadlModelStorage;
class MscModelStorage;

class AadlQtCEditor : public Core::IEditor
{
    Q_OBJECT

public:
    AadlQtCEditor(AadlModelStorage *aadlStorage, MscModelStorage *mscStorage);
    ~AadlQtCEditor();

    Core::IDocument *document() const override;
    AadlEditorDocument *ivDocument() const;
    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin() const;

    QWidget *toolBar() override;

public Q_SLOTS:
    void showAsn1Dialog();
    void showE2EDataflow(const QStringList &mscFiles);

private:
    AadlEditorDocument *m_document = nullptr;
    QPointer<QToolBar> m_toolbar = nullptr;
    AadlMainWidget *m_editorWidget = nullptr;
    QPointer<aadlinterface::EndToEndView> m_endToEndView;
    QPointer<MscModelStorage> m_mscStorage;
};

}
