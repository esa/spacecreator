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

#include <QPointer>
#include <texteditor/textdocument.h>

QT_FORWARD_DECLARE_CLASS(QDesignerFormWindowInterface)

namespace MscPlugin {

class MainWidget;

class MscEditorDocument : public TextEditor::TextDocument
{
    Q_OBJECT

public:
    explicit MscEditorDocument(MainWidget *designWidget, QObject *parent = nullptr);

    // IDocument
    OpenResult open(QString *errorString, const QString &fileName, const QString &realFileName) override;
    bool save(QString *errorString, const QString &fileName, bool autoSave) override;
    bool shouldAutoSave() const override;
    bool isSaveAsAllowed() const override;
    bool isModified() const override;
    bool reload(QString *errorString, ReloadFlag flag, ChangeType type) override;

    // Internal
    MainWidget *designWidget() const;
    void syncXmlFromDesignWidget();
    QString designWidgetContents() const;
    void setFilePath(const Utils::FileName &) override;

Q_SIGNALS:
    void reloadRequested(QString *errorString, const QString &);

private:
    QPointer<MainWidget> m_designWidget;
};

}
