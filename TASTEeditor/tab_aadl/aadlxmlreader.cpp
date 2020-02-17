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

#include "aadlxmlreader.h"

#include "aadlcommonprops.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "tab_interface/connectioncreationvalidator.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>

namespace taste3 {
namespace aadl {

using namespace taste3::aadl::meta;

struct AADLXMLReaderPrivate {
    QVector<AADLObject *> m_allObjects {};
    QHash<QString, AADLObjectFunctionType *> m_functionNames {};
    QHash<QString, AADLObjectIfaceRequired *> m_ifaceRequiredNames {};
    QHash<QString, AADLObjectIfaceProvided *> m_ifaceProvidedNames {};
    QHash<QString, AADLObjectConnection *> m_connectionNames {};
};

AADLXMLReader::AADLXMLReader(QObject *parent)
    : QObject(parent)
    , d(new AADLXMLReaderPrivate)
{
}

AADLXMLReader::~AADLXMLReader() {}

QString AADLXMLReader::badTagWarningMessage(const QXmlStreamReader &xml, const QString &tag)
{
    static const QString msg("The '%1' is unknown/unexpedted here: %2@%3 %4");
    return msg.arg(tag, QString::number(xml.lineNumber()), QString::number(xml.columnNumber()), xml.tokenString());
}

bool AADLXMLReader::handleError(const QXmlStreamReader &xml)
{
    if (xml.hasError()) {
        qWarning() << xml.errorString();
        emit error(xml.errorString());
        return true;
    }
    return false;
}

bool AADLXMLReader::parse(const QString &file)
{
    QFile in(file);
    if (in.exists(file) && in.open(QFile::ReadOnly | QFile::Text)) {
        if (parse(&in)) {
            emit objectsParsed(d->m_allObjects);
            return true;
        }
    }

    const QString &errMsg = QString("Can't open file %1: %2").arg(file, in.errorString());
    qWarning() << errMsg;
    emit error(errMsg);

    return false;
}

bool AADLXMLReader::parse(QIODevice *in)
{
    if (!in)
        return false;

    QXmlStreamReader xml(in);
    if (xml.readNext() == QXmlStreamReader::StartDocument)
        if (xml.readNext() == QXmlStreamReader::StartElement)
            if (Props::token(xml.name().toString()) == Props::Token::InterfaceView)
                return readInterfaceView(xml);

    return false;
}

bool AADLXMLReader::readInterfaceView(QXmlStreamReader &xml)
{
    while (!xml.atEnd()) {
        if (handleError(xml))
            return false;
        if (xml.readNextStartElement())
            readAADLObject(xml);
    }

    return true;
}

bool AADLXMLReader::readAADLObject(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (Props::token(tagName)) {
    case Props::Token::Function: {
        return readFunction(xml);
    }
    case Props::Token::Connection: {
        return readConnection(xml);
    }
    case Props::Token::Comment: {
        return readComment(xml);
    }
    default:
        qWarning() << badTagWarningMessage(xml, tagName);
        return false;
    }
}

bool AADLXMLReader::readFunction(QXmlStreamReader &xml, AADLObject *parent)
{
    AADLObjectFunctionType *obj = createFunction(xml, parent);
    if (!obj)
        return false;

    d->m_allObjects.append(obj);

    while (xml.readNextStartElement()) {
        const QString &name = xml.name().toString();
        switch (Props::token(name)) {
        case Props::Token::Function: {
            readFunction(xml, obj);
            break;
        }
        case Props::Token::Property: {
            readFunctionProperty(xml, obj);
            xml.skipCurrentElement();
            break;
        }
        case Props::Token::Provided_Interface:
        case Props::Token::Required_Interface: {
            if (auto iface = readInterface(xml, obj))
                obj->addInterface(iface);
            break;
        }
        case Props::Token::Connection: {
            readConnection(xml, obj);
            break;
        }
        case Props::Token::Comment: {
            readComment(xml, obj);
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, name);
            d->m_allObjects.removeAll(obj);
            delete obj;
            return false;
        }
        }
    }

    d->m_functionNames.insert(obj->title(), obj);

