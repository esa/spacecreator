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
#include <dvcore/dvdevice.h>
#include <dvcore/dvfunction.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvnode.h>
#include <dvcore/dvobject.h>
#include <dvcore/dvpartition.h>
#include <memory>
#include <shared/entityattribute.h>

namespace testgenerator {

QVector<QVector<qint32>> DvCoordinates::devices;

auto DvGenerator::generate(const std::vector<ivm::IVFunction *> &functionsToBind, const QVector<dvm::DVObject *> &hw,
        const QString &modelName) -> std::unique_ptr<dvm::DVModel>
{
    auto model = std::make_unique<dvm::DVModel>();
    DvCoordinates::devices = {
        { 192, 210 },
        { 192, 251 },
    };

    dvm::DVBoard *const board = findBoard(hw);

    auto *const node = makeDvObject<dvm::DVNode>(model.get(), modelName);
    node->setCoordinates({ 192, 193, 396, 353 });
    node->setEntityAttribute("node_label", "Node_1");
    node->setEntityAttribute("type", board->entityAttributeValue("type"));
    node->setEntityAttribute("namespace", board->entityAttributeValue("namespace"));

    auto *const partition = makeDvObject<dvm::DVPartition>(model.get(), "hostPartition");
    partition->setCoordinates({ 236, 237, 356, 317 });
    partition->setParentObject(node);

    const auto devices = getDevices(hw);
    std::for_each(devices.begin(), devices.end(), [&model, &node](const auto device) {
        const QString devName = device->entityAttributeValue("name").toString();
        const QString asn1Module = device->entityAttributeValue("asn1module").toString();
        const QString devNamespace = device->entityAttributeValue("namespace").toString();
        const QString requiresBusAccess = device->entityAttributeValue("requiresBusAccess").toString();
        const QString extends = device->entityAttributeValue("extends").toString();
        const QString busNamespace = device->entityAttributeValue("bus_namespace").toString();
        const QString asn1File = device->entityAttributeValue("asn1file").toString();
        const QString asn1Type = device->entityAttributeValue("asn1type").toString();
        const QString implExtends = device->entityAttributeValue("impl_extends").toString();

        auto *const dev = makeDvObject<dvm::DVDevice>(model.get(), devName);

        dev->setParentObject(node);
        dev->setCoordinates(DvCoordinates::devices.first());
        DvCoordinates::devices.removeFirst();
        dev->setEntityAttribute("asn1module", asn1Module);
        dev->setEntityAttribute("namespace", devNamespace);
        dev->setEntityAttribute("requires_bus_access", requiresBusAccess);
        dev->setEntityAttribute("extends", extends);
        dev->setEntityAttribute("bus_namespace", busNamespace);
        dev->setEntityAttribute("asn1file", asn1File);
        dev->setEntityAttribute("asn1type", asn1Type);
        dev->setEntityAttribute("impl_extends", implExtends);
        dev->setEntityAttribute("port", devName);

        static_cast<dvm::DVNode *>(node)->addDevice(static_cast<dvm::DVDevice *>(dev));
        model->addObject(dev);
    });

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

    return model;
}

auto DvGenerator::findBoard(const QVector<dvm::DVObject *> &objects) -> dvm::DVBoard *
{
    for (const auto &obj : objects) {
        if (obj->type() == dvm::DVObject::Type::Board) {
            return static_cast<dvm::DVBoard *>(obj);
        }
    }
    return nullptr;
}

auto DvGenerator::setDvObjModelAndTitle(dvm::DVObject *obj, dvm::DVModel *const model, const QString &title) -> void
{
    obj->setTitle(title);
    obj->setModel(model);
}

auto DvGenerator::copyDvObject(dvm::DVObject *obj) -> dvm::DVObject *
{
    dvm::DVObject *const copy = new dvm::DVObject(obj->type(), obj->title());
    for (const auto &entityAttribute : obj->entityAttributes()) {
        copy->setEntityAttribute(entityAttribute.name(), entityAttribute.value());
    }
    // TODO: copy attributes
    // TODO: copy properties

    return copy;
}

auto DvGenerator::getDevices(const QVector<dvm::DVObject *> &objects) -> QVector<dvm::DVObject *>
{
    QVector<dvm::DVObject *> devices;

    std::for_each(objects.begin(), objects.end(), [&devices](const auto &obj) {
        if (obj->type() == dvm::DVObject::Type::Port) {
            devices << obj;
        }
    });

    return devices;
}

} // namespace testgenerator
