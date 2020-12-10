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

namespace aadl {

class AADLObjectFunction;
class AADLObjectFunctionType;
struct AADLObjectIfacePrivate;

class AADLObjectIface : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(AADLObjectIface::IfaceType direction READ direction CONSTANT)

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
        CreationInfo(AADLObjectsModel *model = nullptr, AADLObjectFunctionType *function = nullptr,
                const QPointF &position = QPointF(), AADLObjectIface::IfaceType type = DefaultDirection,
                const shared::Id &id = shared::createId(),
                const QVector<IfaceParameter> &parameters = QVector<IfaceParameter>(),
                OperationKind kind = OperationKind::Sporadic, const QString &name = QString(),
                const CreationInfo::Policy policy = CreationInfo::Policy::Init, AADLObjectIface *toBeCloned = nullptr);
        AADLObjectsModel *model { nullptr };
        AADLObjectFunctionType *function { nullptr };
        QPointF position = {};
        AADLObjectIface::IfaceType type { DefaultDirection };
        shared::Id id = {};
        QVector<IfaceParameter> parameters = {};
        OperationKind kind = { OperationKind::Sporadic };
        QString name {};
        CreationInfo::Policy policy { CreationInfo::Policy::Init };
        AADLObjectIface *toBeCloned { nullptr };

        QVariantList toVarList() const;
        static CreationInfo initFromIface(AADLObjectIface *iface, const CreationInfo::Policy policy);
        static CreationInfo fromIface(AADLObjectIface *iface);
        static CreationInfo cloneIface(AADLObjectIface *iface, AADLObjectFunction *fn);

        void resetKind();
    };

    QMap<AADLObjectIface::OperationKind, QString> availableKindNames() const;
    static QString kindToString(AADLObjectIface::OperationKind k);
    static AADLObjectIface::OperationKind kindFromString(const QString &k, AADLObjectIface::OperationKind defaultKind);
    AADLObjectIface::OperationKind kindFromString(const QString &k) const;

    ~AADLObjectIface() override;

    AADLObjectIface::IfaceType direction() const;

    bool isProvided() const;
    bool isRequired() const;

    OperationKind kind() const;
    bool setKind(OperationKind k);

    QVector<IfaceParameter> params() const;
    IfaceParameter param(const QString &name) const;
    void setParams(const QVector<IfaceParameter> &params);
    void addParam(const IfaceParameter &param);

    AADLObjectFunctionType *function() const;

    QList<AADLObjectFunction *> functionsStack() const;

    AADLObjectIface *cloneOf() const;
    bool isClone() const;
    bool isCloned() const;
    QVector<QPointer<AADLObjectIface>> clones() const;

    static AADLObjectIface *createIface(const CreationInfo &descr);

    QVariant originalAttr(const QString &name) const;
    QVariant originalProp(const QString &name) const;
    QVector<IfaceParameter> originalParams() const;

    void setCloneOrigin(AADLObjectIface *source);

    AADLObjectIface::OperationKind defaultKind() const;

    virtual QString ifaceLabel() const;

Q_SIGNALS:
    void paramsChanged();

protected Q_SLOTS:
    void onReflectedAttrChanged(aadl::meta::Props::Token attr);
    void onReflectedPropChanged(aadl::meta::Props::Token prop);
    void onReflectedParamsChanged();

protected:
    explicit AADLObjectIface(AADLObject::Type ifaceType, const CreationInfo &ci);
    void setupInitialAttrs();

    void rememberClone(AADLObjectIface *clone);
    void forgetClone(AADLObjectIface *clone);

    virtual void cloneInternals(const AADLObjectIface *from);
    virtual void restoreInternals(const AADLObjectIface *disconnectMe);

    void reflectAttrs(const AADLObjectIface *from);
    void reflectProps(const AADLObjectIface *from);
    void reflectParams(const AADLObjectIface *from);

protected:
    struct OriginalPropsHolder {
        // TODO: unite with AADLObjectFunction::OriginalPropsHolder

        QString name;
        QHash<QString, QVariant> attrs;
        QHash<QString, QVariant> props;
        QVector<IfaceParameter> params;

        inline bool collected() const { return m_collected; }

        inline void collect(const AADLObjectIface *src)
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
    const std::unique_ptr<AADLObjectIfacePrivate> d;

    bool storedKindDiffers() const;
};

class AADLObjectIfaceProvided : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceProvided(const CreationInfo &ci);
};

class AADLObjectIfaceRequired : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceRequired(const CreationInfo &ci);

    virtual void setAttr(const QString &name, const QVariant &val) override;
    virtual void setProp(const QString &name, const QVariant &val) override;

    bool isInheritPI() const;
    bool hasPrototypePi() const;

    QStringList inheritedLables() const;
    void setPrototype(const AADLObjectIfaceProvided *pi);
    void unsetPrototype(const AADLObjectIfaceProvided *pi);

    QString ifaceLabel() const override;
    QStringList ifaceLabelList() const;

Q_SIGNALS:
    void propChanged_InheritPI(bool inheritance);
    void inheritedLabelsChanged(const QStringList &labels);

protected:
    QVector<const AADLObjectIfaceProvided *> m_prototypes;

    void cloneInternals(const AADLObjectIface *from) override;
    void restoreInternals(const AADLObjectIface *disconnectMe) override;

private:
    QStringList collectInheritedLabels() const;
    void namesForRIToPIs(QStringList &result) const;
    void namesForRIsToPI(QStringList &result) const;
};

typedef QVector<AADLObjectIface *> AADLIfacesVector;

}

Q_DECLARE_METATYPE(aadl::AADLObjectIface::IfaceType);
Q_DECLARE_METATYPE(aadl::AADLObjectIface::CreationInfo);
