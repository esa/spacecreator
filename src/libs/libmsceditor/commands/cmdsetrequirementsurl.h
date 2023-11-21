/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "msceditorcore.h"
#include "undocommand.h"

#include <QPointer>
#include <QUrl>

namespace msc {

class MSCEditorCore;
namespace cmd {

/*!
 * \brief The CmdSetRequirementsUrl class handles setting the URL for the requirements of a MSC file
 */
class CmdSetRequirementsUrl : public shared::UndoCommand
{
public:
    CmdSetRequirementsUrl(msc::MSCEditorCore *mscCore, const QUrl &url, QObject *parent = nullptr);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<msc::MSCEditorCore> m_mscCore;
    QUrl m_oldUrl;
    QUrl m_newUrl;
};

} // namespace cmd
} // namespace msc

