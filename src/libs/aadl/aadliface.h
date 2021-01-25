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

class AADLFunction;
class AADLFunctionType;
struct AADLIfacePrivate;

class AADLIface : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(AADLIface::IfaceType direction READ direction CONSTANT)

public:
    enum class IfaceType
    {
        Required,
        Provided,
        Grouped
    };
    Q_ENUM(IfaceType)
    static constexpr IfaceType DefaultDirection { IfaceType::Required };

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
        CreationInfo(AADLModel *model = nullptr, AADLFunctionType *function = nullptr,
                const QPointF &position = QPointF(), AADLIface::IfaceType type = DefaultDirection,
                const shared::Id &id = shared::createId(),
                const QVector<IfaceParameter> &parameters = QVector<IfaceParameter>(),
                OperationKind kind = OperationKind::Sporadic, const QString &name = QString(),
                const CreationInfo::Policy policy = CreationInfo::Policy::Init, AADLIface *toBeCloned = nullptr);
        AADLModel *model { nullptr };
        AADLFunctionType *function { nullptr };
        QPointF position = {};
        AADLIface::IfaceType type { DefaultDirection };
        shared::Id id = {};
        QVector<IfaceParameter> parameters = {};
        OperationKind kind = { OperationKind::Sporadic };
        QString name {};
        CreationInfo::Policy policy { CreationInfo::Policy::Init };
        AADLIface *toBeCloned { nullptr };

        QVariantList toVarList() const;
        static CreationInfo initFromIface(AADLIface *iface, const CreationInfo::Policy policy);
        static CreationInfo fromIface(AADLIface *iface);
        static CreationInfo cloneIface(AADLIface *iface, AADLFunction *fn);

        void resetKind();
    };

    QMap<AADLIface::OperationKind, QString> availableKindNames() const;
    static QString kindToString(AADLIface::OperationKind k);
    static AADLIface::OperationKind kindFromString(const QString &k, AADLIface::OperationKind defaultKind);
    AADLIface::OperationKind kindFromString(const QString &k) const;

    ~AADLIface() override;

    AADLIface::IfaceType direction() const;

    bool isProvided() const;
    bool isRequired() const;

    OperationKind kind() const;
    bool setKind(OperationKind k);

    QVector<IfaceParameter> params() const;
    IfaceParameter param(const QString &name) const;
    void setParams(const QVector<IfaceParameter> &params);
    void addParam(const IfaceParameter &param);

    AADLFunctionType *function() const;

    QList<AADLFunction *> functionsStack() const;

    AADLIface *cloneOf() const;
    bool isClone() const;
    bool isCloned() const;
    QVector<QPointer<AADLIface>> clones() const;

    static AADLIface *createIface(const CreationInfo &descr);

    QVariant originalAttr(const QString &name) const;
    QVariant originalProp(const QString &name) const;
    QVector<IfaceParameter> originalParams() const;

    void setCloneOrigin(AADLIface *source);

    AADLIface::OperationKind defaultKind() const;

    virtual QString ifaceLabel() const;

Q_SIGNALS:
    void paramsChanged();

protected Q_SLOTS:
    void onReflectedAttrChanged(ivm::meta::Props::Token attr);
    void onReflectedPropChanged(ivm::meta::Props::Token prop);
    void onReflectedParamsChanged();

protected:
    explicit AADLIface(AADLObject::Type ifaceType, const CreationInfo &ci);
    void setupInitialAttrs();

    void rememberClone(AADLIface *clone);
    void forgetClone(AADLIface *clone);

    virtual void cloneInternals(const AADLIface *from);
    virtual void restoreInternals(const AADLIface *disconnectMe);

    void reflectAttrs(const AADLIface *from);
    void reflectProps(const AADLIface *from);
    void reflectParams(const AADLIface *from);

protected:
    struct OriginalPropsHolder {
        // TODO: unite with AADLFunction::OriginalPropsHolder

        QString name;
        QHash<QString, QVariant> attrs;
        QHash<QString, QVariant> props;
        QVector<IfaceParameter> params;

        inline bool collected() const { return m_collected; }

        inline void collect(const AADLIface *src)
        {
            if (m_collected || !src)
                return;

            name = src->title();
            attrs = src->attrs();
            props = src->props();
            params = src->params();

            m_collected = true;
        }

    private:
        bool m_collected { false };
    } m_originalFields;

private:
    const std::unique_ptr<AADLIfacePrivate> d;

    bool storedKindDiffers() const;
};

class AADLIfaceProvided : public AADLIface
{
    Q_OBJECT
public:
    explicit AADLIfaceProvided(const CreationInfo &ci);
};

class AADLIfaceRequired : public AADLIface
{
    Q_OBJECT
public:
    explicit AADLIfaceRequired(const CreationInfo &ci);

    virtual void setAttr(const QString &name, const QVariant &val) override;
    virtual void setProp(const QString &name, const QVariant &val) override;

    bool isInheritPI() const;
    bool hasPrototypePi() const;

    QStringList inheritedLables() const;
    void setPrototype(const AADLIfaceProvided *pi);
    void unsetPrototype(const AADLIfaceProvided *pi);

    QString ifaceLabel() const override;
    QStringList ifaceLabelList() const;

Q_SIGNALS:
    void propChanged_InheritPI(bool inheritance);
    void inheritedLabelsChanged(const QStringList &labels);

protected:
    QVector<const AADLIfaceProvided *> m_prototypes;

    void cloneInternals(const AADLIface *from) override;
    void restoreInternals(const AADLIface *disconnectMe) override;

private:
    QStringList collectInheritedLabels() const;
    void namesForRIToPIs(QStringList &result) const;
    void namesForRIsToPI(QStringList &result) const;
};

typedef QVector<AADLIface *> AADLIfacesVector;

}

Q_DECLARE_METATYPE(ivm::AADLIface::IfaceType);
Q_DECLARE_METATYPE(ivm::AADLIface::CreationInfo);
