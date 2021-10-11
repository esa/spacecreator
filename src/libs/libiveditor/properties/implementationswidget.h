/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstack.h"

#include <QWidget>

namespace ivm {
class AbstractSystemChecks;
class IVFunction;
}

namespace Ui {
class ImplementationsWidget;
}

namespace ive {
class ImplementationsModel;

/*!
   UI to select the implementation language(s) for functions
 */
class ImplementationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImplementationsWidget(ivm::IVFunction *fn, ivm::AbstractSystemChecks *checks,
            cmd::CommandsStack::Macro *macro, QWidget *parent = nullptr);
    ~ImplementationsWidget();

private Q_SLOTS:
    void addLanguage();
    void deleteSelectedLanguage();
    void updateDeleteButton();
    void rowsInserted(const QModelIndex &parent, int first, int last);

private:
    Ui::ImplementationsWidget *ui;
    ImplementationsModel *m_model = nullptr;
};

} // namespace ive
