/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "commandsstack.h"

#include <QDialog>
#include <QPointer>
#include <memory>

namespace Ui {
class ArchetypesManagerDialog;
}

namespace ivm {
class IVModel;
}

namespace ive {
class ArchetypesManagerModel;

class ArchetypesManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArchetypesManagerDialog(
            ivm::IVModel *objectsModel, cmd::CommandsStack *commandsStack, QWidget *parent = nullptr);
    ~ArchetypesManagerDialog() override;

    void addArchetypeLibrary();
    void deleteArchetypeLibrary();

    void init();

private:
    bool checkReferences();

public Q_SLOTS:
    void accept() override;
    void reject() override;

private:
    QPointer<ivm::IVModel> m_objectsModel;
    QPointer<cmd::CommandsStack> m_commandsStack;
    ArchetypesManagerModel *m_model = nullptr;
    std::unique_ptr<Ui::ArchetypesManagerDialog> m_ui;
    std::unique_ptr<cmd::CommandsStack::Macro> m_cmdMacro;
};

} // namespace ive
