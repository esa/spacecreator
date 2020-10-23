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

namespace msc {
class AadlSystemChecks;
class ChartLayoutManager;
class MscMessage;
class MscMessageDeclaration;
class MscMessageDeclarationList;
class MscModel;
}

namespace Ui {
class MessageDialog;
}

class QTableWidgetItem;

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(
            msc::MscMessage *message, msc::ChartLayoutManager *charlayoutManager, QWidget *parent = nullptr);
    ~MessageDialog();

    void setAadlConnectionNames(const QStringList &names);
    void setAadlChecker(msc::AadlSystemChecks *checker);

public Q_SLOTS:
    void accept() override;
    void copyDeclarationData();
    void selectDeclarationFromName();
    void editDeclarations();
    void addParameter();
    void removeParameter();
    void checkRemoveButton();
    void setParameterEditState();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private Q_SLOTS:
    void editItem(QTableWidgetItem *item);
    void setItemFlags(QTableWidgetItem *item);
    void checkTextValidity();
    void checkforEmptyCompleter();

private:
    void fillMessageDeclartionBox();
    void fillParameters();

    msc::MscModel *mscModel() const;
    msc::MscMessageDeclarationList *messageDeclarations() const;

    Ui::MessageDialog *ui;
    QPointer<msc::MscMessage> m_message;
    QPointer<msc::MscMessageDeclaration> m_selectedDeclaration;
    bool m_isValid = true;
    QStringList m_connectionNames;
    QPointer<msc::AadlSystemChecks> m_aadlChecker;
    QPointer<msc::ChartLayoutManager> m_chartLayoutManager;
};
