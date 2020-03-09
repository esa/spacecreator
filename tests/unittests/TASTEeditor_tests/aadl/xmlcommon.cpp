/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "xmlcommon.h"

#include <QDebug>
#include <QFile>
#include <QMetaEnum>

using namespace taste3::aadl;

XmlFileMock::XmlFileMock(const QString &content, int expectedErrors, bool canBeParsed)
    : m_xmlContent(content)
    , m_expectedErrorCount(expectedErrors)
    , m_canBeParsed(canBeParsed)
{
}

int XmlFileMock::expectedObjectCount(AADLObject::AADLObjectType t) const
{
    int result = 0;
    if (AADLObject::AADLObjectType::AADLUnknown == t) {
        for (int count : m_objectCountByType)
            result += count;
        return result;
    }

    return m_objectCountByType.value(t);
}

void XmlFileMock::setExpectedObjectCount(AADLObject::AADLObjectType t, int count)
{
    if (AADLObject::AADLObjectType::AADLUnknown != t && count >= 0)
        m_objectCountByType[t] = count;
}

XmlFileMock XmlFileMock::createEmptyFile()
{
    XmlFileMock file(QString(), 0, false);

    const QMetaEnum &me = QMetaEnum::fromType<taste3::aadl::AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        file.setExpectedObjectCount(t, 0);
    }

    return file;
}

XmlFileMock XmlFileMock::createEmptyDoc()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n"
                                    "</InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        file.setExpectedObjectCount(t, 0);
    }

    return file;
}

XmlFileMock XmlFileMock::createSingleFunction()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Function name=\"Foo\" language=\"C\" is_type=\"NO\" instance_of=\"\">\n \
                                    <Property name=\"Taste::coordinates\" value =\"142673 57006 169917 79368\" />\n \
                                    <Property name=\"Taste::SomeOtherProp\" value =\"Some value\" />\n \
                                    </Function>\n \
                                    </InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunction:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlFileMock XmlFileMock::createSingleFunctionType()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Function name=\"FooType\" language=\"C\" is_type=\"yes\" instance_of=\"\">\n \
                                    <Property name=\"Taste::coordinates\" value =\"142673 57006 169917 79368\" />\n \
                                    <Property name=\"Taste::SomeOtherProp\" value =\"Some value\" />\n \
                                    </Function>\n \
                                    </InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunctionType:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlFileMock XmlFileMock::createSingleComment()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Comment name=\"Comment text\"> \
                                        <Property name=\"Taste::coordinates\" value=\"-2200 27700 21700 36200\"/> \
                                        <Property name=\"Taste::unknownProperty\" value=\"foo\"/> \
                                    </Comment> \n \
                                    </InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLComment:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlFileMock XmlFileMock::createSingleIfaceInvalid()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Required_Interface name=\"GlobalFnType_PI\" kind=\"SPORADIC_OPERATION\" period=\"\" wcet=\"\" queue_size=\"\">\n \
                                        <Property name=\"Taste::coordinates\" value=\"35000 25400\"/>\n \
                                        <Property name=\"Taste::labelInheritance\" value=\"true\"/>\n \
                                    </Required_Interface>\n \
                                    </InterfaceView>"),
                     1, false);

    return file;
}

