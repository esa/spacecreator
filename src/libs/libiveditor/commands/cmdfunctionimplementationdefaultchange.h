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
 along with this program. If not, see
 <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "cmdfunctionattrchange.h"

#include <QPointer>

class EntityAttribute;

namespace ivm {
class IVFunction;
}

namespace ive {
namespace cmd {

class CmdFunctionImplementationDefaultChange : public CmdFunctionAttrChange
{
    Q_OBJECT
public:
    explicit CmdFunctionImplementationDefaultChange(
            const QString &projectPath, ivm::IVFunction *function, const QString &defaultName, const QString &language);

    void redo() override;
    void undo() override;
    int id() const override;

private:
    QString currentImplementationPath(const QString &language) const;
    QString newCurrentImplementationPath() const;
    QString oldCurrentImplementationPath() const;

    QString implementationPath(const QString &language, const QString &implName) const;
    QString newImplementationPath() const;
    QString oldImplementationPath() const;

private:
    const QString m_projectPath;
};

} // namespace cmd
} // namespace ive
