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
class IVFunctionType;
class ArchetypeModel;
}

namespace Ui {
class ArchetypesWidget;
}

namespace ive {
class ArchetypesWidgetModel;

/*!
   UI to select the archetype(s) for functions
 */
class ArchetypesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArchetypesWidget(ivm::ArchetypeModel *archetypeModel, ivm::IVFunctionType *function, ivm::AbstractSystemChecks *checks,
            cmd::CommandsStack::Macro *macro, QWidget *parent = nullptr);
    ~ArchetypesWidget();

private:
    Ui::ArchetypesWidget *ui;
    ArchetypesWidgetModel *m_model = nullptr;
};

} // namespace ive