XmlFileMock XmlFileMock::createSingleIfaceValid()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Function name=\"Global_Fn3\" language=\"\" is_type=\"NO\" instance_of=\"\">\n \
                                        <Property name=\"Taste::coordinates\" value=\"-5200 25400 59900 38700\"/>\n \
                                        <Required_Interface name=\"GlobalFnType_PI\" kind=\"SPORADIC_OPERATION\" period=\"\" wcet=\"\" queue_size=\"\">\n \
                                            <Property name=\"Taste::coordinates\" value=\"35000 25400\"/>\n \
                                            <Property name=\"Taste::labelInheritance\" value=\"true\"/>\n \
                                        </Required_Interface>\n \
                                    </Function>\n \
                                    </InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunction:
            file.setExpectedObjectCount(t, 1);
            break;
        case AADLObject::AADLObjectType::AADLIface:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlFileMock XmlFileMock::createParametrizedIfaces()
{
    QString content;

    QFile datFile(":/data/ifaceparams.xml");
    if (datFile.open(QIODevice::ReadOnly | QIODevice::Text))
        content = QString::fromUtf8(datFile.readAll());
    else {
        qWarning() << "Failed loading test data file:" << datFile.fileName() << datFile.errorString();
        return XmlFileMock(QString(), 0, false);
    }

    XmlFileMock file(content);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunction:
            file.setExpectedObjectCount(t, 2);
            break;
        case AADLObject::AADLObjectType::AADLIface:
            file.setExpectedObjectCount(t, 2);
            break;
        case AADLObject::AADLObjectType::AADLConnection:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlFileMock XmlFileMock::createSingleConnectionOrphan()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                        <Connection>\n \
                                            <Source func_name=\"NestedFn_typed_Global\" ri_name=\"RI_8\"/>\n \
                                            <Target func_name=\"NestedFn1\" pi_name=\"PI_8\"/>\n \
                                        </Connection>\n \
                                    </InterfaceView>"),
                     0, true);

    file.setExpectedObjectCount(AADLObject::AADLObjectType::AADLConnection, 1);

    return file;
}

