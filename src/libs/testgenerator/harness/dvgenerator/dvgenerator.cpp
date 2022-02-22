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

#include <dvcore/dvcommonprops.h>
#include <dvcore/dvfunction.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvnode.h>
#include <dvcore/dvobject.h>
#include <dvcore/dvpartition.h>
#include <dvcore/dvport.h>
#include <memory>
#include <shared/common.h>
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
    model->addObject(node);

    dvm::DVObject *const partition = new dvm::DVPartition;
    partition->setTitle("hostPartition");
    partition->setModel(model.get());
    model->addObject(partition);

    (void)functionsToBind;

    dvm::DVObject *const fun1 = new dvm::DVFunction;
    fun1->setTitle("TestDriver");
    fun1->setModel(model.get());
    model->addObject(fun1);

    dvm::DVObject *const fun2 = new dvm::DVFunction;
    fun2->setTitle("FunctionUnderTest");
    fun2->setModel(model.get());
    model->addObject(fun2);

    dvm::DVObject *const port1 = new dvm::DVPort;
    port1->setTitle("eth0");
    port1->setModel(model.get());
    model->addObject(port1);

    dvm::DVObject *const port2 = new dvm::DVPort;
    port2->setTitle("uart0");
    port2->setModel(model.get());
    model->addObject(port2);

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
