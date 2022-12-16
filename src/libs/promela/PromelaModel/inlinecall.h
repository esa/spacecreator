/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "constant.h"
#include "realconstant.h"
#include "variableref.h"

#include <QList>
#include <QString>
#include <variant>

namespace promela::model {
/**
 * @brief Representation of inline call in promela
 */
class InlineCall
{
public:
    /**
     * @brief Argument type definition.
     */
    using Argument = std::variant<VariableRef, Constant, RealConstant>;

    /**
     * @brief Constructor
     *
     * @param name name of inline
     * @param arguments list of arguments
     */
    InlineCall(QString name, const QList<Argument> &arguments);

    /**
     * @brief Getter for name of inline
     *
     * @return Name of inline
     */
    const QString &getName() const noexcept;
    /**
     * @brief Getter for arguments of inline call
     *
     * @return List of inline arguments.
     */
    const QList<Argument> &getArguments() const noexcept;

private:
    QString m_name;
    QList<Argument> m_arguments;
};
}
