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

#include "aadlobjectconnection.h"
#include "aadlobjectcontainer.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>

namespace taste3 {
namespace aadl {

const QHash<QString, AADLXMLReader::Token> AADLXMLReader::m_tokens = {
    { "InterfaceView", AADLXMLReader::Token::InterfaceView },

    // tags:
    { "Function", AADLXMLReader::Token::Function },
    { "Property", AADLXMLReader::Token::Property },
    { "Provided_Interface", AADLXMLReader::Token::Provided_Interface },
    { "Input_Parameter", AADLXMLReader::Token::Input_Parameter },
    { "Output_Parameter", AADLXMLReader::Token::Output_Parameter },
    { "Required_Interface", AADLXMLReader::Token::Required_Interface },
    { "Connection", AADLXMLReader::Token::Connection },

    // attrs:
    { "name", AADLXMLReader::Token::name },
    { "language", AADLXMLReader::Token::language },
    { "is_type", AADLXMLReader::Token::is_type },
    { "instance_of", AADLXMLReader::Token::instance_of },
    { "value", AADLXMLReader::Token::value },
    { "kind", AADLXMLReader::Token::kind },
    { "period", AADLXMLReader::Token::period },
    { "wcet", AADLXMLReader::Token::wcet },
    { "queue_size", AADLXMLReader::Token::queue_size },
    { "type", AADLXMLReader::Token::type },
    { "encoding", AADLXMLReader::Token::encoding },
    { "from", AADLXMLReader::Token::from },
    { "ri_name", AADLXMLReader::Token::ri_name },
    { "to", AADLXMLReader::Token::to },
    { "pi_name", AADLXMLReader::Token::pi_name },

    // TASTE props:
    { "Taste::Active_Interfaces", AADLXMLReader::Token::Active_Interfaces },
    { "Taste::coordinates", AADLXMLReader::Token::coordinates },
    { "Taste::Deadline", AADLXMLReader::Token::Deadline },
    { "Taste::InterfaceName", AADLXMLReader::Token::InterfaceName },
    { "Taste::RCMoperationKind", AADLXMLReader::Token::RCMoperationKind },
    { "Taste::RCMperiod", AADLXMLReader::Token::RCMperiod },
    { "Taste::labelInheritance", AADLXMLReader::Token::labelInheritance }
};

AADLXMLReader::Token AADLXMLReader::token(const QString &fromString)
{
    return m_tokens.value(fromString, Token::Unknown);
}

struct AADLXMLReaderPrivate {
    QVector<AADLObject *> m_allObjects {};
    QHash<QString, AADLObjectContainer *> m_functionNames {};
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
            if (token(xml.name().toString()) == Token::InterfaceView)
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
    switch (token(tagName)) {
    case AADLXMLReader::Token::Function: {
        return readFunction(xml);
    }
    case AADLXMLReader::Token::Connection: {
        return readConnection(xml);
    }
    default:
        qWarning() << badTagWarningMessage(xml, tagName);
        return false;
    }
}

bool AADLXMLReader::readFunction(QXmlStreamReader &xml, AADLObject *parent)
{
    AADLObjectContainer *obj = createFunction(xml, parent);
    if (!obj)
        return false;

    d->m_allObjects.append(obj);

    while (xml.readNextStartElement()) {
        const QString &name = xml.name().toString();
        switch (token(name)) {
        case Token::Property: {
            readFunctionProperty(xml, obj);
            xml.skipCurrentElement();
            break;
        }
        case Token::Provided_Interface:
        case Token::Required_Interface: {
            if (auto iface = readInterface(xml, obj))
                obj->addInterface(iface);
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, name);
            return false;
        }
        }
    }

    d->m_functionNames.insert(obj->title(), obj);