    return true;
}

bool AADLXMLReader::readFunctionProperty(QXmlStreamReader &xml, AADLObjectFunctionType *obj)
{
    if (!obj)
        return false;

    const QString &name = xml.attributes().value(Props::token(Props::Token::name)).toString();
    const QString &valueString = xml.attributes().value(Props::token(Props::Token::value)).toString();
    switch (Props::token(name)) {
    case Props::Token::Active_Interfaces:
    case Props::Token::InnerCoordinates:
    case Props::Token::coordinates: {
        obj->setProp(name, valueString);
        break;
    }
    default: {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }
    }
    return true;
}

AADLObjectIface *AADLXMLReader::readInterface(QXmlStreamReader &xml, AADLObject *parent)
{
    const QString &tagName = xml.name().toString();
    Props::Token ifaceType = Props::token(tagName);
    const bool isProvided = ifaceType == Props::Token::Provided_Interface;
    const bool isRequired = ifaceType == Props::Token::Required_Interface;
    const bool isIface = isProvided || isRequired;
    if (!isIface) {
        qWarning() << badTagWarningMessage(xml, tagName);
        return nullptr;
    }

    AADLObjectIface *iface(nullptr);
    if (isProvided)
        iface = new AADLObjectIfaceProvided(parent);
    else
        iface = new AADLObjectIfaceRequired(parent);

    readIfaceAttributes(xml, iface);

    readIfaceProperties(xml, iface);

    if (isProvided)
        d->m_ifaceProvidedNames.insert(iface->title(), qobject_cast<AADLObjectIfaceProvided *>(iface));
    else
        d->m_ifaceRequiredNames.insert(iface->title(), qobject_cast<AADLObjectIfaceRequired *>(iface));
    d->m_allObjects.append(iface);

    return iface;
}

bool AADLXMLReader::readIfaceAttributes(QXmlStreamReader &xml, AADLObjectIface *iface)
{
    if (!iface)
        return false;

    const QString &tagName = xml.name().toString();
    if (Props::token(tagName) == Props::Token::Unknown) {
        qWarning() << badTagWarningMessage(xml, tagName);
        return false;
    }

    const QXmlStreamAttributes &attrs(xml.attributes());

    for (const QXmlStreamAttribute &attr : attrs) {
        const QString &attrName = attr.name().toString();
        switch (Props::token(attrName)) {
        case Props::Token::name:
        case Props::Token::kind:
        case Props::Token::period:
        case Props::Token::wcet:
        case Props::Token::queue_size: {
            iface->setAttr(attrName, attrs.value(attrName).toString());
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, attrName);
            return false;
        }
        }
    }

    return true;
}

bool AADLXMLReader::readIfaceProperties(QXmlStreamReader &xml, AADLObjectIface *iface)
{
    while (xml.readNextStartElement()) {
        if (handleError(xml))
            return false;

        const QString &name = xml.name().toString();
        switch (Props::token(name)) {
        case Props::Token::Property: {
            readIfaceProperty(xml, iface);
            break;
        }
        case Props::Token::Input_Parameter:
        case Props::Token::Output_Parameter: {
            readIfaceParameter(xml, iface);
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, name);
            return false;
        }
        }

        xml.skipCurrentElement();
    }
    return true;
}

bool AADLXMLReader::readIfaceProperty(QXmlStreamReader &xml, AADLObjectIface *iface)
{
    const QString &name = xml.attributes().value(Props::token(Props::Token::name)).toString();
    const QXmlStreamAttributes &attrs = xml.attributes();
    const QString &propVal = attrs.value(Props::token(Props::Token::value)).toString();
    switch (Props::token(name)) {
    case Props::Token::RCMoperationKind:
    case Props::Token::InnerCoordinates:
    case Props::Token::coordinates:
    case Props::Token::Deadline:
    case Props::Token::RCMperiod:
    case Props::Token::InterfaceName:
    case Props::Token::labelInheritance: {
        iface->setProp(name, propVal);
        break;
    }
    default: {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }
    }
    return true;
}