XmlFileMock XmlFileMock::createSingleConnectionValid()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Function name=\"Function_1\" language=\"\" is_type=\"NO\" instance_of=\"\">\n \
                                        <Property name=\"Taste::coordinates\" value=\"-62300 -36600 -42300 -28600\"/>\n \
                                        <Required_Interface name=\"RI_2\" kind=\"SPORADIC_OPERATION\" period=\"\" wcet=\"\" queue_size=\"\">\n \
                                            <Property name=\"Taste::coordinates\" value=\"-42300 -33000\"/>\n \
                                            <Property name=\"Taste::labelInheritance\" value=\"true\"/>\n \
                                        </Required_Interface>\n \
                                    </Function>\n \
                                    <Function name=\"Function_2\" language=\"\" is_type=\"NO\" instance_of=\"\">\n \
                                        <Property name=\"Taste::coordinates\" value=\"-30600 -36600 -10600 -28600\"/>\n \
                                        <Provided_Interface name=\"PI_2\" kind=\"SPORADIC_OPERATION\" period=\"\" wcet=\"\" queue_size=\"\">\n \
                                            <Property name=\"Taste::coordinates\" value=\"-30600 -33000\"/>\n \
                                        </Provided_Interface>\n \
                                    </Function>\n \
                                    <Connection>\n \
                                        <Source func_name=\"Function_1\" ri_name=\"RI_2\"/>\n \
                                        <Target func_name=\"Function_2\" pi_name=\"PI_2\"/>\n \
                                    </Connection>\n \
                                </InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunction:
            file.setExpectedObjectCount(t, 2);
            break;
        case AADLObject::AADLObjectType::AADLIface:
            file.setExpectedObjectCount(t, 2);
            break;
        case AADLObject::AADLObjectType::AADLConnection:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlFileMock XmlFileMock::createSingleConnectionValidMultipoint()
{
    XmlFileMock file(QStringLiteral("<InterfaceView>\n \
                                    <Function name=\"Function_1\" language=\"\" is_type=\"NO\" instance_of=\"\">\n \
                                        <Property name=\"Taste::coordinates\" value=\"-62300 -36600 -42300 -28600\"/>\n \
                                        <Required_Interface name=\"RI_2\" kind=\"SPORADIC_OPERATION\" period=\"\" wcet=\"\" queue_size=\"\">\n \
                                            <Property name=\"Taste::coordinates\" value=\"-42300 -33000\"/>\n \
                                            <Property name=\"Taste::labelInheritance\" value=\"true\"/>\n \
                                        </Required_Interface>\n \
                                    </Function>\n \
                                    <Function name=\"Function_2\" language=\"\" is_type=\"NO\" instance_of=\"\">\n \
                                        <Property name=\"Taste::coordinates\" value=\"-30600 -36600 -10600 -28600\"/>\n \
                                        <Provided_Interface name=\"PI_2\" kind=\"SPORADIC_OPERATION\" period=\"\" wcet=\"\" queue_size=\"\">\n \
                                            <Property name=\"Taste::coordinates\" value=\"-30600 -33000\"/>\n \
                                        </Provided_Interface>\n \
                                    </Function>\n \
                                    <Connection>\n \
                                        <Source func_name=\"Function_1\" ri_name=\"RI_2\"/>\n \
                                        <Target func_name=\"Function_2\" pi_name=\"PI_2\"/>\n \
                                        <Property name=\"Taste::coordinates\" value=\"-30600 -33000 -30800 -33000 -32900 -37000 -32600 -44100 -24400 -43000 -16300 -42300 -8100 -46800 -1200 -41900 2700 -37300 4300 -29500 4200 -25300 3800 -23100 300 -19600 -1400 -18000 -7200 -15300 -13900 -13700 -22300 -12200 -34100 -11500 -47400 -12200 -58300 -14000 -64500 -15500 -80100 -18600 -85200 -21600 -86800 -28900 -85200 -35600 -81600 -41400 -79200 -44300 -76000 -47300 -72800 -49000 -66800 -48900 -62600 -48700 -56100 -47900 -51300 -47800 -44000 -47200 -43100 -46400 -41300 -44400 -39900 -42800 -38800 -40700 -38100 -33000 -42300 -33000\"/>\n \
                                    </Connection>\n \
                                </InterfaceView>"),
                     0, true);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunction:
            file.setExpectedObjectCount(t, 2);
            break;
        case AADLObject::AADLObjectType::AADLIface:
            file.setExpectedObjectCount(t, 2);
            break;
        case AADLObject::AADLObjectType::AADLConnection:
            file.setExpectedObjectCount(t, 1);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlHelper *XmlHelper::m_instance = nullptr;
XmlHelper *XmlHelper::instance()
{
    if (!m_instance) {
        m_instance = new XmlHelper();
    }
    return m_instance;
}

XmlFileMock XmlFileMock::createAllItems()
{
    QString content;

    QFile datFile(":/data/allitems.xml");
    if (datFile.open(QIODevice::ReadOnly | QIODevice::Text))
        content = QString::fromUtf8(datFile.readAll());
    else {
        qWarning() << "Failed loading test data file:" << datFile.fileName() << datFile.errorString();
        return XmlFileMock(QString(), 0, false);
    }

    XmlFileMock file(content);

    const QMetaEnum &me = QMetaEnum::fromType<AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i) {
        const AADLObject::AADLObjectType t = static_cast<AADLObject::AADLObjectType>(me.value(i));
        switch (t) {
        case AADLObject::AADLObjectType::AADLFunction:
            file.setExpectedObjectCount(t, 9);
            break;
        case AADLObject::AADLObjectType::AADLIface:
            file.setExpectedObjectCount(t, 27);
            break;
        case AADLObject::AADLObjectType::AADLConnection:
            file.setExpectedObjectCount(t, 13);
            break;
        case AADLObject::AADLObjectType::AADLComment:
            file.setExpectedObjectCount(t, 2);
            break;

        default:
            file.setExpectedObjectCount(t, 0);
            break;
        }
    }

    return file;
}

XmlHelper::XmlHelper()
    : EmptyFile(XmlFileMock::createEmptyFile())
    , EmptyDoc(XmlFileMock::createEmptyDoc())
    , SingleFunction(XmlFileMock::createSingleFunction())
    , SingleFunctionType(XmlFileMock::createSingleFunctionType())
    , SingleComment(XmlFileMock::createSingleComment())
    , SingleIfaceInvalid(XmlFileMock::createSingleIfaceInvalid())
    , SingleIfaceValid(XmlFileMock::createSingleIfaceValid())
    , ParametrizedIfaces(XmlFileMock::createParametrizedIfaces())
    , SingleConnectionOrphan(XmlFileMock::createSingleConnectionOrphan())
    , SingleConnectionValid(XmlFileMock::createSingleConnectionValid())
    , SingleConnectionValidMultipoint(XmlFileMock::createSingleConnectionValidMultipoint())
    , AllItems(XmlFileMock::createAllItems())
{
}
