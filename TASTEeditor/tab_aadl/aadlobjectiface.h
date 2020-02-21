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

#include "aadlobject.h"
#include "aadlparameter.h"

#include <QMetaType>
#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QVector>
#include <memory>

namespace taste3 {
namespace aadl {

class AADLObjectFunction;
class AADLObjectFunctionType;
struct AADLObjectIfacePrivate;

class AADLObjectIface : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(AADLObjectIface::IfaceType direction READ direction)
    Q_PROPERTY(bool isProvided READ isProvided)
    Q_PROPERTY(bool isRequired READ isRequired)
    Q_PROPERTY(QVariantList params READ paramList)

public:
    enum class IfaceType
    {
        Required = 0,
        Provided
    };
    Q_ENUM(IfaceType)
    static constexpr IfaceType DefaultDirection { IfaceType::Required };

    enum class OperationKind
    {
        Any = 0,
        Cyclic, // period, deadline, wcet
        Sporadic, // min interval t,deadline, wcet,queuesize
        Protected, // deadline, wcet
        Unprotected, // deadline, wcet
    };
    Q_ENUM(OperationKind)

    struct CreationInfo {
        CreationInfo(AADLObjectsModel *model = nullptr, AADLObjectFunctionType *function = nullptr,
                     const QPointF &position = QPointF(), AADLObjectIface::IfaceType type = DefaultDirection,
                     const taste3::common::Id &id = common::createId(),
                     const QVector<IfaceParameter> parameters = QVector<IfaceParameter>())
            : model(model)
            , function(function)
            , position(position)
            , type(type)
            , id(id)
            , parameters(parameters)
        {
        }

        AADLObjectsModel *model { nullptr };
        AADLObjectFunctionType *function { nullptr };
        QPointF position = {};
        AADLObjectIface::IfaceType type { DefaultDirection };
        taste3::common::Id id = {};
        QVector<IfaceParameter> parameters;

        inline QVariantList toVarList() const { return { QVariant::fromValue(*this) }; };
    };

    static QMap<AADLObjectIface::OperationKind, QString> xmlKindNames();
    static QString kindToString(AADLObjectIface::OperationKind k);
    static AADLObjectIface::OperationKind kindFromString(const QString &k);

    ~AADLObjectIface() override;

    AADLObject::AADLObjectType aadlType() const override;

    AADLObjectIface::IfaceType direction() const;

    bool isProvided() const;
    bool isRequired() const;

    OperationKind kind() const;
    bool setKind(OperationKind k);

    QVector<IfaceParameter> params() const;
    QVariantList paramList() const;
    void setParams(const QVector<IfaceParameter> &params);
    void addParam(const IfaceParameter &param);

    AADLObjectFunction *function() const;

    bool isClone() const;
    bool isCloned() const;
    QVector<QPointer<AADLObjectIface>> clones() const;

    static AADLObjectIface *createIface(AADLObjectIface::IfaceType direction, const common::Id &id, AADLObject *parent,
                                        const QVector<IfaceParameter> &params = QVector<IfaceParameter>());
    static AADLObjectIface *cloneIface(AADLObjectIface *source, AADLObjectFunction *parent);

    void setAttr(const QString &name, const QVariant &val) override;

    QVariant originalAttr(const QString &name) const;
    QVariant originalProp(const QString &name) const;
    QVector<IfaceParameter> originalParams() const;

Q_SIGNALS:
    void attrChanged_kind(AADLObjectIface::OperationKind kind);
    void paramsChanged();

protected Q_SLOTS:
    void onReflectedAttrChanged(taste3::aadl::meta::Props::Token attr);
    void onReflectedPropChanged(taste3::aadl::meta::Props::Token prop);
    void onReflectedParamsChanged();

protected:
    explicit AADLObjectIface(AADLObjectIface::IfaceType direction, const QString &title, AADLObject *parent = nullptr);
    explicit AADLObjectIface(const common::Id &id, AADLObjectIface::IfaceType direction, const QString &title,
                             AADLObject *parent = nullptr);
    void setupInitialAttrs();

    void setCloneOrigin(AADLObjectIface *source);
    void rememberClone(AADLObjectIface *clone);
    void forgetClone(AADLObjectIface *clone);

    void cloneInternals(const AADLObjectIface *from);
    void restoreInternals(const AADLObjectIface *disconnectMe);

    void reflectAttrs(const AADLObjectIface *from);
    void reflectProps(const AADLObjectIface *from);
    void reflectParams(const AADLObjectIface *from);

protected:
    struct OriginalPropsHolder {
        QString name;
        QHash<QString, QVariant> attrs;
        QHash<QString, QVariant> props;
        QVector<IfaceParameter> params;
    } m_original;

private:
    const std::unique_ptr<AADLObjectIfacePrivate> d;
};

class AADLObjectIfaceProvided : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceProvided(AADLObject *parent = nullptr);
    explicit AADLObjectIfaceProvided(const QString &title, AADLObject *parent = nullptr);
    explicit AADLObjectIfaceProvided(const common::Id &id, const QString &title = QString(),
                                     AADLObject *parent = nullptr);
};

class AADLObjectIfaceRequired : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceRequired(AADLObject *parent = nullptr);
    explicit AADLObjectIfaceRequired(const QString &title, AADLObject *parent = nullptr);
    explicit AADLObjectIfaceRequired(const common::Id &id, const QString &title = QString(),
                                     AADLObject *parent = nullptr);

    virtual void setProp(const QString &name, const QVariant &val) override;

    bool inheritPi() const;
    bool hasPrototypePi() const;

    QStringList inheritedLables() const;
    void setPrototype(const AADLObjectIfaceProvided *pi);
    void unsetPrototype(const AADLObjectIfaceProvided *pi);

Q_SIGNALS:
    void propChanged_labelInheritance(bool inheritance);
    void inheritedLabelsChanged(const QStringList &labels);

protected:
    QVector<const AADLObjectIfaceProvided *> m_prototypes;

private:
    QStringList collectInheritedLabels() const;
    void namesForRIToPIs(QStringList &result) const;
    void namesForRIsToPI(QStringList &result) const;
};

typedef QVector<AADLObjectIface *> AADLIfacesVector;

} // ns aadl
} // ns taste3

Q_DECLARE_METATYPE(taste3::aadl::AADLObjectIface::IfaceType);
Q_DECLARE_METATYPE(taste3::aadl::AADLObjectIface::CreationInfo);