bool AADLXMLReader::readIfaceParameter(QXmlStreamReader &xml, AADLObjectIface *iface)
{
    const QString &name = xml.name().toString();
    const Props::Token currParam = Props::token(name);
    if (currParam != Props::Token::Input_Parameter && currParam != Props::Token::Output_Parameter) {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }

    IfaceParameter param;

    const QXmlStreamAttributes &attributes = xml.attributes();
    for (const QXmlStreamAttribute &attr : attributes) {
        const QString &attrName = attr.name().toString();
        const QString &attrValue = attr.value().toString();

        switch (Props::token(attrName)) {
        case Props::Token::name: {
            param.setName(attrValue);
            break;
        }
        case Props::Token::type: {
            param.setParamTypeName(attrValue);
            break;
        }
        case Props::Token::encoding: {
            param.setEncoding(attrValue);
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, attrName);
            return false;
        }
        }
    }

    param.setDirection(currParam == Props::Token::Input_Parameter ? IfaceParameter::Direction::In
                                                                  : IfaceParameter::Direction::Out);
    iface->addParam(param);

    return true;
}

AADLObjectFunctionType *AADLXMLReader::createFunction(QXmlStreamReader &xml, AADLObject *parent)
{
    const QXmlStreamAttributes &attributes(xml.attributes());

    QHash<QString, QString> attrs;
    for (const QXmlStreamAttribute &attr : attributes) {
        const QString &attrName = attr.name().toString();
        switch (Props::token(attrName)) {
        case Props::Token::name:
        case Props::Token::language:
        case Props::Token::is_type:
        case Props::Token::instance_of: {
            attrs.insert(attrName, attr.value().toString());
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, attrName);
            break;
        }
        }
    }

    static const QString attrName_isType = Props::token(Props::Token::is_type);
    bool isFunctionType(false);
    if (attrs.contains(attrName_isType))
        isFunctionType = attrs.take(attrName_isType).toLower() == "yes";

    const bool parentIsFunctionType = parent && parent->aadlType() == AADLObject::AADLObjectType::AADLFunctionType;
    const bool nestingAllowd = !(parentIsFunctionType && isFunctionType); // direct FnT->FnT nesting is not allowed
    AADLObject *usedParent = nestingAllowd ? parent : nullptr;
    AADLObjectFunctionType *currObj = isFunctionType ? new AADLObjectFunctionType(QString(), usedParent)
                                                     : new AADLObjectFunction(QString(), usedParent);
    if (nestingAllowd) {
        if (AADLObjectFunctionType *parentFunction = qobject_cast<AADLObjectFunctionType *>(parent))
            parentFunction->addChild(currObj);
    }

    QHash<QString, QString>::const_iterator i = attrs.cbegin();
    while (i != attrs.cend()) {
        currObj->setAttr(i.key(), i.value());
        ++i;
    }

    return currObj;
}

struct ConnectionEndPoint {
    AADLObject *m_function { nullptr };
    AADLObjectIface *m_interface { nullptr };
    bool isReady() const { return m_function && m_interface; }
};

struct ConnectionHolder {
    ConnectionEndPoint m_from;
    ConnectionEndPoint m_to;

    bool isValid() const { return m_from.isReady() && m_to.isReady(); }
};

