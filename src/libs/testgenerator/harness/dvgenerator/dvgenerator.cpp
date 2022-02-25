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
#include <dvcore/dvdevice.h>
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

template<typename T>
static auto makeDvObject(dvm::DVModel *const model, const QString &title) -> dvm::DVObject *
{
    dvm::DVObject *const object = new T;
    object->setTitle(title);
    object->setModel(model);

    return object;
}

auto DvGenerator::generate(const std::vector<ivm::IVFunction *> &functionsToBind) -> std::unique_ptr<dvm::DVModel>
{
    auto model = std::make_unique<dvm::DVModel>();

    auto *const node = makeDvObject<dvm::DVNode>(model.get(), "x86_Linux_TestRunner");
    node->setCoordinates({ 192, 193, 396, 353 });
    node->setEntityAttribute("node_label", "Node_1");
    node->setEntityAttribute("type", "ocarina_processors_x86::x86.generic_linux");
    node->setEntityAttribute("namespace", "ocarina_processors_x86");

    auto *const partition = makeDvObject<dvm::DVPartition>(model.get(), "hostPartition");
    partition->setCoordinates({ 236, 237, 356, 317 });
    partition->setParentObject(node);

    auto *const dev1 = makeDvObject<dvm::DVDevice>(model.get(), "eth0");
    dev1->setCoordinates({ 192, 210 });
    dev1->setEntityAttribute("asn1module", "LINUX-SOCKET-IP-DRIVER");
    dev1->setEntityAttribute("namespace", "ocarina_drivers");
    dev1->setEntityAttribute("requires_bus_access", "ocarina_buses::ip.generic");
    dev1->setEntityAttribute("extends", "ocarina_drivers::ip_socket");
    dev1->setEntityAttribute("bus_namespace", "ocarina_buses");
    dev1->setEntityAttribute(
            "asn1file", "/home/taste/tool-inst/include/TASTE-Linux-Drivers/configurations/linux-socket-ip-driver.asn");
    dev1->setEntityAttribute("asn1type", "Socket-IP-Conf-T");
    dev1->setEntityAttribute("impl_extends", "ocarina_drivers::ip_socket.linux");
    dev1->setEntityAttribute("port", "eth0");
    dev1->setParentObject(node);

    auto *const dev2 = makeDvObject<dvm::DVDevice>(model.get(), "uart0");
    dev2->setCoordinates({ 192, 251 });
    dev2->setEntityAttribute("asn1module", "LINUX-SERIAL-CCSDS-DRIVER");
    dev2->setEntityAttribute("namespace", "ocarina_drivers");
    dev2->setEntityAttribute("requires_bus_access", "ocarina_buses::serial.ccsds");
    dev2->setEntityAttribute("extends", "ocarina_drivers::serial_ccsds");
    dev2->setEntityAttribute("bus_namespace", "ocarina_buses");
    dev2->setEntityAttribute("asn1file",
            "/home/taste/tool-inst/include/TASTE-Linux-Drivers/configurations/linux-serial-ccsds-driver.asn");
    dev2->setEntityAttribute("asn1type", "Serial-CCSDS-Linux-Conf-T");
    dev2->setEntityAttribute("impl_extends", "ocarina_drivers::serial_ccsds.linux");
    dev2->setEntityAttribute("port", "uart0");
    dev2->setParentObject(node);

    auto *const dvNode = static_cast<dvm::DVNode *>(node);
    auto *const dvPartition = static_cast<dvm::DVPartition *>(partition);
    if (dvNode == nullptr || dvPartition == nullptr) {
        throw std::runtime_error("DVObject could not be converted to DVType");
    }

    for (const auto &function : functionsToBind) {
        auto *const fun = makeDvObject<dvm::DVFunction>(model.get(), function->title());
        fun->setEntityAttribute("path", function->title());
        fun->setParentObject(partition);
        dvPartition->addFunction(static_cast<dvm::DVFunction *>(fun));
        model->addObject(fun);
    }

    dvNode->addPartition(dvPartition);

    model->addObject(node);
    model->addObject(partition);
    model->addObject(dev1);
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
