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

#include <algorithm>
#include <cstddef>
#include <dvcore/dvboard.h>
#include <dvcore/dvcommonprops.h>
#include <dvcore/dvdevice.h>
#include <dvcore/dvfunction.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvnode.h>
#include <dvcore/dvobject.h>
#include <dvcore/dvpartition.h>
#include <memory>
#include <shared/entityattribute.h>

namespace testgenerator {

QVector<QVector<qint32>> DvGenerator::DvCoordinates::devices;
DvGenerator::Coordinates DvGenerator::DvCoordinates::node = { 192, 193, 396, 353 };
DvGenerator::Coordinates DvGenerator::DvCoordinates::partition = { 236, 237, 356, 317 };

const QString DvGenerator::X86_LINUX_CPP = "x86 Linux CPP";
const QString DvGenerator::SAM_V71_FREERTOS_N7S = "SAM V71 FreeRTOS N7S";
const QString DvGenerator::X86_LINUX_POHIC = "x86 Linux POHIC";
const QString DvGenerator::GR740_RTEMS_POHIC = "GR740 RTEMS POHIC";
const QString DvGenerator::RASPBERRY_PI_LINUX_POHIC = "Raspberry PI Linux POHIC";
const QString DvGenerator::ZYNQ_ZC706_RTEMS_POHIC = "ZynQ ZC706 RTEMS POHIC";
const QString DvGenerator::BRAVE_LARGE_FREERTOS = "BRAVE_Large FreeRTOS";
const QString DvGenerator::LINUX_ARM_RUNTIME = "Linux ARM Runtime";

auto DvGenerator::generate(const std::vector<ivm::IVFunction *> &functionsToBind,
        const QVector<dvm::DVObject *> &hwObjects, const QString &nodeName, const QString &nodeLabel,
        const QString &hostPartitionName) -> std::unique_ptr<dvm::DVModel>
{
    DvCoordinates::devices = {
        { 192, 210 },
        { 192, 251 },
    };

    auto model = std::make_unique<dvm::DVModel>();

    dvm::DVNode *const node = makeNodeAndAddToModel(nodeName, nodeLabel, model.get(), getBoard(hwObjects));
    makePartitionAndAddToNode(hostPartitionName, model.get(), node);

    const QVector<dvm::DVObject *> hwDevices = getDevices(hwObjects);
    std::for_each(hwDevices.begin(), hwDevices.end(),
            [&](const auto &device) { cloneDeviceAndAddToModelAndNode(device, model.get(), node); });

    std::for_each(functionsToBind.begin(), functionsToBind.end(),
            [&](const auto &function) { cloneFunctionAndAddToModel(function, model.get(), node, hostPartitionName); });

    return model;
}

auto DvGenerator::getBoard(const QVector<dvm::DVObject *> &objects) -> dvm::DVBoard *
{
    for (const auto &object : objects) {
        if (object->type() == dvm::DVObject::Type::Board) {
            return static_cast<dvm::DVBoard *>(object);
        }
    }
    return nullptr;
}

auto DvGenerator::setDvObjectModelAndTitle(dvm::DVObject *const object, dvm::DVModel *const model, const QString &title)
        -> void
{
    object->setTitle(title);
    object->setModel(model);
}

auto DvGenerator::cloneDvObject(dvm::DVObject *const object) -> dvm::DVObject *
{
    dvm::DVObject *const copy = new dvm::DVObject(object->type(), object->title());

    for (const auto &entityAttribute : object->entityAttributes()) {
        copy->setEntityAttribute(entityAttribute.name(), entityAttribute.value());
    }
    for (const auto &propertyName : object->dynamicPropertyNames()) {
        copy->setProperty(propertyName, object->property(propertyName));
    }

    return copy;
}

auto DvGenerator::getDevices(const QVector<dvm::DVObject *> &objects) -> QVector<dvm::DVObject *>
{
    QVector<dvm::DVObject *> devices;

    std::for_each(objects.begin(), objects.end(), [&devices](const auto &object) {
        if (object->type() == dvm::DVObject::Type::Port) {
            devices << object;
        }
    });

    return devices;
}

auto DvGenerator::cloneDeviceAndAddToModelAndNode(
        dvm::DVObject *const device, dvm::DVModel *const model, dvm::DVObject *const node) -> void
{
    const QString devName = device->entityAttributeValue(nameToken).toString();
    const QString asn1Module = device->entityAttributeValue(asn1moduleToken).toString();
    const QString devNamespace = device->entityAttributeValue(devNamespaceToken).toString();
    const QString requiresBusAccess = device->entityAttributeValue(requiresBusAccessToken).toString();
    const QString extends = device->entityAttributeValue(extendsToken).toString();
    const QString busNamespace = device->entityAttributeValue(busNamespaceToken).toString();
    const QString asn1File = device->entityAttributeValue(asn1fileToken).toString();
    const QString asn1Type = device->entityAttributeValue(asn1typeToken).toString();
    const QString implExtends = device->entityAttributeValue(implExtendsToken).toString();

    auto *const dev = makeDvObject<dvm::DVDevice>(model, devName);
    dev->setParentObject(node);

    dev->setCoordinates(DvCoordinates::devices.first());
    DvCoordinates::devices.removeFirst();
    dev->setEntityAttribute(asn1moduleToken, asn1Module);
    dev->setEntityAttribute(devNamespaceToken, devNamespace);
    dev->setEntityAttribute(requires_bus_accessToken, requiresBusAccess);
    dev->setEntityAttribute(extendsToken, extends);
    dev->setEntityAttribute(busNamespaceToken, busNamespace);
    dev->setEntityAttribute(asn1fileToken, asn1File);
    dev->setEntityAttribute(asn1typeToken, asn1Type);
    dev->setEntityAttribute(implExtendsToken, implExtends);
    dev->setEntityAttribute(portToken, devName);

    static_cast<dvm::DVNode *>(node)->addDevice(static_cast<dvm::DVDevice *>(dev));
    model->addObject(dev);
}

auto DvGenerator::cloneFunctionAndAddToModel(ivm::IVFunction *function, dvm::DVModel *model, dvm::DVObject *const node,
        const QString &partitionTitle) -> void
{
    auto *const dvNode = static_cast<dvm::DVNode *>(node);
    for (const auto &partition : dvNode->partitions()) {
        if (partition->title().compare(partitionTitle) == 0) {
            auto *const fun = makeDvObject<dvm::DVFunction>(model, function->title());
            fun->setEntityAttribute(pathToken, function->title());
            fun->setParentObject(partition);
            const auto dvPartition = static_cast<dvm::DVPartition *>(partition);

            dvPartition->addFunction(static_cast<dvm::DVFunction *>(fun));
            dvNode->addPartition(dvPartition);
            model->addObject(fun);
        }
    }
}

auto DvGenerator::makeNodeAndAddToModel(const QString &nodeName, const QString &nodeLabel, dvm::DVModel *const model,
        dvm::DVBoard *const board) -> dvm::DVNode *
{
    auto *const node = makeDvObject<dvm::DVNode>(model, nodeName);
    node->setCoordinates(DvCoordinates::node);
    node->setEntityAttribute(nameToken, nodeName);
    node->setEntityAttribute(nodeLabelToken, nodeLabel);
    node->setEntityAttribute(typeToken, board->entityAttributeValue(typeToken));
    node->setEntityAttribute(devNamespaceToken, board->entityAttributeValue(devNamespaceToken));

    model->addObject(node);

    return node;
}

auto DvGenerator::makePartitionAndAddToNode(
        const QString &hostPartitionName, dvm::DVModel *const model, dvm::DVNode *const node) -> dvm::DVPartition *
{
    auto *const partition = makeDvObject<dvm::DVPartition>(model, hostPartitionName);

    partition->setCoordinates(DvCoordinates::partition);
    partition->setParentObject(node);
    partition->setParent(node);

    model->addObject(partition);
    node->addPartition(partition);

    return partition;
}

const QString DvGenerator::nodeLabelToken = dvm::meta::Props::token(dvm::meta::Props::Token::node_label);
const QString DvGenerator::nameToken = dvm::meta::Props::token(dvm::meta::Props::Token::name);
const QString DvGenerator::asn1moduleToken = dvm::meta::Props::token(dvm::meta::Props::Token::asn1module);
const QString DvGenerator::devNamespaceToken = "namespace";
const QString DvGenerator::requiresBusAccessToken = "requiresBusAccess";
const QString DvGenerator::requires_bus_accessToken =
        dvm::meta::Props::token(dvm::meta::Props::Token::requires_bus_access);
const QString DvGenerator::extendsToken = "extends";
const QString DvGenerator::busNamespaceToken = "bus_namespace";
const QString DvGenerator::asn1fileToken = dvm::meta::Props::token(dvm::meta::Props::Token::asn1file);
const QString DvGenerator::asn1typeToken = dvm::meta::Props::token(dvm::meta::Props::Token::asn1type);
const QString DvGenerator::implExtendsToken = "impl_extends";
const QString DvGenerator::portToken = dvm::meta::Props::token(dvm::meta::Props::Token::port);
const QString DvGenerator::typeToken = dvm::meta::Props::token(dvm::meta::Props::Token::type);
const QString DvGenerator::pathToken = dvm::meta::Props::token(dvm::meta::Props::Token::path);

} // namespace testgenerator
