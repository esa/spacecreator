/*
    Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "exportableattribute.h"

#include <QObject>
#include <QVariantHash>

class EntityAttribute;

namespace shared {
class VEObject;
class PropertyTemplateConfig;
/**
 * @brief ExportableProperty is a helper class for converting
 * attributes into the tree of properties according to their namespaces
 */
class ExportableProperty
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QList<shared::ExportableAttribute> attributes READ attributes)
    Q_PROPERTY(QList<ExportableProperty> children READ children)

public:
    ExportableProperty();

    /**
     * @brief Collect all attributes from `object` matching `groupName`
     * stored in `propertyConfig`
     */
    static QList<ExportableProperty> createPropertiesFromObject(
            PropertyTemplateConfig *propertyConfig, const VEObject *object, const QString &groupName);

    /**
     * @brief Update helper `property` with the data from EntityAttribute `attr`
     */
    static void updatePropertyFromAttribute(ExportableProperty &property, const EntityAttribute &attr);

    QString name() const;
    void setName(const QString &name);

    QVariant value() const;
    void setValue(const QVariant &value);

    QList<ExportableAttribute> attributes() const;
    void addAttribute(const ExportableAttribute &attr);
    void addAttribute(const QString &attrName, const QVariant &attrValue);

    QList<ExportableProperty> children() const;
    ExportableProperty &child(const QString &name);

private:
    QString m_name;
    QVariant m_value;
    QList<shared::ExportableAttribute> m_attrs;
    QList<ExportableProperty> m_children;
};

} // namespace shared

Q_DECLARE_METATYPE(shared::ExportableProperty)
Q_DECLARE_METATYPE(QList<shared::ExportableProperty>)