    return true;
}

bool AADLXMLReader::readFunctionProperty(QXmlStreamReader &xml, AADLObjectContainer *obj)
{
    if (!obj)
        return false;

    const QString &name = xml.attributes().value(m_tokens.key(Token::name)).toString();
    const QString &valueString = xml.attributes().value(m_tokens.key(Token::value)).toString();
    switch (token(name)) {
    case Token::Active_Interfaces: {
        obj->setActiveInterfaces(valueString.split(","));
        break;
    }
    case Token::coordinates: {
        const QStringList &strCoords = valueString.split(" ");
        const int coordsCount = strCoords.size();
        QVector<qint32> coords(coordsCount);
        for (int i = 0; i < coordsCount; ++i)
            coords[i] = strCoords[i].toLong() / 100;
        obj->setCoordinates(coords);
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
    Token ifaceType = token(tagName);
    const bool isProvided = ifaceType == Token::Provided_Interface;
    const bool isRequired = ifaceType == Token::Required_Interface;
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
    if (token(tagName) == Token::Unknown) {
        qWarning() << badTagWarningMessage(xml, tagName);
        return false;
    }

    const QXmlStreamAttributes &attrs(xml.attributes());

    for (const QXmlStreamAttribute &attr : attrs) {
        const QString &attrName = attr.name().toString();
        switch (token(attrName)) {
        case Token::name: {
            iface->setTitle(attrs.value(attrName).toString());
            break;
        }
        case Token::kind: {
            iface->setKind(attrs.value(attrName).toString());
            break;
        }
        case Token::period: {
            iface->setPeriod(attrs.value(attrName).toLong());
            break;
        }
        case Token::wcet: {
            iface->setWcet(attrs.value(attrName).toLong());
            break;
        }
        case Token::queue_size: {
            iface->setQueueSize(attrs.value(attrName).toLong());
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
        switch (token(name)) {
        case Token::Property: {
            readIfaceProperty(xml, iface);
            break;
        }
        case Token::Input_Parameter:
        case Token::Output_Parameter: {
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
    const QString &name = xml.attributes().value(m_tokens.key(Token::name)).toString();
    const QXmlStreamAttributes &attrs = xml.attributes();
    const QString &propVal = attrs.value(m_tokens.key(Token::value)).toString();
    switch (token(name)) {
    case Token::RCMoperationKind: {
        iface->setRcmOperationKind(propVal);
        break;
    }
    case Token::coordinates: {
        const QStringList &strCoords = propVal.split(" ");
        const int coordsCount = strCoords.size();
        QVector<qint32> coords(coordsCount);
        for (int i = 0; i < coordsCount; ++i)
            coords[i] = strCoords[i].toLong() / 100;
        iface->setCoordinates(coords);
        break;
    }
    case Token::Deadline: {
        iface->setDeadline(propVal);
        break;
    }
    case Token::RCMperiod: {
        iface->setRcmPeriod(propVal);
        break;
    }
    case Token::InterfaceName: {
        iface->setInterfaceName(propVal);
        break;
    }
    case Token::labelInheritance: {
        iface->setLabelInheritance(QVariant(propVal).toBool());
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
    const Token currParam = token(name);
    if (currParam != Token::Input_Parameter && currParam != Token::Output_Parameter) {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }

    IfaceParam param;

    const QXmlStreamAttributes &attributes = xml.attributes();
    for (const QXmlStreamAttribute &attr : attributes) {
        const QString &attrName = attr.name().toString();
        const QString &attrValue = attr.value().toString();

        switch (token(attrName)) {
        case AADLXMLReader::Token::name: {
            param.m_name = attrValue;
            break;
        }
        case AADLXMLReader::Token::type: {
            param.m_type = attrValue;
            break;
        }
        case AADLXMLReader::Token::encoding: {
            param.m_encoding = attrValue;
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, attrName);
            return false;
        }
        }
    }

    if (currParam == Token::Input_Parameter)
        iface->addParamIn(param);
    else
        iface->addParamOut(param);

    return true;
}

AADLObjectContainer *AADLXMLReader::createFunction(QXmlStreamReader &xml, AADLObject *parent)
{
    static const QXmlStreamAttribute attrContainer(m_tokens.key(AADLXMLReader::Token::is_type), "true");
    const QXmlStreamAttributes &attributes(xml.attributes());

    AADLObjectContainer *currObj = attributes.contains(attrContainer) ? new AADLObjectContainer(QString(), parent)
                                                                      : new AADLObjectFunction(QString(), parent);

    for (const QXmlStreamAttribute &attr : attributes) {
        const QString &attrName = attr.name().toString();
        switch (token(attrName)) {
        case AADLXMLReader::Token::name: {
            currObj->setTitle(attr.value().toString());
            break;
        }
        case AADLXMLReader::Token::language: {
            currObj->setLanguage(attr.value().toString());
            break;
        }
        case AADLXMLReader::Token::is_type: {
            // provided by currObj's class, ignore here
            break;
        }
        case AADLXMLReader::Token::instance_of: {
            currObj->setInstanceOf(attr.value().toString());
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, attrName);
            break;
        }
        }
    }

    return currObj;
}

struct ConnectionInitParamsHolderPrivate {
    AADLObject *m_from { nullptr };
    AADLObject *m_to { nullptr };
    AADLObjectIfaceRequired *m_ri { nullptr };
    AADLObjectIfaceProvided *m_pi { nullptr };
};

bool AADLXMLReader::readConnection(QXmlStreamReader &xml)
{
    const QString &name = xml.name().toString();
    const Token currParam = token(name);
    if (currParam != Token::Connection) {
        qWarning() << badTagWarningMessage(xml, name);
        return false;
    }

    ConnectionInitParamsHolderPrivate connectionInitParams;
    const QXmlStreamAttributes &attributes = xml.attributes();
    for (const QXmlStreamAttribute &attr : attributes) {
        const QString &attrName = attr.name().toString();
        const QString &attrValue = attr.value().toString();

        switch (token(attrName)) {
        case AADLXMLReader::Token::from: {
            connectionInitParams.m_from = d->m_functionNames.value(attrValue, nullptr);
            if (!connectionInitParams.m_from)
                qCritical() << Q_FUNC_INFO << attrValue;
            break;
        }
        case AADLXMLReader::Token::to: {
            connectionInitParams.m_to = d->m_functionNames.value(attrValue, nullptr);
            if (!connectionInitParams.m_to)
                qCritical() << Q_FUNC_INFO << attrValue;
            break;
        }
        case AADLXMLReader::Token::ri_name: {
            connectionInitParams.m_ri = d->m_ifaceRequiredNames.value(attrValue, nullptr);
            if (!connectionInitParams.m_ri)
                qCritical() << Q_FUNC_INFO << attrValue;
            break;
        }
        case AADLXMLReader::Token::pi_name: {
            connectionInitParams.m_pi = d->m_ifaceProvidedNames.value(attrValue, nullptr);
            if (!connectionInitParams.m_pi)
                qCritical() << Q_FUNC_INFO << attrValue;
            break;
        }
        default: {
            qWarning() << badTagWarningMessage(xml, name);
            return false;
        }
        }
    }

    /// TODO: remove this after discussions with critical warnings in the switch block
    if (!connectionInitParams.m_from || !connectionInitParams.m_to || !connectionInitParams.m_ri
        || !connectionInitParams.m_pi) {
        qCritical("Skipping connection from/to ENV");
        return true;
    }

    Q_ASSERT(connectionInitParams.m_from || connectionInitParams.m_to);
    Q_ASSERT(connectionInitParams.m_ri || connectionInitParams.m_pi);

    AADLObjectConnection *objConnection =
            new AADLObjectConnection(connectionInitParams.m_from, connectionInitParams.m_to, connectionInitParams.m_ri,
                                     connectionInitParams.m_pi);
    d->m_connectionNames.insert(objConnection->id().toString(), objConnection);
    d->m_allObjects.append(objConnection);
    return true;
}

} // ns aadl
} // ns taste3
