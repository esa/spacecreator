/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include <QRectF>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivmodel.h>

namespace ivmerger {

static const qreal MARGIN = 100.0;

/**
 * @brief   Merger that merges two interface view model
 */
class IvMerger final
{
public:
    /**
     * @brief   Merges two interface view models, file names as inputs
     *
     * @param   inputTargetIvFile target interface view input file name
     * @param   inputTargetIvFile source interface view input file name
     */
    bool mergeInterfaceViews(const QString &inputTargetIvFile, const QString &inputSourceIvFile);
    /**
     * @brief   Merges two interface view models, model objects as inputs
     *
     * @param   targetIvModel target interface view model object reference
     * @param   sourceIvModel source interface view model object reference
     */
    bool mergeInterfaceViews(ivm::IVModel &targetIvModel, ivm::IVModel &sourceIvModel);

private:
    using EndpointInfo = QPair<shared::Id, QString>;
    using FunctionEndpoints = QMultiMap<EndpointInfo, EndpointInfo>;

private:
    void removeFunctionAndItsChildren(ivm::IVModel &ivModel, ivm::IVFunctionType *targetFunctionToRemove);
    void collectTopLevelFunctions(
            QVector<ivm::IVFunctionType *> &allFunctions, QVector<ivm::IVFunctionType *> &topLevelFunctions);
    void mergeFunctions(ivm::IVModel &targetIvModel, ivm::IVModel &sourceIvModel,
            QVector<ivm::IVFunctionType *> &topLevelSourceFunctions, QVector<ivm::IVFunctionType *> &targetFunctions,
            QMap<ivm::IVFunctionType *, FunctionEndpoints> &connectionsToRestore,
            QSet<ivm::IVFunctionType *> &insertedSourceFunctions);
    void addNewFunctions(ivm::IVModel &targetIvModel, ivm::IVModel &sourceIvModel,
            QVector<ivm::IVFunctionType *> &topLevelSourceFunctions, QVector<ivm::IVFunctionType *> &targetFunctions,
            QMap<ivm::IVFunctionType *, FunctionEndpoints> &connectionsToRestore,
            QSet<ivm::IVFunctionType *> &insertedSourceFunctions);
    void replaceFunction(ivm::IVModel &ivModel, ivm::IVModel &sourceIvModel, ivm::IVFunctionType *currentFunction,
            ivm::IVFunctionType *newFunction, FunctionEndpoints &connectionInfos, QVector<ivm::IVFunctionType *> functionsThatWillNotBeReplaced);
    void restoreConnections(ivm::IVModel &ivModel, QVector<ivm::IVFunctionType *> &allFunctions,
            QMap<ivm::IVFunctionType *, FunctionEndpoints> connectionsToRestore);
    void reparentRecursive(ivm::IVModel &newParent, ivm::IVObject *obj);
    void realizeConnection(ivm::IVModel &ivModel, const QVector<ivm::IVFunctionType *> allTopLevelFunctions,
            ivm::IVFunctionType *fromFunction, const QString &fromInterfaceName, ivm::IVFunctionType *toFunction,
            const QString &toInterfaceName);
    bool parseInterfaceView(ivm::IVModel *model, const QString &inputIvFile);
    bool saveInterfaceView(ivm::IVModel *model, const QString &inputIvFile);
};
} // namespace ivmerger
