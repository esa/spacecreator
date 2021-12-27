/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "ivconnectionlayertype.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "vemodel.h"

#include <QAbstractItemModel>
#include <QVector>
#include <conversion/common/model.h>
#include <memory>

namespace shared {
class PropertyTemplateConfig;
}
namespace ivm {

class IVComment;
class IVConnection;
class IVFunction;
class IVFunctionType;

struct IVModelPrivate;
class IVModel : public shared::VEModel, public conversion::Model
{
    Q_OBJECT
public:
    explicit IVModel(
            shared::PropertyTemplateConfig *dynPropConfig, IVModel *sharedModel = nullptr, QObject *parent = nullptr);
    ~IVModel() override;

    bool removeObject(shared::VEObject *obj) override;

    void setRootObject(shared::Id rootId);
    IVObject *rootObject() const;
    shared::Id rootObjectId() const;

    IVObject *getObject(const shared::Id &id) const override;
    IVObject *getObjectByName(const QString &name, IVObject::Type type = IVObject::Type::Unknown,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    IVInterface *getIfaceByName(const QString &name, IVInterface::InterfaceType dir,
            const IVFunctionType *parent = nullptr, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    QList<IVInterface *> getIfacesByName(
            const QString &name, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    IVFunction *getFunction(const shared::Id &id) const;
    IVFunction *getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    IVFunctionType *getFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    IVFunctionType *getFunctionType(const shared::Id &id) const;
    IVFunctionType *getSharedFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    IVFunctionType *getSharedFunctionType(const shared::Id &id) const;
    QHash<QString, IVFunctionType *> getAvailableFunctionTypes(const IVFunction *fnObj) const;
    IVInterface *getInterface(const shared::Id &id) const;
    IVInterfaceRequired *getRequiredInterface(const shared::Id &id) const;
    IVInterfaceProvided *getProvidedInterface(const shared::Id &id) const;
    IVConnection *getConnection(const shared::Id &id) const;
    IVComment *getCommentById(const shared::Id &id) const;
    IVConnection *getConnection(const QString &interfaceName, const QString &source, const QString &target,
            Qt::CaseSensitivity caseSensitivity) const;

    QSet<QString> nestedFunctionNames(const IVFunctionType *fnt = nullptr) const;
    QSet<QStringList> nestedFunctionPaths(const IVFunctionType *fnt = nullptr) const;

    IVConnection *getConnectionForIface(const shared::Id &id) const;
    QVector<IVConnection *> getConnectionsForIface(const shared::Id &id) const;

    QVector<IVConnection *> getConnectionsForFunction(const shared::Id &id) const;

    auto setConnectionLayersModel(IVModel *layersModel) -> void;
    auto getConnectionLayerByName(const QString &name) const -> IVConnectionLayerType *;

    QList<IVObject *> visibleObjects() const;
    QList<IVObject *> visibleObjects(shared::Id rootId) const;

    void clear() override;

    virtual conversion::ModelType modelType() const override;

    shared::PropertyTemplateConfig *dynPropConfig() const;

    QString defaultFunctionLanguage() const;
    QStringList availableFunctionLanguages() const;

Q_SIGNALS:
    void rootObjectChanged(shared::Id rootId);

protected:
    bool addObjectImpl(shared::VEObject *obj) override;

private:
    const std::unique_ptr<IVModelPrivate> d;
};

}

namespace conversion {

/**
 * @brief   Specialization for InterfaceView model
 */
template<>
struct ModelProperties<ivm::IVModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::InterfaceView;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