bool AADLXMLReader::readConnection(QXmlStreamReader &xml, AADLObject *parent)
{
    const QString &name = xml.name().toString();
    const Props::Token currParam = Props::token(name);
    if (currParam != Props::Token::Connection) {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }

    auto readEndpointAttributes = [this](ConnectionEndPoint &endpoint, const QXmlStreamAttributes &attrs) {
        for (const QXmlStreamAttribute &attr : attrs) {

            const QString &attrName = attr.name().toString();
            const QString &attrValue = attr.value().toString();

            switch (Props::token(attrName)) {
            case Props::Token::func_name: {
                endpoint.m_function = d->m_functionNames.value(attrValue, nullptr);
                break;
            }
            case Props::Token::si_name:
            case Props::Token::ri_name: {
                endpoint.m_interface = d->m_ifaceRequiredNames.value(attrValue, nullptr);
                break;
            }
            case Props::Token::ti_name:
            case Props::Token::pi_name: {
                endpoint.m_interface = d->m_ifaceProvidedNames.value(attrValue, nullptr);
                break;
            }
            default: {
                qWarning() << "Unexeptected Connection attribute:" << attrName << attrValue;
                break;
            }
            }
        }
    };

    auto readEndpoint = [readEndpointAttributes, &xml](ConnectionHolder &connection) {
        const Props::Token t = Props::token(xml.name().toString());
        switch (t) {
        case Props::Token::Source:
        case Props::Token::Target: {
            const bool isSrc = t == Props::Token::Source;
            readEndpointAttributes(isSrc ? connection.m_from : connection.m_to, xml.attributes());
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, xml.name().toString());
            return false;
        }
        }
        return true;
    };

    auto readConnectionPart = [&readEndpoint, &xml](ConnectionHolder &connection) {
        if (!xml.readNextStartElement())
            return false;

        if (readEndpoint(connection)) {
            xml.skipCurrentElement();
            return true;
        }
        return false;
    };

    ConnectionHolder connection;

    if (!readConnectionPart(connection)) // read the first one (Source or Target)
        return false;
    if (!readConnectionPart(connection)) // read the second one (Target or Source)
        return false;

    if (connection.isValid()) {
        xml.skipCurrentElement();
        const ConnectionCreationValidator::FailReason status =
                ConnectionCreationValidator::canConnect(connection.m_from.m_function->as<AADLObjectFunction *>(),
                                                        connection.m_to.m_function->as<AADLObjectFunction *>(),
                                                        connection.m_from.m_interface, connection.m_to.m_interface);
        if (status == ConnectionCreationValidator::FailReason::NotFail) {
            AADLObjectConnection *objConnection =
                    new AADLObjectConnection(connection.m_from.m_function, connection.m_to.m_function,
                                             connection.m_from.m_interface, connection.m_to.m_interface, parent);

            d->m_connectionNames.insert(objConnection->id().toString(), objConnection);
            d->m_allObjects.append(objConnection);
            return true;
        } else {
            auto edgeName = [](const ConnectionEndPoint &edge) {
                static const QString nameTemplate("%1.%2");
                static const QLatin1String strNull("null");
                return nameTemplate.arg((edge.m_function ? edge.m_function->title() : strNull),
                                        (edge.m_interface ? edge.m_interface->title() : strNull));
            };

            const QString connectionName =
                    QString("%1->%2").arg(edgeName(connection.m_from), edgeName(connection.m_to));
            qWarning() << "Can't perform connection" << connectionName << status;
            return false;
        }
    }

    return false;
}

bool AADLXMLReader::readComment(QXmlStreamReader &xml, AADLObject *parent)
{
    const QString &name = xml.name().toString();
    const Props::Token currParam = Props::token(name);
    if (currParam != Props::Token::Comment) {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }

    AADLObjectComment *obj = new AADLObjectComment(QString(), parent);
    if (AADLObjectFunctionType *fn = qobject_cast<AADLObjectFunctionType *>(obj->parentObject()))
        fn->addChild(obj);
    d->m_allObjects.append(obj);

    for (const QXmlStreamAttribute &attr : xml.attributes()) {
        const QString &attrName = attr.name().toString();
        const QString &attrValue = attr.value().toString();

        switch (Props::token(attrName)) {
        case Props::Token::name: {
            obj->setAttr(attrName, attrValue);
            break;
        }
        default:
            qWarning() << "Unexpected Comment's attribute found:" << attrName << attrValue;
            break;
        }
    }

    auto readCommentProperty = [&xml, &obj]() {
        const QXmlStreamAttributes &attrs = xml.attributes();
        const QString &propName = attrs.value(Props::token(Props::Token::name)).toString();
        const QString &propVal = attrs.value(Props::token(Props::Token::value)).toString();
        switch (Props::token(propName)) {
        case Props::Token::coordinates: {
            obj->setProp(propName, propVal);
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, xml.name().toString());
            return false;
        }
        }
        return true;
    };

    if (xml.readNextStartElement()) {
        switch (Props::token(xml.name().toString())) {
        case Props::Token::Property: {
            if (readCommentProperty())
                xml.skipCurrentElement();
            else
                return false;
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, xml.name().toString());
            return false;
        }
        }
    }

    return true;
}

} // ns aadl
} // ns taste3
