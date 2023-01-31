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

#include "ivtools.h"

#include "../messagestrings.h"

#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <ivcore/ivfunction.h>
#include <memory>
#include <modeltype.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>
#include <seds/SedsXmlImporter/importer.h>
#include <vector>

namespace plugincommon {

auto IvTools::getFunctions(ivm::IVModel *model) -> QList<ivm::IVFunction *>
{
    if (model == nullptr) {
        return QList<ivm::IVFunction *>();
    }

    QList<ivm::IVFunction *> functions;

    for (auto &ivObject : model->visibleObjects()) {
        auto *const function = dynamic_cast<ivm::IVFunction *>(ivObject);
        if (function != nullptr) {
            functions.push_back(function);
        }
    }

    return functions;
}

auto IvTools::getIfaceFromModel(const QString &ifaceName, ivm::IVModel *const model) -> ivm::IVInterface *
{
    ivm::IVInterface *const ivIface = model->getIfaceByName(ifaceName, ivm::IVInterface::InterfaceType::Provided);
    if (ivIface == nullptr) {
        throw std::logic_error("requested interface not found");
    }
    return ivIface;
}

} // namespace plugincommon
