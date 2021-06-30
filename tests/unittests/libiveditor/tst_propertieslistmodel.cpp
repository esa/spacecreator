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

#include "baseitems/common/ivutils.h"
#include "commandsstack.h"
#include "interface/properties/ivpropertieslistmodel.h"
#include "ivfunction.h"
#include "ivpropertytemplateconfig.h"

#include <QObject>
#include <QtTest>

class tst_PropertiesListModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void tst_object();

private:
    ive::cmd::CommandsStack::Macro *m_macro { nullptr };
    ivm::IVPropertyTemplateConfig *m_config { nullptr };
};

void tst_PropertiesListModel::initTestCase()
{
    m_macro = new ive::cmd::CommandsStack::Macro(new QUndoStack);
    m_config = ivm::IVPropertyTemplateConfig::instance();
    m_config->init(ive::dynamicPropertiesFilePath());
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

QTEST_MAIN(tst_PropertiesListModel)

#include "tst_propertieslistmodel.moc"
