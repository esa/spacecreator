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

#include "exportableproperty.h"

#include "common.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
#include "veobject.h"

namespace shared {

ExportableProperty::ExportableProperty() { }

static inline void processAttr(shared::ExportableProperty &uiNode, QStringView name, const QVariant &value)
{
    if (name.isEmpty())
        return;

    const int idx = name.indexOf(shared::kStringDelemiter);
    if (idx == -1) {
        uiNode.addAttribute(name.toString(), value);
    } else {
        processAttr(uiNode.child(name.sliced(0, idx).toString()), name.sliced(idx + shared::kStringDelemiter.size()),
                value);
    }
}

static inline void processObject(PropertyTemplateConfig *propertyConfig, const VEObject *object,
        const QString &groupName, QList<ExportableProperty> &data)
{
    if (!object)
        return;

    shared::ExportableProperty uiNode;
    uiNode.setName(QLatin1String("Entity"));
    uiNode.addAttribute(QLatin1String("id"), object->id());

    for (const EntityAttribute &attr : object->entityAttributes()) {
        shared::PropertyTemplate *pt = propertyConfig->propertyTemplateForObject(object, attr.name());
        if (pt && pt->exportGroupName() == groupName)
            processAttr(uiNode, attr.name(), attr.value());
    }
    data.append(uiNode);
    for (const auto child : object->children()) {
        if (const shared::VEObject *veChild = qobject_cast<const shared::VEObject *>(child)) {
            processObject(propertyConfig, veChild, groupName, data);
        }
    }
}

QList<ExportableProperty> ExportableProperty::createPropertiesFromObject(
        PropertyTemplateConfig *propertyConfig, const VEObject *object, const QString &groupName)
{
    QList<ExportableProperty> result;
    processObject(propertyConfig, object, groupName, result);
    return result;
}

void ExportableProperty::updatePropertyFromAttribute(ExportableProperty &property, const EntityAttribute &attr)
{
    processAttr(property, attr.name(), attr.value());
}

QString ExportableProperty::name() const
{
    return m_name;
}

void ExportableProperty::setName(const QString &name)
{
    m_name = name;
}

QVariant ExportableProperty::value() const
{
    return m_value;
}

void ExportableProperty::setValue(const QVariant &value)
{
    m_value = value;
}

QList<shared::ExportableAttribute> ExportableProperty::attributes() const
{
    return m_attrs;
}

void ExportableProperty::addAttribute(const ExportableAttribute &attr)
{
    m_attrs.append(attr);
}

void ExportableProperty::addAttribute(const QString &attrName, const QVariant &attrValue)
{
    m_attrs.append(shared::ExportableAttribute(attrName, attrValue));
}

QList<ExportableProperty> ExportableProperty::children() const
{
    return m_children;
}

ExportableProperty &ExportableProperty::child(const QString &name)
{
    auto it = std::find_if(m_children.begin(), m_children.end(),
            [name](const ExportableProperty &node) { return node.name() == name; });
    if (it != m_children.end())
        return *it;

    ExportableProperty node;
    node.setName(name);
    m_children.append(node);
    return m_children.last();
}

} // namespace shared
