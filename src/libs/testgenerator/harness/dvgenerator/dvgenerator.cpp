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

#include "dvgenerator.h"

#include <QDebug>
#include <dvcore/dvcommonprops.h>
#include <dvcore/dvfunction.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvnode.h>
#include <dvcore/dvobject.h>
#include <dvcore/dvpartition.h>
#include <dvcore/dvport.h>
#include <memory>
#include <shared/common.h>
#include <shared/entityattribute.h>
#include <shared/exportableproperty.h>
#include <shared/parameter.h>
#include <shared/propertytemplateconfig.h>
#include <sstream>
#include <stdexcept>

namespace testgenerator {

auto DvGenerator::generate(const std::vector<ivm::IVFunction *> &functionsToBind) -> std::unique_ptr<dvm::DVModel>
{
    auto model = std::make_unique<dvm::DVModel>();

    dvm::DVObject *const node = new dvm::DVNode;
    node->setTitle("x86_Linux_TestRunner");
    node->setModel(model.get());
    const auto nodeToken = dvm::meta::Props::token(dvm::meta::Props::Token::Node);
    node->setEntityProperty(nodeToken, "");
    model->addObject(node);

    dvm::DVObject *const partition = new dvm::DVPartition;
    partition->setTitle("hostPartition");
    partition->setModel(model.get());
    const auto partitionToken = dvm::meta::Props::token(dvm::meta::Props::Token::Partition);
    partition->setEntityProperty(partitionToken, "");
    model->addObject(partition);

    for (const auto &function : functionsToBind) {
        dvm::DVObject *const funToAdd = new dvm::DVFunction;
        funToAdd->setTitle(function->title());
        funToAdd->setModel(model.get());
        model->addObject(funToAdd);
    }

    const auto deviceToken = dvm::meta::Props::token(dvm::meta::Props::Token::Device);

    dvm::DVObject *const dev1 = new dvm::DVDevice;
    dev1->setTitle("eth0");
    dev1->setModel(model.get());
    dev1->setEntityProperty(deviceToken, "");
    model->addObject(dev1);

    dvm::DVObject *const dev2 = new dvm::DVDevice;
    dev2->setTitle("uart0");
    dev2->setModel(model.get());
    dev2->setEntityProperty(deviceToken, "");
    model->addObject(dev2);

    return model;
}

auto DvGenerator::setObjectCoordinates(dvm::DVObject *const object, const QVector<qint32> &coordinates) -> void
{
    object->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::coordinates),
            dvm::DVObject::coordinatesToString(coordinates));
}

auto DvGenerator::throwOnNullpointer(void *const pointer) -> void
{
    if (pointer == nullptr) {
        throw std::invalid_argument("pointer cannot be null");
    }
}

} // namespace testgenerator
