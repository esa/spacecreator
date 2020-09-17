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

#include "interface/properties/dynamicproperty.h"
#include "interface/properties/dynamicpropertyconfig.h"
#include "iveditor.h"

#include <QDomDocument>
#include <QMetaEnum>
#include <QObject>
#include <QtTest>

class tst_AttributesConfigure : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void tst_attributesLoad();
    void tst_loadImpl();
};

void tst_AttributesConfigure::initTestCase()
{
    aadlinterface::initIvEditor();
}

void tst_AttributesConfigure::tst_attributesLoad()
{
    QFile file(aadlinterface::DynamicPropertyConfig::resourceConfigPath());
    QVERIFY2(file.open(QIODevice::ReadOnly | QIODevice::Text), file.errorString().toUtf8().data());
    QString errMsg;
    int line;
    int column;
    aadlinterface::DynamicPropertyConfig::parseAttributesList(
            QString::fromUtf8(file.readAll()), &errMsg, &line, &column);
    QVERIFY2(errMsg.isEmpty(), QStringLiteral("ERROR [%1:%2]: %3").arg(line).arg(column).arg(errMsg).toUtf8().data());
}

void tst_AttributesConfigure::tst_loadImpl()
{
    aadlinterface::DynamicProperty dp;
    dp.setName(QLatin1String("test_name"));
    dp.setInfo(aadlinterface::DynamicProperty::Info::Property);
    dp.setType(aadlinterface::DynamicProperty::Type::Enumeration);
    const auto scope = aadlinterface::DynamicProperty::Scope::Provided_Interface;
    dp.setScope(scope);
    dp.setVisible(false);
    dp.setValuesList({ QVariant::fromValue(QString("value1")), QVariant::fromValue(QString("value2")) });
    dp.setValueValidatorPattern(QString("[\\d+]"));
    dp.setAttrValidatorPattern(QMap<aadlinterface::DynamicProperty::Scope, QPair<QString, QString>> {
            { scope, qMakePair(QString("attrName"), QString("value")) } });

    QDomDocument doc;
    doc.appendChild(dp.toXml(&doc));

    const auto element = doc.documentElement();
    auto propPtr = aadlinterface::DynamicProperty::fromXml(element);
    QVERIFY(propPtr != nullptr);
    QVERIFY(propPtr->name() == dp.name());
    QVERIFY(propPtr->info() == dp.info());
    QVERIFY(propPtr->type() == dp.type());
    QVERIFY(propPtr->scope() == dp.scope());
    QVERIFY(propPtr->isVisible() == dp.isVisible());
    QVERIFY(propPtr->valuesList() == dp.valuesList());
    QVERIFY(propPtr->valueValidatorPattern() == dp.valueValidatorPattern());
    QVERIFY(propPtr->attrValidatorPatterns() == dp.attrValidatorPatterns());
}

QTEST_MAIN(tst_AttributesConfigure)

#include "tst_attributesconfigure.moc"
