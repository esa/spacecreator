/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstack.h"
#include "itemeditor/common/ivutils.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "properties/ivpropertieslistmodel.h"

#include <QObject>
#include <QtTest>

class tst_PropertiesListModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void tst_object();
    void tst_attributes();

private:
    ive::cmd::CommandsStack::Macro *m_macro { nullptr };
    ivm::IVPropertyTemplateConfig *m_config { nullptr };
};

void tst_PropertiesListModel::initTestCase()
{
    ivm::initIVLibrary();
    m_macro = new ive::cmd::CommandsStack::Macro(new ive::cmd::CommandsStack);
    m_config = ivm::IVPropertyTemplateConfig::instance();
    m_config->init(shared::interfaceCustomAttributesFilePath());
}

void tst_PropertiesListModel::tst_object()
{
    const QHash<QString, EntityAttribute> attrs {
        { QLatin1String("Attr1"),
                EntityAttribute {
                        QLatin1String("Attr1"), QStringLiteral("value_1"), EntityAttribute::Type::Attribute } },
        { QLatin1String("Attr2"),
                EntityAttribute {
                        QLatin1String("Attr2"), QStringLiteral("value_2"), EntityAttribute::Type::Attribute } },
        { QLatin1String("Attr3"),
                EntityAttribute {
                        QLatin1String("Attr3"), QStringLiteral("value_3"), EntityAttribute::Type::Attribute } },
        { QLatin1String("Attr4"),
                EntityAttribute {
                        QLatin1String("Attr4"), QStringLiteral("value_4"), EntityAttribute::Type::Attribute } },
        { QLatin1String("Attr5"),
                EntityAttribute {
                        QLatin1String("Attr5"), QStringLiteral("value_5"), EntityAttribute::Type::Attribute } },
    };

    ivm::IVFunction fn;
    fn.setEntityAttributes(attrs);

    ive::IVPropertiesListModel plmodel(m_macro, m_config);
    plmodel.setDataObject(&fn);

    QVERIFY(!plmodel.createProperty(QLatin1String("Attr1")));

    const QString attrName = QLatin1String("Attr6");
    QVERIFY(plmodel.createProperty(attrName));

    const QModelIndexList indexes = plmodel.match(plmodel.index(0, 0), 0, attrName);
    QVERIFY(!indexes.isEmpty());
    for (int idx = 0; idx < indexes.size(); ++idx) {
        QVERIFY(plmodel.removeProperty(indexes.at(idx)));
    }
}

void tst_PropertiesListModel::tst_attributes()
{
    ivm::IVModel model(m_config);

    ivm::IVFunction fn;
    fn.setTitle("Function_1");
    model.addObject(&fn);

    ivm::IVInterface::CreationInfo ci;
    ci.function = &fn;
    ci.name = "Interface_1";
    ci.type = ivm::IVInterface::InterfaceType::Provided;
    ivm::IVInterfaceProvided iface(ci);
    fn.addChild(&iface);
    model.addObject(&iface);

    ive::IVPropertiesListModel plmodel(m_macro, m_config);
    plmodel.setDataObject(&iface);

    const QString attrName = QLatin1String("kind");
    QVERIFY(iface.hasEntityAttribute(attrName));

    const QList<shared::PropertyTemplate *> propTemplates = m_config->propertyTemplatesForObject(&iface);
    auto it = std::find_if(
            propTemplates.cbegin(), propTemplates.cend(), [attrName](shared::PropertyTemplate *propTemplate) {
                for (const QPair<QString, QString> &attrData : propTemplate->attrValidatorPatterns()) {
                    if (attrData.first == attrName && propTemplate->attrValidatorPatterns().size() == 1) {
                        return true;
                    }
                }
                return false;
            });
    if (it != propTemplates.cend()) {
        shared::PropertyTemplate *propTemplate = *it;
        QVERIFY(propTemplate != nullptr);
        auto kindMetaEnum = QMetaEnum::fromType<ivm::IVInterface::OperationKind>();
        const QString validatorValue = propTemplate->attrValidatorPatterns().first().second;
        for (int idx = 1; idx < kindMetaEnum.keyCount(); ++idx) {
            const int enumValue = kindMetaEnum.keyToValue(kindMetaEnum.key(idx));
            iface.setKind(static_cast<ivm::IVInterface::OperationKind>(enumValue));
            QVERIFY(propTemplate->validate(&iface) == (kindMetaEnum.valueToKey(enumValue) == validatorValue));
        }
    }
}

QTEST_MAIN(tst_PropertiesListModel)

#include "tst_propertieslistmodel.moc"
