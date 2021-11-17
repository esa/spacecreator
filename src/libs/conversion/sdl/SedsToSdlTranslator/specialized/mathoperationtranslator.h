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

#include <asn1library/asn1/asn1model.h>
#include <functional>
#include <queue>
#include <seds/SedsModel/components/activities/mathoperation.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion::sdl::translator {

/**
 *  @brief Translator of SEDS Math Operation into SDL task contents
 */
class MathOperationTranslator final
{
public:
    static auto translateOperation(const seds::model::MathOperation::Elements &elements) -> QString;

private:
    using Expression = std::queue<const seds::model::MathOperation::Element *>;

    static auto extractMutableExpression(const seds::model::MathOperation::Elements &elements) -> Expression;
    static auto translateMutableExpression(Expression &expression, bool &outIsComplexValue) -> QString;
    static auto translateOperator(const seds::model::Operator &op, Expression &expression, bool &outIsComplexValue)
            -> QString;
    static auto formExpression(const QString pattern, const QString left, const bool isLeftComplex, const QString right,
            const bool isRightComplex) -> QString;
};

}
