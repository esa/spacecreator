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

#include "aadlcommonprops.h"
#include "aadliface.h"
#include "baseitems/common/aadlutils.h"
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
    void tst_systemAttrs();

private:
    ivm::PropertyTemplateConfig *m_dynPropConfig;
};

void tst_AttributesConfigure::initTestCase()
{
    ive::initIvEditor();
    QStandardPaths::setTestModeEnabled(true);
    m_dynPropConfig = ivm::PropertyTemplateConfig::instance();
    m_dynPropConfig->init(ive::dynamicPropertiesFilePath());
}

void tst_AttributesConfigure::tst_attributesLoad()
{
    QFile file(m_dynPropConfig->configPath());
    QVERIFY2(file.open(QIODevice::ReadOnly | QIODevice::Text), file.errorString().toUtf8().data());
    QString errMsg;
    int line;
    int column;
    ivm::PropertyTemplateConfig::parseAttributesList(QString::fromUtf8(file.readAll()), &errMsg, &line, &column);
    QVERIFY2(errMsg.isEmpty(), QStringLiteral("ERROR [%1:%2]: %3").arg(line).arg(column).arg(errMsg).toUtf8().data());
}

void tst_AttributesConfigure::tst_loadImpl()
{
    ivm::PropertyTemplate dp;
    dp.setName(QLatin1String("test_name"));
    dp.setInfo(ivm::PropertyTemplate::Info::Property);
    dp.setType(ivm::PropertyTemplate::Type::Enumeration);
    const auto scope = ivm::PropertyTemplate::Scope::Provided_Interface;
    dp.setScope(scope);
    dp.setVisible(false);
    dp.setValuesList({ QVariant::fromValue(QString("value1")), QVariant::fromValue(QString("value2")) });
    dp.setValueValidatorPattern(QString("[\\d+]"));
    dp.setAttrValidatorPattern(QMap<ivm::PropertyTemplate::Scope, QPair<QString, QString>> {
            { scope, qMakePair(QString("attrName"), QString("value")) } });

    QDomDocument doc;
    doc.appendChild(dp.toXml(&doc));

    const auto element = doc.documentElement();
    auto propPtr = ivm::PropertyTemplate::fromXml(element);
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

void tst_AttributesConfigure::tst_systemAttrs()
{
    const auto sysAttrs = m_dynPropConfig->systemAttributes();
    auto it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::name)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    QVERIFY(it != sysAttrs.cend());
    if (it != sysAttrs.cend()) {
        QVERIFY((*it)->scope().testFlag(ivm::PropertyTemplate::Scope::All));
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Attribute);
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::String);
        const QRegularExpression rx((*it)->valueValidatorPattern());
        QString title = QLatin1String("name-1");
        QRegularExpressionMatch match = rx.match(title);
        QVERIFY(match.capturedLength() != title.length());
        title = QLatin1String("name 1");
        match = rx.match(title);
        QVERIFY(match.capturedLength() != title.length());
        title = QLatin1String("name_1");
        match = rx.match(title);
        QVERIFY(match.capturedLength() == title.length());
        title = QLatin1String("1_name");
        match = rx.match(title);
        QVERIFY(match.capturedLength() != title.length());
    }
    it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::kind)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    if (it != sysAttrs.cend()) {
        QVERIFY(int((*it)->scope())
                == int(ivm::PropertyTemplate::Scope::Provided_Interface
                           | ivm::PropertyTemplate::Scope::Required_Interface));
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::Enumeration);
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Attribute);

        const QMetaEnum &me = QMetaEnum::fromType<ivm::AADLIface::OperationKind>();
        for (int i = 0; i < me.keyCount(); ++i) {
            QVERIFY((*it)->valuesList().contains(qVariantFromValue(QString::fromLatin1(me.key(i)))));
        }
    }

    it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::instance_of)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    QVERIFY(it != sysAttrs.cend());
    if (it != sysAttrs.cend()) {
        QVERIFY(int((*it)->scope()) == int(ivm::PropertyTemplate::Scope::Function));
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Attribute);
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::String);
        const QRegularExpression rx((*it)->valueValidatorPattern());
        QString title = QLatin1String("name-1");
        QRegularExpressionMatch match = rx.match(title);
        QVERIFY(match.capturedLength() != title.length());
        title = QLatin1String("name 1");
        match = rx.match(title);
        QVERIFY(match.capturedLength() != title.length());
        title = QLatin1String("name_1");
        match = rx.match(title);
        QVERIFY(match.capturedLength() == title.length());
        title = QLatin1String("1_name");
        match = rx.match(title);
        QVERIFY(match.capturedLength() != title.length());
    }

    it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::is_type)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    if (it != sysAttrs.cend()) {
        QVERIFY(int((*it)->scope()) == int(ivm::PropertyTemplate::Scope::Function));
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::Enumeration);
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Attribute);
        QVERIFY((*it)->valuesList().contains(QLatin1String("YES"))
                && (*it)->valuesList().contains(QLatin1String("NO")));
    }

    it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::InheritPI)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    if (it != sysAttrs.cend()) {
        QVERIFY(int((*it)->scope()) == int(ivm::PropertyTemplate::Scope::Required_Interface));
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::Boolean);
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Property);
    }

    it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::Autonamed)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    if (it != sysAttrs.cend()) {
        QVERIFY(int((*it)->scope()) == int(ivm::PropertyTemplate::Scope::Required_Interface));
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::Boolean);
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Property);
    }

    it = std::find_if(sysAttrs.cbegin(), sysAttrs.cend(),
            [title = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates)](
                    const ivm::PropertyTemplate *namePropTemplate) { return namePropTemplate->name() == title; });
    QVERIFY(it != sysAttrs.cend());
    if (it != sysAttrs.cend()) {
        QVERIFY(int((*it)->scope()) == int(ivm::PropertyTemplate::Scope::All));
        QVERIFY((*it)->info() == ivm::PropertyTemplate::Info::Property);
        QVERIFY((*it)->type() == ivm::PropertyTemplate::Type::String);
        const QRegularExpression rx((*it)->valueValidatorPattern());
        QString coordinates = QLatin1String("123 456 789 000");
        QRegularExpressionMatch match = rx.match(coordinates);
        QVERIFY(match.capturedLength() == coordinates.length());
        coordinates = QLatin1String("ABC 123");
        match = rx.match(coordinates);
        QVERIFY(match.capturedLength() != coordinates.length());
        coordinates = QLatin1String("ABC_1");
        match = rx.match(coordinates);
        QVERIFY(match.capturedLength() != coordinates.length());
        coordinates = QLatin1String("1_ABC");
        match = rx.match(coordinates);
        QVERIFY(match.capturedLength() != coordinates.length());
        coordinates = QLatin1String("1_ABC");
        match = rx.match(coordinates);
        QVERIFY(match.capturedLength() != coordinates.length());
        coordinates = QLatin1String("123 ABD 789 QAZ");
        match = rx.match(coordinates);
        QVERIFY(match.capturedLength() != coordinates.length());
    }
}

QTEST_MAIN(tst_AttributesConfigure)

#include "tst_attributesconfigure.moc"
