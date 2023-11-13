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

#include "archetypeobject.h"

#include "archetypemodel.h"
#include "exportableattribute.h"
#include "ivcoreutils.h"
#include "ivnamevalidator.h"

#include <QPointer>
#include <QVector>
#include <QtDebug>

namespace ivm {

ArchetypeObject::ArchetypeObject(
        const QString &title, const ivm::ArchetypeObject::Type type, QObject *parent, const shared::Id &id)
    : shared::VEObject(id, parent)
    , m_type(type)
{
    if (const ArchetypeObject *parentObject = qobject_cast<const ArchetypeObject *>(parent)) {
        setModel(parentObject->model());
    } else if (ArchetypeModel *model = qobject_cast<ArchetypeModel *>(parent)) {
        setModel(model);
    }

    setTitle(title);
}

ArchetypeObject::~ArchetypeObject() = default;

QString ArchetypeObject::title() const
{
    return entityAttributeValue<QString>(meta::Props::token(meta::Props::Token::name));
}

QString ArchetypeObject::titleUI() const
{
    QString text { title() };
    std::for_each(text.begin(), text.end(), [](QChar &ch) {
        if (!ch.isLetterOrNumber())
            ch = QLatin1Char(' ');
    });

    return text;
}

bool ArchetypeObject::postInit()
{
    if (title().isEmpty()) {
        resetTitle();
    }
    return true;
}

bool ArchetypeObject::aboutToBeRemoved()
{
    return true;
}

void ArchetypeObject::resetTitle()
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::name), {});
}

QString ArchetypeObject::typeToString(Type t)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Type>();
    return metaEnum.valueToKey(int(t));
}

QStringList ArchetypeObject::requestsIDs() const
{
    QString value = entityAttributeValue<QString>(meta::Props::token(meta::Props::Token::requests_ids));
    return value.isEmpty() ? QStringList() : value.split(",");
}

void ArchetypeObject::setRequestsIDs(const QStringList &requestIDs)
{
    if (requestIDs.isEmpty()) {
        removeEntityAttribute(meta::Props::token(meta::Props::Token::requests_ids));
    } else {
        setEntityAttribute(meta::Props::token(meta::Props::Token::requests_ids), requestIDs.join(","));
    }
}

ArchetypeObject::Type ArchetypeObject::type() const
{
    return m_type;
}

bool ArchetypeObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::name), title);
        return true;
    }
    return false;
}

QVector<qint32> ArchetypeObject::coordinates() const
{
    return QVector<qint32>();
}

void ArchetypeObject::setCoordinates(const QVector<qint32> &coordinates) { }

ArchetypeObject *ArchetypeObject::parentObject() const
{
    return qobject_cast<ArchetypeObject *>(parent());
}

bool ArchetypeObject::isParameterArchetype() const
{
    return type() == Type::ParameterArchetype;
}

bool ArchetypeObject::isInterfaceArchetype() const
{
    return type() == Type::InterfaceArchetype;
}

bool ArchetypeObject::isFunctionArchetype() const
{
    return type() == Type::FunctionArchetype;
}

ArchetypeModel *ArchetypeObject::model() const
{
    return qobject_cast<ArchetypeModel *>(shared::VEObject::model());
}

bool ArchetypeObject::isRootObject() const
{
    return model() ? model()->rootObject() == this : false;
}

QVariantList ArchetypeObject::attributes() const
{
    return generateProperties(false);
}

QVariantList ArchetypeObject::properties() const
{
    return generateProperties(true);
}

QVariantList ArchetypeObject::generateProperties(bool isProperty) const
{
    QVariantList result;
    EntityAttributes attributes = entityAttributes();
    for (auto it = attributes.cbegin(); it != attributes.cend(); ++it) {
        if (it.value().isExportable() && it.value().isProperty() == isProperty) {
            result << QVariant::fromValue(shared::ExportableAttribute(it.key(), it.value().value()));
        }
    }

    std::sort(result.begin(), result.end(), [](const QVariant &leftVal, const QVariant &rightVal) {
        const auto &rightExportable = rightVal.value<shared::ExportableAttribute>();
        const ivm::meta::ArchetypeProps::Token rightToken = ivm::meta::ArchetypeProps::token(rightExportable.name());
        if (rightToken == ivm::meta::ArchetypeProps::Token::Unknown)
            return true;

        const auto &leftExportable = leftVal.value<shared::ExportableAttribute>();
        const ivm::meta::ArchetypeProps::Token leftToken = ivm::meta::ArchetypeProps::token(leftExportable.name());
        if (leftToken == ivm::meta::ArchetypeProps::Token::Unknown)
            return false;

        return leftToken < rightToken;
    });

    return result;
}

QList<EntityAttribute> ArchetypeObject::sortedAttributesValues(const EntityAttributes &attributes)
{
    QList<EntityAttribute> attrs = attributes.values();
    std::sort(attrs.begin(), attrs.end(), [](const EntityAttribute &a1, const EntityAttribute &a2) {
        return meta::Props::token(a1.name()) > meta::Props::token(a2.name());
    });
    return attrs;
}

}
