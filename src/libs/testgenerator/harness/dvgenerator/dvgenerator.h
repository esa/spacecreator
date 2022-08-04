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

#include <dvcore/dvdevice.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <dvcore/dvpartition.h>
#include <ivcore/ivfunction.h>
#include <memory>
#include <vector>

namespace dvm {
class DVBoard;
class DVNode;
}

namespace testgenerator {

/**
 * @brief Test harness DeploymentView generator
 *
 */
class DvGenerator final
{
private:
    static const QString nodeLabelToken;
    static const QString nameToken;
    static const QString devNamespaceToken;
    static const QString typeToken;
    static const QString pathToken;

public:
    /**
     * @brief Coordinates of Deployment View diagram entities
     *
     */
    struct Coordinates {
        /** Coordinates of main node **/
        static QVector<qint32> node;
        /** Coordinates of partition present in the node **/
        static QVector<qint32> partition;
    };

    /**
     * @brief Generate DeploymentView model
     *
     * @param functionsToBind   functions to be included on the partition
     * @param hw                deployment hardware name
     * @param nodeTitle         title of a node to be generated
     * @param nodeLabel         label of a node to be generated
     * @param hostPartitionName name of a host partition (partition present on a node)
     *
     * @return required DeploymentView model
     */
    static auto generate(const std::vector<ivm::IVFunction *> &functionsToBind, const QString &hw,
            const QString &nodeTitle = "Node", const QString &nodeLabel = "Node_1",
            const QString &hostPartitionName = "hostPartition") -> std::unique_ptr<dvm::DVModel>;

private:
    static auto cloneDvObject(dvm::DVObject *object) -> dvm::DVObject *;
    static auto cloneFunctionAndAddToModel(
            ivm::IVFunction *function, dvm::DVModel *model, dvm::DVObject *node, const QString &partitionTitle) -> void;

    static auto getAllHwObjectsFromLib() -> QVector<dvm::DVObject *>;
    static auto getBoard(const QVector<dvm::DVObject *> &objects) -> dvm::DVBoard *;
    static auto getSelectedHwObjects(const QVector<dvm::DVObject *> &hwObjects, const QString &hwTitle)
            -> QVector<dvm::DVObject *>;

    static auto makeNodeAndAddToModel(const QString &nodeTitle, const QString &nodeLabel, dvm::DVModel *model,
            dvm::DVBoard *board) -> dvm::DVNode *;
    static auto makePartitionAndAddToNode(const QString &hostPartitionName, dvm::DVModel *model, dvm::DVNode *node)
            -> dvm::DVPartition *;

    static auto setDvObjectModelAndTitle(dvm::DVObject *object, dvm::DVModel *model, const QString &title) -> void;

    template<typename DVType>
    static auto makeDvObject(dvm::DVModel *const model, const QString &title) -> DVType *
    {
        DVType *const object = new DVType;
        object->setTitle(title);
        object->setModel(model);

        return object;
    }
};

} // namespace testgenerator
