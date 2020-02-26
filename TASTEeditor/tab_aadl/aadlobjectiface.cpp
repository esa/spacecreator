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

#include "aadlobjectiface.h"

#include "aadlcommonprops.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"

#include <QMetaEnum>

namespace taste3 {
namespace aadl {

static int sProvidedCounter = 0;
static int sRequiredCounter = 0;

struct AADLObjectIfacePrivate {
    explicit AADLObjectIfacePrivate(AADLObjectIface::IfaceType dir)
        : m_direction(dir)
    {
    }
    const AADLObjectIface::IfaceType m_direction;
    QVector<IfaceParameter> m_params = {};
    QPointer<AADLObjectIface> m_cloneOf { nullptr };
    QVector<QPointer<AADLObjectIface>> m_clones {};
};

AADLObjectIface::AADLObjectIface(AADLObjectIface::IfaceType direction, const QString &title, AADLObject *parent)
    : AADLObject(title, parent)
    , d(new AADLObjectIfacePrivate(direction))
{
    setupInitialAttrs();
}

AADLObjectIface::AADLObjectIface(const common::Id &id, AADLObjectIface::IfaceType direction, const QString &title,
                                 AADLObject *parent)
    : AADLObject(id.isNull() ? common::Id::create() : id, title, parent)
    , d(new AADLObjectIfacePrivate(direction))
{
    setupInitialAttrs();
}

AADLObjectIface::~AADLObjectIface()
{
    if (d->m_cloneOf)
        d->m_cloneOf->forgetClone(this);
}

void AADLObjectIface::setupInitialAttrs()
{
    setAttr(meta::Props::token(meta::Props::Token::kind), kindToString(AADLObjectIface::OperationKind::Sporadic));
    if (isProvided()) {
        setAttr(meta::Props::token(meta::Props::Token::period), QVariant());
        setAttr(meta::Props::token(meta::Props::Token::wcet), QVariant());
        setAttr(meta::Props::token(meta::Props::Token::queue_size), QVariant());
    } else {
        setProp(meta::Props::token(meta::Props::Token::labelInheritance), true);
    }
}

AADLObject::AADLObjectType AADLObjectIface::aadlType() const
{
    return AADLObjectType::AADLIface;
}

AADLObjectIface::IfaceType AADLObjectIface::direction() const
{
    return d->m_direction;
}

bool AADLObjectIface::isProvided() const
{
    return direction() == IfaceType::Provided;
}
bool AADLObjectIface::isRequired() const
{
    return direction() == IfaceType::Required;
}

QMap<AADLObjectIface::OperationKind, QString> AADLObjectIface::xmlKindNames()
{
    static QMap<AADLObjectIface::OperationKind, QString> result;
    if (result.isEmpty()) {
        const QMetaEnum &me = QMetaEnum::fromType<taste3::aadl::AADLObjectIface::OperationKind>();
        for (int i = 0; i < me.keyCount(); ++i) {
            const AADLObjectIface::OperationKind k = static_cast<AADLObjectIface::OperationKind>(me.value(i));
            result.insert(k, QString(me.key(i)).toUpper() + QLatin1String("_OPERATION"));
        }
    }
    return result;
}

QString AADLObjectIface::kindToString(AADLObjectIface::OperationKind k)
{
    static const QMap<AADLObjectIface::OperationKind, QString> &kindNamesXml = xmlKindNames();
    return kindNamesXml.contains(k) ? kindNamesXml.value(k) : QString();
}

AADLObjectIface::OperationKind AADLObjectIface::kindFromString(const QString &k)
{
    static const QMap<AADLObjectIface::OperationKind, QString> &kindNamesXml = AADLObjectIface::xmlKindNames();
    static const QStringList &names = kindNamesXml.values();

    return names.contains(k) ? kindNamesXml.key(k) : AADLObjectIface::OperationKind::Any;
}

AADLObjectIface::OperationKind AADLObjectIface::kind() const
{
    return kindFromString(attr(meta::Props::token(meta::Props::Token::kind)).toString());
}

bool AADLObjectIface::setKind(AADLObjectIface::OperationKind k)
{
    if (this->kind() != k) {
        setAttr(meta::Props::token(meta::Props::Token::kind), kindToString(k));
        return true;
    }

    return false;
}

QVector<IfaceParameter> AADLObjectIface::params() const
{
    return d->m_params;
}

QVariantList AADLObjectIface::paramList() const
{
    QVariantList list;
    for (const auto &param : d->m_params)
        list << QVariant::fromValue(param);
    return list;
}

void AADLObjectIface::setParams(const QVector<IfaceParameter> &params)
{
    if (d->m_params != params) {
        d->m_params = params;
        emit paramsChanged();
    }
}

void AADLObjectIface::addParam(const IfaceParameter &param)
{
    if (!d->m_params.contains(param)) {
        d->m_params.append(param);
        emit paramsChanged();
    }
}

AADLObjectFunction *AADLObjectIface::function() const
{
    return qobject_cast<AADLObjectFunction *>(parentObject());
}

bool AADLObjectIface::isClone() const
{
    return d->m_cloneOf;
}

bool AADLObjectIface::isCloned() const
{
    return d->m_clones.size();
}

QVector<QPointer<AADLObjectIface>> AADLObjectIface::clones() const
{
    return d->m_clones;
}

void AADLObjectIface::setCloneOrigin(AADLObjectIface *source)
{
    if (d->m_cloneOf != source) {
        d->m_cloneOf = source;
        if (d->m_cloneOf) {
            d->m_cloneOf->rememberClone(this);
        }
    }
}

void AADLObjectIface::rememberClone(AADLObjectIface *clone)
{
    if (clone && !d->m_clones.contains(clone)) {
        clone->cloneInternals(this);
        d->m_clones.append(clone);
    }
}

void AADLObjectIface::forgetClone(AADLObjectIface *clone)
{
    if (clone)
        clone->restoreInternals(this);
    d->m_clones.removeAll(clone);
}

void AADLObjectIface::setAttr(const QString &name, const QVariant &val)
{
    switch (meta::Props::token(name)) {
    case meta::Props::Token::kind: {
        const AADLObjectIface::OperationKind k = kindFromString(val.toString());
        if (k != kind()) {
            AADLObject::setAttr(name, val);
            emit attrChanged_kind(k);
        }
        break;
    }
    default:
        AADLObject::setAttr(name, val);
        break;
    }
}

AADLObjectIfaceProvided::AADLObjectIfaceProvided(AADLObject *parent)
    : AADLObjectIface(IfaceType::Provided, tr("PI_%1").arg(++sProvidedCounter), parent)
{
}

AADLObjectIfaceProvided::AADLObjectIfaceProvided(const QString &title, AADLObject *parent)
    : AADLObjectIface(IfaceType::Provided, title, parent)
{
}

AADLObjectIfaceProvided::AADLObjectIfaceProvided(const common::Id &id, const QString &title, AADLObject *parent)
    : AADLObjectIface(id, IfaceType::Provided, title, parent)
{
}

AADLObjectIfaceRequired::AADLObjectIfaceRequired(AADLObject *parent)
    : AADLObjectIface(IfaceType::Required, tr("RI_%1").arg(++sRequiredCounter), parent)
{
}

AADLObjectIfaceRequired::AADLObjectIfaceRequired(const QString &title, AADLObject *parent)
    : AADLObjectIface(IfaceType::Required, title, parent)
{
}

AADLObjectIfaceRequired::AADLObjectIfaceRequired(const common::Id &id, const QString &title, AADLObject *parent)
    : AADLObjectIface(id, IfaceType::Required, title, parent)
{
}

AADLObjectIface *AADLObjectIface::createIface(AADLObjectIface::IfaceType direction, const common::Id &id,
                                              AADLObject *parent, const QVector<IfaceParameter> &params)
{
    AADLObjectIface *iface { nullptr };
    if (direction == AADLObjectIface::IfaceType::Provided)
        iface = new AADLObjectIfaceProvided(id, QObject::tr("PI_%1").arg(++sProvidedCounter), parent);
    else
        iface = new AADLObjectIfaceRequired(id, QObject::tr("RI_%1").arg(++sRequiredCounter), parent);

    iface->setParams(params);

    return iface;
}

void AADLObjectIface::cloneInternals(const AADLObjectIface *from)
{
    if (!from)
        return;

    if (!m_originalFields.collected())
        m_originalFields.collect(isClone() ? d->m_cloneOf.data() : this);

    reflectAttrs(from);
    reflectProps(from);
    reflectParams(from);

    connect(from, &AADLObjectIface::attributeChanged, this, &AADLObjectIface::onReflectedAttrChanged,
            Qt::UniqueConnection);
    connect(from, &AADLObjectIface::propertyChanged, this, &AADLObjectIface::onReflectedPropChanged,
            Qt::UniqueConnection);
    connect(from, &AADLObjectIface::paramsChanged, this, &AADLObjectIface::onReflectedParamsChanged,
            Qt::UniqueConnection);
}

void AADLObjectIface::restoreInternals(const AADLObjectIface *disconnectMe)
{
    disconnect(disconnectMe, &AADLObjectIface::attributeChanged, this, &AADLObjectIface::onReflectedAttrChanged);
    disconnect(disconnectMe, &AADLObjectIface::propertyChanged, this, &AADLObjectIface::onReflectedPropChanged);
    disconnect(disconnectMe, &AADLObjectIface::paramsChanged, this, &AADLObjectIface::onReflectedParamsChanged);

    setAttrs(m_originalFields.attrs);

    // keep current coordinates:
    for (meta::Props::Token t : { meta::Props::Token::InnerCoordinates, meta::Props::Token::coordinates }) {
        const QString &name = meta::Props::token(t);
        if (m_originalFields.props.contains(name))
            m_originalFields.props[name] = prop(name);
    }
    setProps(m_originalFields.props);

    setParams(m_originalFields.params);
}

QVariant AADLObjectIface::originalAttr(const QString &name) const
{
    return m_originalFields.attrs.value(name, QVariant());
}

QVariant AADLObjectIface::originalProp(const QString &name) const
{
    return m_originalFields.props.value(name, QVariant());
}

QVector<IfaceParameter> AADLObjectIface::originalParams() const
{
    return m_originalFields.params;
}

AADLObjectIface *AADLObjectIface::cloneIface(AADLObjectIface *source, AADLObjectFunction *parent)
{
    if (!source)
        return nullptr;

    AADLObjectIface *target = AADLObjectIface::createIface(source->direction(), common::Id::create(), parent);

    target->setCloneOrigin(source);

    if (parent)
        parent->addInterface(target);
    if (AADLObjectsModel *model = parent->objectsModel())
        model->addObject(target);

    return target;
}

void AADLObjectIface::onReflectedAttrChanged(taste3::aadl::meta::Props::Token /*attr*/)
{
    if (AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(sender()))
        reflectAttrs(iface);
}

void AADLObjectIface::onReflectedPropChanged(taste3::aadl::meta::Props::Token /*prop*/)
{
    if (AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(sender()))
        reflectProps(iface);
}

void AADLObjectIface::onReflectedParamsChanged()
{
    if (AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(sender()))
        reflectParams(iface);
}

void AADLObjectIface::reflectAttrs(const AADLObjectIface *from)
{
    if (!from)
        return;

    auto revertAttribute = [](const QString &name, QHash<QString, QVariant> &attrs,
                              const QHash<QString, QVariant> &attrsOriginal) {
        const QVariant &value = attrsOriginal.value(name);
        if (value.isValid())
            attrs[name] = value;
    };

    QHash<QString, QVariant> newAttrs = from->attrs();

    const bool isFunctionTypeInherited =
            from->parent() && from->parentObject()->as<const AADLObjectFunctionType *>()->isFunctionType();
    const bool keepName = !isFunctionTypeInherited && isRequired() && from->isProvided();
    if (keepName)
        revertAttribute(meta::Props::token(meta::Props::Token::name), newAttrs, m_originalFields.attrs);

    setAttrs(newAttrs);
}

void AADLObjectIface::reflectProps(const AADLObjectIface *from)
{
    if (!from)
        return;

    auto revertProperty = [](meta::Props::Token t, QHash<QString, QVariant> &props,
                             const QHash<QString, QVariant> &propsOriginal) {
        const QString &name = meta::Props::token(t);
        const QVariant &value = propsOriginal.value(name);
        if (value.isValid())
            props[name] = value;
    };

    const QHash<QString, QVariant> &currProps = props();
    QHash<QString, QVariant> newProps = from->props();
    const bool isFunctionTypeInherited =
            from->parent() && from->parentObject()->as<const AADLObjectFunctionType *>()->isFunctionType();

    for (auto t : { meta::Props::Token::labelInheritance, meta::Props::Token::coordinates,
                    meta::Props::Token::InnerCoordinates }) {
        const bool isLabelInheritanceFlag = t == meta::Props::Token::labelInheritance;
        const bool isInheritedPI = !isFunctionTypeInherited && isRequired() && from->isProvided();
        if (!isLabelInheritanceFlag || isInheritedPI)
            revertProperty(t, newProps, currProps);
    }

    setProps(newProps);
}

void AADLObjectIface::reflectParams(const AADLObjectIface *from)
{
    if (!from)
        return;

    setParams(from->params());
}

void AADLObjectIfaceRequired::setProp(const QString &name, const QVariant &val)
{
    if (!name.isEmpty()) {
        const meta::Props::Token t = meta::Props::token(name);
        switch (t) {
        case meta::Props::Token::labelInheritance: {
            const bool newVal = val.toBool();
            if (prop(meta::Props::token(meta::Props::Token::labelInheritance)) != newVal) {
                // should be handled in Connection _before_ the actual value change:
                emit inheritedLabelsChanged(inheritedLables());

                AADLObject::setProp(name, val);
                emit propChanged_labelInheritance(newVal);
            }
            break;
        }
        default:
            AADLObject::setProp(name, val);
            break;
        }
    }
}

QStringList AADLObjectIfaceRequired::inheritedLables() const
{
    QStringList result;
    if (inheritPi()) {
        result = collectInheritedLabels();

        // append suffix for connection to the same named PIs with same parent (Function.PI becames Funtcion.PI#N)
        namesForRIToPIs(result);

        // if 2+ FnA.RI connected to the same FnB.PI, populate the inherited name with number suffix
        // (based on the index of the Connection among related connections)
        namesForRIsToPI(result);
    }

    if (result.isEmpty())
        result.append(title());

    return result;
}

QStringList AADLObjectIfaceRequired::collectInheritedLabels() const
{
    QStringList result, titles;
    std::transform(m_prototypes.cbegin(), m_prototypes.cend(), std::back_inserter(titles),
                   [](const AADLObjectIfaceProvided *pi) { return pi->title(); });

    for (const AADLObjectIfaceProvided *pi : m_prototypes) {
        QString label = pi->title();
        if (titles.count(label) > 1) {
            Q_ASSERT(pi->parentObject());
            // if PIs have same name, prepend it with parent's name
            label = pi->parentObject()->title() + "." + label;
        }
        result.prepend(label);
    }
    return result;
}

void AADLObjectIfaceRequired::namesForRIToPIs(QStringList &result) const
{
    for (const QString label : result) {
        int count = result.count(label);
        if (count > 1)
            while (count >= 1) {
                const int pos = result.indexOf(label);
                if (pos != -1)
                    result.replace(pos, label + "#" + QString::number(count));
                count = result.count(label);
            }
    }
}

void AADLObjectIfaceRequired::namesForRIsToPI(QStringList &result) const
{
    AADLObjectFunction *parentFn = function();
    if (!parentFn)
        return;

    auto findRI = [](const AADLObjectConnection *in, const AADLObjectIfaceProvided *pi) -> AADLObjectIfaceRequired * {
        if (in && pi)
            for (AADLObjectIface *iface : { in->sourceInterface(), in->targetInterface() })
                if (iface && iface != pi && iface->isRequired())
                    return qobject_cast<AADLObjectIfaceRequired *>(iface);
        return nullptr;
    };

    const common::Id &parentId = parentFn->id();
    for (const AADLObjectIfaceProvided *pi : m_prototypes) {
        const QVector<AADLObjectConnection *> &relatedConnecions = objectsModel()->getConnectionsForIface(pi->id());
        QVector<AADLObjectConnection *>::const_reverse_iterator i = relatedConnecions.crbegin();
        while (i != relatedConnecions.crend()) {
            AADLObjectConnection *c = *i;
            const bool sameSrcFn = c->source() && c->source()->id() == parentId;
            const bool sameDstFn = c->target() && c->target()->id() == parentId;
            const bool sameFn = sameSrcFn || sameDstFn;
            const bool isMeSrc = sameFn && c->sourceInterface() == this;
            const bool isMeDst = sameFn && c->targetInterface() == this;
            const bool toSibling = !isMeSrc && !isMeDst;
            if (toSibling)
                if (AADLObjectIfaceRequired *otherRI = findRI(c, pi)) {
                    if (!otherRI->inheritPi())
                        continue;

                    const QString &oldLabel = pi->title();
                    const int labelPos = result.indexOf(oldLabel);
                    if (labelPos >= 0) {
                        result.replace(labelPos, oldLabel + "#" + QString::number(relatedConnecions.indexOf(c)));
                    }
                }
            ++i;
        }
    }
}

void AADLObjectIfaceRequired::setPrototype(const AADLObjectIfaceProvided *pi)
{
    if (!pi || !inheritPi())
        return;

    if (!m_prototypes.contains(pi)) {
        m_prototypes.append(pi);
    }

    if (!m_prototypes.isEmpty())
        cloneInternals(pi);

    emit inheritedLabelsChanged(inheritedLables());
}

void AADLObjectIfaceRequired::unsetPrototype(const AADLObjectIfaceProvided *pi)
{
    if (!pi)
        return;

    m_prototypes.removeAll(pi);
    if (m_prototypes.isEmpty())
        restoreInternals(pi);

    emit inheritedLabelsChanged(inheritedLables());
}

bool AADLObjectIfaceRequired::inheritPi() const
{
    return prop(meta::Props::token(meta::Props::Token::labelInheritance)).toBool();
}

bool AADLObjectIfaceRequired::hasPrototypePi() const
{
    return m_prototypes.size();
}

void AADLObjectIfaceRequired::cloneInternals(const AADLObjectIface *from)
{
    AADLObjectIface::cloneInternals(from);

    if (const AADLObjectIfaceRequired *ri = from->as<const AADLObjectIfaceRequired *>())
        connect(ri, &AADLObjectIfaceRequired::propChanged_labelInheritance, this,
                &AADLObjectIfaceRequired::propChanged_labelInheritance, Qt::UniqueConnection);
}

void AADLObjectIfaceRequired::restoreInternals(const AADLObjectIface *disconnectMe)
{
    AADLObjectIface::restoreInternals(disconnectMe);
    if (const AADLObjectIfaceRequired *ri = disconnectMe->as<const AADLObjectIfaceRequired *>())
        disconnect(ri, &AADLObjectIfaceRequired::propChanged_labelInheritance, this,
                   &AADLObjectIfaceRequired::propChanged_labelInheritance);
}

} // ns aadl
} // ns taste3
