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
#include <QSet>

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
    : AADLObject(id.isNull() ? common::createId() : id, title, parent)
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
    if (isProvided()) {
        setAttr(meta::Props::token(meta::Props::Token::kind), QVariant());
        setAttr(meta::Props::token(meta::Props::Token::period), QVariant());
        setAttr(meta::Props::token(meta::Props::Token::wcet), QVariant());
        setAttr(meta::Props::token(meta::Props::Token::queue_size), QVariant());
    } else {
        setAttr(meta::Props::token(meta::Props::Token::kind), QVariant());
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
            result.insert(k, QString(me.key(i)).toUpper() + QLatin1Literal("_OPERATION"));
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

void AADLObjectIface::setParams(const QVector<IfaceParameter> &params)
{
    if (d->m_params != params)
        d->m_params = params;
}

void AADLObjectIface::addParam(const IfaceParameter &param)
{
    if (!d->m_params.contains(param))
        d->m_params.append(param);
}

AADLObjectFunction *AADLObjectIface::function() const
{
    return qobject_cast<AADLObjectFunction *>(parentObject());
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
        d->m_cloneOf->rememberClone(this);
    }
}

void AADLObjectIface::rememberClone(AADLObjectIface *clone)
{
    if (clone && !d->m_clones.contains(clone))
        d->m_clones.append(clone);
}

void AADLObjectIface::forgetClone(AADLObjectIface *clone)
{
    d->m_clones.removeAll(clone);
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
                                              AADLObject *parent)
{
    if (direction == AADLObjectIface::IfaceType::Provided)
        return new AADLObjectIfaceProvided(id, QObject::tr("PI_%1").arg(++sProvidedCounter), parent);

    return new AADLObjectIfaceRequired(id, QObject::tr("RI_%1").arg(++sRequiredCounter), parent);
}

AADLObjectIface *AADLObjectIface::cloneIface(AADLObjectIface *source, AADLObjectFunction *parent)
{
    if (!source)
        return nullptr;

    AADLObjectIface *target = AADLObjectIface::createIface(source->direction(), common::createId(), parent);
    target->setCloneOrigin(source);

    auto cloneInternals = [target](void (AADLObject::*fp)(const QString &, const QVariant &),
                                   const QHash<QString, QVariant> &collection) {
        QHash<QString, QVariant>::const_iterator i = collection.cbegin();
        while (i != collection.cend()) {
            (target->*fp)(i.key(), i.value());
            ++i;
        }
    };

    cloneInternals(&AADLObject::setAttr, source->attrs());
    cloneInternals(&AADLObject::setProp, source->props());

    if (parent)
        parent->addInterface(target);
    if (AADLObjectsModel *model = parent->objectsModel())
        model->addObject(target);

    return target;
}

void AADLObjectIfaceRequired::setProp(const QString &name, const QVariant &val)
{
    AADLObject::setProp(name, val);

    if (!name.isEmpty()) {
        const meta::Props::Token t = meta::Props::token(name);
        switch (t) {
        case meta::Props::Token::labelInheritance: {
            const bool inherited = val.toBool();
            if (!inherited)
                m_inheritedLables.clear();
            emit propChanged_labelInheritance(inherited);
            emit inheritedLabelsChanged(inheritedLables());
            break;
        }
        default:
            break;
        }
    }
}

QStringList AADLObjectIfaceRequired::inheritedLables() const
{
    QStringList result;
    const QStringList &titles = m_inheritedLables.values();

    QHash<const AADLObjectIfaceProvided *, QString>::const_iterator i = m_inheritedLables.cbegin();
    while (i != m_inheritedLables.cend()) {
        const AADLObjectIfaceProvided *pi = i.key();
        QString label = pi->title();
        if (titles.count(label) > 1) {
            Q_ASSERT(pi->parentObject());
            label = pi->parentObject()->title() + "." + label;
        }

        result.append(label);

        ++i;
    }
    return result;
}

void AADLObjectIfaceRequired::updateInheritedLabel(const AADLObjectIfaceProvided *pi, const QString &label)
{
    if (!pi)
        return;

    if (label.isEmpty())
        m_inheritedLables.take(pi);
    else
        m_inheritedLables[pi] = label;
    emit inheritedLabelsChanged(inheritedLables());
}

bool AADLObjectIfaceRequired::labelInherited() const
{
    return prop(meta::Props::token(meta::Props::Token::labelInheritance)).toBool();
}

} // ns aadl
} // ns taste3
