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

#include "specialized/functionstranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::TranslationException;

namespace conversion::seds::translator {

void FunctionsTranslator::translateFunction(const ivm::IVFunction *ivFunction, ::seds::model::Package &sedsPackage)
{
    ::seds::model::Component sedsComponent;

    auto ivFunctionName =
            ivFunction->entityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name)).value<QString>();

    for (const auto ivInterface : ivFunction->allInterfaces()) {
        translateInterface(ivInterface, sedsComponent);
    }

    sedsPackage.addComponent(std::move(sedsComponent));
}

void FunctionsTranslator::translateInterface(
        const ivm::IVInterface *ivInterface, ::seds::model::Component &sedsComponent)
{
    Q_UNUSED(sedsComponent);

    switch (ivInterface->kind()) {
    case ivm::IVInterface::OperationKind::Cyclic:
        throw TranslationException("Cyclic interface translation not implemented");
        break;
    case ivm::IVInterface::OperationKind::Sporadic:
        throw TranslationException("Sporadic interface translation not implemented");
        break;
    case ivm::IVInterface::OperationKind::Protected:
        throw TranslationException("Protected interface translation not implemented");
        break;
    case ivm::IVInterface::OperationKind::Unprotected:
        throw TranslationException("Unprotected interface translation not implemented");
        break;
    default:
        throw TranslationException("Unhandled OperationKind value");
        break;
    }
}

} // namespace conversion::seds::translator
