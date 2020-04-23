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

#ifndef CMDSETASN1FILE_H
#define CMDSETASN1FILE_H

#include <QPointer>
#include <QUndoCommand>

namespace msc {
class MscModel;

namespace cmd {

class CmdSetAsn1File : public QUndoCommand
{
public:
    CmdSetAsn1File(msc::MscModel *model, const QString &filename, const QString &language);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<msc::MscModel> m_model;
    QString m_newFilename;
    QString m_oldFilename;
    QString m_newLanguage;
    QString m_oldLanguage;
};

} // namespace cmd
} // namespace msc

#endif // CMDSETASN1FILE_H
