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

#include "propertytemplate.h"

#include "ivobject.h"

#include <QDomElement>
#include <QMetaEnum>
#include <QRegularExpressionMatch>

namespace ivm {

const QString kTagName = QLatin1String("Attr");

struct PropertyTemplate::PropertyTemplatePrivate {
    QString m_name;
    QString m_label;
    PropertyTemplate::Info m_info;
    PropertyTemplate::Type m_type;
    PropertyTemplate::Scopes m_scope;
    QVariant m_value;
    QVariant m_defaultValue;
    QString m_rxValueValidatorPattern;
    QMap<PropertyTemplate::Scope, QPair<QString, QString>> m_rxAttrValidatorPattern;
    bool m_isVisible = true;
    bool m_isSystem = false;
};

PropertyTemplate::PropertyTemplate()
    : d(new PropertyTemplatePrivate)
{
}

PropertyTemplate::~PropertyTemplate() { }

QString PropertyTemplate::name() const
{
    return d->m_name;
}

void PropertyTemplate::setName(const QString &name)
{
    d->m_name = name;
}

QString PropertyTemplate::label() const
{
    return d->m_label;
}

void PropertyTemplate::setLabel(const QString &label)
{
    d->m_label = label;
}

PropertyTemplate::Info PropertyTemplate::info() const
{
    return d->m_info;
}

void PropertyTemplate::setInfo(PropertyTemplate::Info info)
{
    d->m_info = info;
}

bool PropertyTemplate::isSystem() const
{
    return d->m_isSystem;
}

void PropertyTemplate::setSystem(bool value)
{
    d->m_isSystem = value;
}

PropertyTemplate::Type PropertyTemplate::type() const
{
    return d->m_type;
}

void PropertyTemplate::setType(PropertyTemplate::Type t)
{
    d->m_type = t;
}

PropertyTemplate::Scopes PropertyTemplate::scope() const
{
    return d->m_scope;
}

void PropertyTemplate::setScope(const PropertyTemplate::Scopes &s)
{
    d->m_scope = s;
}

bool PropertyTemplate::isVisible() const
{
    return d->m_isVisible;
}

void PropertyTemplate::setVisible(bool value)
{
    d->m_isVisible = value;
}

QVariant PropertyTemplate::value() const
{
    return d->m_value;
}

void PropertyTemplate::setValue(const QVariant &value)
{
    d->m_value = value;
}

QVariant PropertyTemplate::defaultValue() const
{
    return d->m_defaultValue;
}

void PropertyTemplate::setDefaultValue(const QVariant &value)
{
    d->m_defaultValue = value;
}

QString PropertyTemplate::valueValidatorPattern() const
{
    return d->m_rxValueValidatorPattern;
}

void PropertyTemplate::setValueValidatorPattern(const QString &pattern)
{
    d->m_rxValueValidatorPattern = pattern;
}

QMap<PropertyTemplate::Scope, QPair<QString, QString>> PropertyTemplate::attrValidatorPatterns() const
{
    return d->m_rxAttrValidatorPattern;
}

void PropertyTemplate::setAttrValidatorPattern(const QMap<Scope, QPair<QString, QString>> &pattern)
{
    d->m_rxAttrValidatorPattern = pattern;
}

QDomElement PropertyTemplate::toXml(QDomDocument *doc) const
{
    QDomElement attrElement = doc->createElement(kTagName);
    attrElement.setAttribute(QLatin1String("name"), d->m_name);
    if (!d->m_label.isEmpty()) {
        attrElement.setAttribute(QLatin1String("label"), d->m_label);
    }
    if (!d->m_isVisible) {
        attrElement.setAttribute(QLatin1String("visible"), QLatin1String("false"));
    }

    static const QMetaEnum &infoMeta = QMetaEnum::fromType<PropertyTemplate::Info>();
    if (d->m_info == PropertyTemplate::Info::Property) {
        attrElement.setAttribute(QLatin1String("type"), infoMeta.valueToKey(static_cast<int>(d->m_info)));
    }

    static const QMetaEnum &typeMeta = QMetaEnum::fromType<PropertyTemplate::Type>();
    QDomElement typeElement = doc->createElement(typeMeta.name());
    const QString typeString = typeMeta.valueToKey(static_cast<int>(d->m_type));
    QDomElement typeSubElement = doc->createElement(typeString);
    if (!d->m_rxValueValidatorPattern.isEmpty()) {
        typeSubElement.setAttribute(QLatin1String("validator"), d->m_rxValueValidatorPattern);
    }
    if (d->m_type == PropertyTemplate::Type::Enumeration) {
        for (auto entry : d->m_value.toList()) {
            QDomElement entryElement = doc->createElement(QLatin1String("Entry"));
            entryElement.setAttribute(QLatin1String("value"), entry.toString());
            typeSubElement.appendChild(entryElement);
        }
    }
    typeElement.appendChild(typeSubElement);
    attrElement.appendChild(typeElement);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    static const QMetaEnum &scopeMeta = QMetaEnum::fromType<PropertyTemplate::Scopes>();
    static const QString scopesElementTag = scopeMeta.name();
#else
    static const QMetaEnum &scopeMeta = QMetaEnum::fromType<PropertyTemplate::Scope>();
    static const QString scopesElementTag = QLatin1String("Scopes");
#endif
    QDomElement scopeElement = doc->createElement(scopesElementTag);
    auto scopesToString = [](const PropertyTemplate::Scopes s) -> QStringList {
        QStringList result;
        for (int idx = 0; idx < scopeMeta.keyCount(); ++idx) {
            if (s.testFlag(static_cast<PropertyTemplate::Scope>(scopeMeta.value(idx)))) {
                result.append(QString::fromLatin1(scopeMeta.key(idx)));
            }
        }
        if (result.isEmpty()) {
            return { scopeMeta.valueToKeys(static_cast<int>(PropertyTemplate::Scope::None)) };
        }
        return result;
    };
    for (const QString &scope : scopesToString(d->m_scope)) {
        QDomElement scopeSubElement = doc->createElement(scope);
        for (auto it = d->m_rxAttrValidatorPattern.constBegin(); it != d->m_rxAttrValidatorPattern.constEnd(); ++it) {
            QDomElement attrValidatorElement = doc->createElement(QLatin1String("AttrValidator"));
            attrValidatorElement.setAttribute(QLatin1String("name"), it.value().first);
            attrValidatorElement.setAttribute(QLatin1String("value"), it.value().second);
            scopeSubElement.appendChild(attrValidatorElement);
        }
        scopeElement.appendChild(scopeSubElement);
    }
    attrElement.appendChild(scopeElement);

    return attrElement;
}

PropertyTemplate *PropertyTemplate::fromXml(const QDomElement &element)
{
    if (element.isNull() || element.tagName() != kTagName || !element.hasAttribute(QLatin1String("name"))
            || !element.hasChildNodes()) {
        return nullptr;
    }
    bool ok;

    const QString attrName = element.attribute(QLatin1String("name"));
    const QString attrLabel = element.attribute(QLatin1String("label"));

    const QString attrType = element.attribute(QLatin1String("type"), QLatin1String("Attribute"));
    const bool isVisible =
            QString::compare(element.attribute(QLatin1String("visible")), QLatin1String("false"), Qt::CaseInsensitive)
            != 0;
    static const QMetaEnum &infoMeta = QMetaEnum::fromType<PropertyTemplate::Info>();
    const int infoInt = infoMeta.keyToValue(attrType.toUtf8().data(), &ok);
    const PropertyTemplate::Info i =
            ok ? static_cast<PropertyTemplate::Info>(infoInt) : PropertyTemplate::Info::Property;

    static const QMetaEnum &typeMeta = QMetaEnum::fromType<PropertyTemplate::Type>();
    const QDomElement typeElement = element.firstChildElement(typeMeta.name());
    PropertyTemplate::Type t = PropertyTemplate::Type::Unknown;

    QVariant defaultValue { QVariant::String };
    QVariant value;
    QString typeValidator;
    if (!typeElement.isNull()) {
        const QDomElement typeSubElement = typeElement.firstChildElement();
        if (!typeSubElement.isNull()) {
            typeValidator = typeSubElement.attribute(QLatin1String("validator"));
            const int typeInt = typeMeta.keyToValue(typeSubElement.tagName().toUtf8().data(), &ok);
            if (ok && typeInt != -1) {
                t = static_cast<PropertyTemplate::Type>(typeInt);
            }
            if (t == PropertyTemplate::Type::Enumeration) {
                defaultValue = typeSubElement.attribute(QLatin1String("defaultValue"));
                QDomElement typeEntryElement = typeSubElement.firstChildElement(QLatin1String("Entry"));
                QList<QVariant> values;
                while (!typeEntryElement.isNull()) {
                    const QVariant enumValue = typeEntryElement.attribute(QLatin1String("value"));
                    values.append(enumValue);
                    typeEntryElement = typeEntryElement.nextSiblingElement(typeEntryElement.tagName());
                }
                value = values;
            } else {
                defaultValue = convertData(typeSubElement.attribute(QLatin1String("defaultValue")), t);
            }
        }
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    static const QMetaEnum &scopeMeta = QMetaEnum::fromType<PropertyTemplate::Scopes>();
    static const QString scopesElementTag = scopeMeta.name();
#else
    static const QMetaEnum &scopeMeta = QMetaEnum::fromType<PropertyTemplate::Scope>();
    static const QString scopesElementTag = QLatin1String("Scopes");
#endif
    const QDomElement scopeElement = element.firstChildElement(scopesElementTag);
    PropertyTemplate::Scopes s = PropertyTemplate::Scope::None;
    QMap<PropertyTemplate::Scope, QPair<QString, QString>> attrValidators;
    if (scopeElement.isNull() || !scopeElement.hasChildNodes()) {
        s = PropertyTemplate::Scope::All;
    } else {
        QDomElement scopeSubElement = scopeElement.firstChildElement();
        while (!scopeSubElement.isNull()) {
            const QString scopeName = scopeSubElement.tagName();
            const int scopeInt = scopeMeta.keyToValue(scopeName.toUtf8().data(), &ok);
            if (!ok || scopeInt == -1) {
                continue;
            }
            const PropertyTemplate::Scope scope = static_cast<PropertyTemplate::Scope>(scopeInt);
            s.setFlag(scope);
            QDomElement attrValidatorElement = scopeSubElement.firstChildElement(QLatin1String("AttrValidator"));
            while (!attrValidatorElement.isNull()) {
                const QString attrValidatorName = attrValidatorElement.attribute(QLatin1String("name"));
                const QString attrValidatorPattern = attrValidatorElement.attribute(QLatin1String("value"));
                if (!attrValidatorName.isEmpty() && !attrValidatorPattern.isNull()) {
                    attrValidators.insert(scope, qMakePair(attrValidatorName, attrValidatorPattern));
                }
                attrValidatorElement = attrValidatorElement.nextSiblingElement(attrValidatorElement.tagName());
            }
            scopeSubElement = scopeSubElement.nextSiblingElement();
        }
    }

    auto propertyTemplate = new PropertyTemplate;
    propertyTemplate->setName(attrName);
    propertyTemplate->setLabel(attrLabel);
    propertyTemplate->setInfo(i);
    propertyTemplate->setType(t);
    propertyTemplate->setScope(s);
    propertyTemplate->setValue(value);
    propertyTemplate->setDefaultValue(defaultValue);
    propertyTemplate->setAttrValidatorPattern(attrValidators);
    propertyTemplate->setValueValidatorPattern(typeValidator);
    propertyTemplate->setVisible(isVisible);
    return propertyTemplate;
}

QString PropertyTemplate::tagName()
{
    return kTagName;
}

QVariant PropertyTemplate::convertData(const QVariant &value, PropertyTemplate::Type type)
{
    QVariant typedValue;
    switch (type) {
    case PropertyTemplate::Type::Boolean: {
        const bool falseValue = QString::compare(value.toString(), QLatin1String("false"), Qt::CaseInsensitive) == 0;
        const bool trueValue = QString::compare(value.toString(), QLatin1String("true"), Qt::CaseInsensitive) == 0;
        if (falseValue) {
            typedValue = false;
        } else if (trueValue) {
            typedValue = true;
        } else {
            return QVariant(QVariant::Bool);
        }
    } break;
    case PropertyTemplate::Type::Integer: {
        bool ok;
        typedValue = value.toString().toInt(&ok);
        if (!ok)
            return QVariant(QVariant::Int);
    } break;
    case PropertyTemplate::Type::Real: {
        bool ok;
        typedValue = value.toString().toDouble(&ok);
        if (!ok)
            return QVariant(QVariant::Double);
    } break;
    case PropertyTemplate::Type::String: {
        if (value.isValid())
            typedValue = value.toString();
        else
            typedValue = QVariant(QVariant::String);
    } break;
    case PropertyTemplate::Type::Enumeration: {
        if (value.isValid()) {
            QStringList typedList;
            for (const auto &dataItem : value.toList()) {
                typedList.append(dataItem.toString());
            }
            typedValue = typedList;
        } else {
            typedValue = QVariant(QVariant::StringList);
        }
    } break;
    default:
        break;
    }
    return typedValue;
}

static inline PropertyTemplate::Scope typeToScope(const IVObject::Type &type)
{
    auto scope = PropertyTemplate::Scope::None;
    switch (type) {
    case ivm::IVObject::Type::Connection:
        scope = PropertyTemplate::Scope::Connection;
        break;
    case ivm::IVObject::Type::Comment:
        scope = PropertyTemplate::Scope::Comment;
        break;
    case ivm::IVObject::Type::FunctionType:
    case ivm::IVObject::Type::Function:
        scope = PropertyTemplate::Scope::Function;
        break;
    case ivm::IVObject::Type::RequiredInterface:
        scope = PropertyTemplate::Scope::Required_Interface;
        break;
    case ivm::IVObject::Type::ProvidedInterface:
        scope = PropertyTemplate::Scope::Provided_Interface;
        break;
    default:
        break;
    }
    return scope;
}

/*!
 * \brief PropertyTemplate::validate
 * \param object which attributes to be checked
 * \return
 */
bool PropertyTemplate::validate(const IVObject *object) const
{
    const PropertyTemplate::Scope objectScope = typeToScope(object->type());
    if (!d->m_scope.testFlag(objectScope)) {
        return false;
    }

    if (d->m_rxAttrValidatorPattern.isEmpty()) {
        return true;
    }

    const auto it = d->m_rxAttrValidatorPattern.constFind(objectScope);
    if (it != d->m_rxAttrValidatorPattern.constEnd()) {
        auto checkPattern = [](const EntityAttributes &data, const QString &name, const QString &pattern) {
            auto objAttrIter = data.constFind(name);
            if (objAttrIter != data.constEnd()) {
                const QRegularExpression rx(pattern);
                const QString value = objAttrIter.value().value<QString>();
                const QRegularExpressionMatch match = rx.match(value);
                return match.capturedLength() == value.length();
            }
            return true;
        };
        return checkPattern(object->entityAttributes(), it->first, it->second);
    }
    return false;
}

} // namespace ivm
