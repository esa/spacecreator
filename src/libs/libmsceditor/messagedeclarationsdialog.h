/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QDialog>
#include <QPointer>
#include <QStringList>
#include <QVariantList>
#include <memory>

namespace msc {
class MscCommandsStack;
class MscModel;
class MscMessageDeclaration;
class MscMessageDeclarationList;
class SystemChecks;
}

namespace Ui {
class MessageDeclarationsDialog;
}

class MessageDeclarationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDeclarationsDialog(msc::MscMessageDeclarationList *model, msc::MscModel *mscModel,
            msc::MscCommandsStack *undoStack, msc::SystemChecks *checker, QWidget *parent = nullptr);
    ~MessageDeclarationsDialog();

    msc::MscMessageDeclarationList *declarations() const;

    void setFileName(const QString &fileName);
    const QString &fileName() const;

    void setIVConnectionNames(const QStringList &names);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private Q_SLOTS:
    void showSelectedMessage();
    void addDeclaration();
    void removeDeclaration();

    void addSelectedParameter();
    void removeSelectedParameter();

    void updateNames();
    void updateDeclarationDetails();
    void updateParameterButtons();

    void selectAsn1File();

    void checkforEmptyCompleter();

    void importFromIV();

private:
    void updateAsn1TypesView();

    Ui::MessageDeclarationsDialog *ui;
    msc::MscMessageDeclarationList *m_model = nullptr;
    msc::MscMessageDeclaration *m_selectedDeclaration = nullptr;
    msc::MscModel *m_mscModel;
    QString m_fileName;
    QPointer<msc::MscCommandsStack> m_undoStack;
    QPointer<msc::SystemChecks> m_checker;
};
