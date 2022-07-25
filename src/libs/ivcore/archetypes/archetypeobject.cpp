/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "archetypeobject.h"

#include "archetypemodel.h"
#include "exportableproperty.h"
#include "ivcoreutils.h"
#include "ivnamevalidator.h"

#include <QPointer>
#include <QVector>
#include <QtDebug>

namespace ivm {

/*!
 * \namespace ivm
 * \brief The Archetype object code
 */

struct ArchetypeObjectPrivate {
    ArchetypeObjectPrivate(const ArchetypeObject::Type type)
        : m_type(type)
    {
    }

    ArchetypeObject::Type m_type;
};

ArchetypeObject::ArchetypeObject(
        const QString &title, const ivm::ArchetypeObject::Type type, QObject *parent, const shared::Id &id)
    : shared::VEObject(id, parent)
    , m_privateObject(std::make_unique<ArchetypeObjectPrivate>(type))
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
    QString text;
    std::transform(title().cbegin(), title().cend(), std::back_inserter(text),
            [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char(' '); });

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

/**
 * @brief Returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList ArchetypeObject::attributes() const
{
    return generateProperties(false);
}

/**
 * @brief generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of ArchetypeObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList ArchetypeObject::generateProperties(bool isProperty) const
{
    QVariantList result;
    EntityAttributes attributes = entityAttributes();
    for (auto it = attributes.cbegin(); it != attributes.cend(); ++it) {
        if (it.value().isExportable() && it.value().isProperty() == isProperty) {
            result << QVariant::fromValue(shared::ExportableProperty(it.key(), it.value().value()));
        }
    }

    std::sort(result.begin(), result.end(), [](const QVariant &left_val, const QVariant &right_val) {
        const auto &r = right_val.value<shared::ExportableProperty>();
        const ivm::meta::Props::Token right_token = ivm::meta::Props::token(r.name());
        if (right_token == ivm::meta::Props::Token::Unknown)
            return true;

        const auto &l = left_val.value<shared::ExportableProperty>();
        const ivm::meta::Props::Token left_token = ivm::meta::Props::token(l.name());
        if (left_token == ivm::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

ArchetypeObject::Type ArchetypeObject::type() const
{
    return m_privateObject->m_type;
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

void ArchetypeObject::setCoordinates(const QVector<qint32> &coordinates) {}

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

void ArchetypeObject::setVisible(bool isVisible)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::is_visible), isVisible);
}

/*!
   Returns false, if the "is_visible" is set to false.
   Returns true, if set to true or is not set at all. Also does not take the parent's visibility into account.
 */
bool ArchetypeObject::isVisible() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::is_visible), true);
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
