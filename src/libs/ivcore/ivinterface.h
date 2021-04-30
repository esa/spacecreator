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

#include "ivobject.h"
#include "parameter.h"

#include <QHash>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QVector>
#include <memory>

namespace ivm {

class IVFunction;
class IVFunctionType;
struct IVInterfacePrivate;

class IVInterface : public IVObject
{
    Q_OBJECT
    Q_PROPERTY(IVInterface::InterfaceType direction READ direction CONSTANT)

public:
    enum class InterfaceType
    {
        Required,
        Provided,
        Grouped
    };
    Q_ENUM(InterfaceType)
    static constexpr InterfaceType DefaultDirection { InterfaceType::Required };

    enum class OperationKind
    {
        Any = 0,
        Cyclic,
        Sporadic,
        Protected,
        Unprotected,
    };
    Q_ENUM(OperationKind)

    struct CreationInfo {
        enum class Policy
        {
            Init = 0,
            Clone
        };
        CreationInfo(IVModel *model = nullptr, IVFunctionType *function = nullptr, const QPointF &position = QPointF(),
                IVInterface::InterfaceType type = DefaultDirection, const shared::Id &id = shared::createId(),
                const QVector<InterfaceParameter> &parameters = QVector<InterfaceParameter>(),
                OperationKind kind = OperationKind::Sporadic, const QString &name = QString(),
                const CreationInfo::Policy policy = CreationInfo::Policy::Init, IVInterface *toBeCloned = nullptr);
        IVModel *model { nullptr };
        IVFunctionType *function { nullptr };
        QPointF position = {};
        IVInterface::InterfaceType type { DefaultDirection };
        shared::Id id = {};
        QVector<InterfaceParameter> parameters = {};
        OperationKind kind = { OperationKind::Sporadic };
        QString name {};
        CreationInfo::Policy policy { CreationInfo::Policy::Init };
        IVInterface *toBeCloned { nullptr };

        QVariantList toVarList() const;
        static CreationInfo initFromIface(IVInterface *iface, const CreationInfo::Policy policy);
        static CreationInfo fromIface(IVInterface *iface);
        static CreationInfo cloneIface(IVInterface *iface, IVFunction *fn);

        void resetKind();
    };

    QMap<IVInterface::OperationKind, QString> availableKindNames() const;
    static QString kindToString(IVInterface::OperationKind k);
    static IVInterface::OperationKind kindFromString(const QString &k, IVInterface::OperationKind defaultKind);
    IVInterface::OperationKind kindFromString(const QString &k) const;

    ~IVInterface() override;

    IVInterface::InterfaceType direction() const;

    bool isProvided() const;
    bool isRequired() const;

    OperationKind kind() const;
    bool setKind(OperationKind k);

    QVector<InterfaceParameter> params() const;
    InterfaceParameter param(const QString &name) const;
    void setParams(const QVector<InterfaceParameter> &params);
    void addParam(const InterfaceParameter &param);

    IVFunctionType *function() const;

    QList<IVFunction *> functionsStack() const;

    IVInterface *cloneOf() const;
    bool isClone() const;
    bool isCloned() const;
    QVector<QPointer<IVInterface>> clones() const;

    static IVInterface *createIface(const CreationInfo &descr);

    QVariant originalAttributeValue(const QString &name) const;
    QVector<InterfaceParameter> originalParams() const;

    void setCloneOrigin(IVInterface *source);

    IVInterface::OperationKind defaultKind() const;

    virtual QString ifaceLabel() const;

Q_SIGNALS:
    void paramsChanged();

protected Q_SLOTS:
    void onReflectedAttrChanged(const QString &attrName);
    void onReflectedParamsChanged();

protected:
    explicit IVInterface(IVObject::Type ifaceType, const CreationInfo &ci);

    void rememberClone(IVInterface *clone);
    void forgetClone(IVInterface *clone);

    void setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type) override;
    virtual void cloneInternals(const IVInterface *from);
    virtual void restoreInternals(const IVInterface *disconnectMe);

    void reflectAttrs(const IVInterface *from);
    void reflectParams(const IVInterface *from);

protected:
    struct OriginalPropsHolder {
        // TODO: unite with IVFunction::OriginalPropsHolder
        EntityAttributes attrs;
        QVector<InterfaceParameter> params;

        inline QString name() const
        {
            const QString attrName = meta::Props::token(meta::Props::Token::name);
            return attrs.value(attrName).value().value<QString>();
        }
        inline bool collected() const { return m_collected; }

        inline void collect(const IVInterface *src)
        {
            if (m_collected || !src)
                return;

            attrs = src->entityAttributes();
            params = src->params();

            m_collected = true;
        }

    private:
        bool m_collected { false };
    } m_originalFields;

private:
    const std::unique_ptr<IVInterfacePrivate> d;

    bool storedKindDiffers() const;
};

class IVInterfaceProvided : public IVInterface
{
    Q_OBJECT
public:
    explicit IVInterfaceProvided(const CreationInfo &ci);
};

class IVInterfaceRequired : public IVInterface
{
    Q_OBJECT
public:
    explicit IVInterfaceRequired(const CreationInfo &ci);

    bool isInheritPI() const;
    bool hasPrototypePi() const;

    QStringList inheritedLables() const;
    void setPrototype(const IVInterfaceProvided *pi);
    void unsetPrototype(const IVInterfaceProvided *pi);

    QString ifaceLabel() const override;
    QStringList ifaceLabelList() const;

Q_SIGNALS:
    void propChanged_InheritPI(bool inheritance);
    void inheritedLabelsChanged(const QStringList &labels);

protected:
    QVector<const IVInterfaceProvided *> m_prototypes;

    void cloneInternals(const IVInterface *from) override;
    void restoreInternals(const IVInterface *disconnectMe) override;
    void setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type) override;

private:
    QStringList collectInheritedLabels() const;
    void namesForRIToPIs(QStringList &result) const;
    void namesForRIsToPI(QStringList &result) const;
};

}

Q_DECLARE_METATYPE(ivm::IVInterface::InterfaceType);
Q_DECLARE_METATYPE(ivm::IVInterface::CreationInfo);
