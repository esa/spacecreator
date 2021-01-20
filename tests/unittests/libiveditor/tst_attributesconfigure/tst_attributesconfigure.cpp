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

#include "iveditor.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

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

private:
    aadl::PropertyTemplateConfig *m_dynPropConfig;
};

void tst_AttributesConfigure::initTestCase()
{
    aadlinterface::initIvEditor();
    m_dynPropConfig = aadl::PropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
}

void tst_AttributesConfigure::tst_attributesLoad()
{
    QFile file(m_dynPropConfig->configPath());
    QVERIFY2(file.open(QIODevice::ReadOnly | QIODevice::Text), file.errorString().toUtf8().data());
    QString errMsg;
    int line;
    int column;
    aadl::PropertyTemplateConfig::parseAttributesList(QString::fromUtf8(file.readAll()), &errMsg, &line, &column);
    QVERIFY2(errMsg.isEmpty(), QStringLiteral("ERROR [%1:%2]: %3").arg(line).arg(column).arg(errMsg).toUtf8().data());
}

void tst_AttributesConfigure::tst_loadImpl()
{
    aadl::PropertyTemplate dp;
    dp.setName(QLatin1String("test_name"));
    dp.setInfo(aadl::PropertyTemplate::Info::Property);
    dp.setType(aadl::PropertyTemplate::Type::Enumeration);
    const auto scope = aadl::PropertyTemplate::Scope::Provided_Interface;
    dp.setScope(scope);
    dp.setVisible(false);
    dp.setValuesList({ QVariant::fromValue(QString("value1")), QVariant::fromValue(QString("value2")) });
    dp.setValueValidatorPattern(QString("[\\d+]"));
    dp.setAttrValidatorPattern(QMap<aadl::PropertyTemplate::Scope, QPair<QString, QString>> {
            { scope, qMakePair(QString("attrName"), QString("value")) } });

    QDomDocument doc;
    doc.appendChild(dp.toXml(&doc));

    const auto element = doc.documentElement();
    auto propPtr = aadl::PropertyTemplate::fromXml(element);
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
