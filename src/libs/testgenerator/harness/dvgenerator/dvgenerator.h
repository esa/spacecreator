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

#pragma once

#include <dvcore/dvboard.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <ivcore/ivfunction.h>
#include <memory>
#include <vector>

namespace testgenerator {

/**
 * @brief Test harness DeploymentView generator
 *
 */
class DvGenerator final
{
public:
    typedef QVector<qint32> Coordinates;

    struct DvCoordinates {
        static QVector<Coordinates> devices;
        static Coordinates node;
        static Coordinates partition;
    };

    static const QString hostPartitionName;
    static const QString nodeName;

    static auto generate(const std::vector<ivm::IVFunction *> &functionsToBind, const QVector<dvm::DVObject *> &hw,
            const QString &modelName) -> std::unique_ptr<dvm::DVModel>;

private:
    static auto findBoard(const QVector<dvm::DVObject *> &objects) -> dvm::DVBoard *;
    static auto setDvObjectModelAndTitle(dvm::DVObject *object, dvm::DVModel *model, const QString &title) -> void;
    static auto copyDvObject(dvm::DVObject *object) -> dvm::DVObject *;
    static auto getDevices(const QVector<dvm::DVObject *> &objects) -> QVector<dvm::DVObject *>;

    template<typename T>
    static auto makeDvObject(dvm::DVModel *const model, const QString &title) -> dvm::DVObject *
    {
        dvm::DVObject *const object = new T;
        object->setTitle(title);
        object->setModel(model);

        return object;
    }
};

} // namespace testgenerator
