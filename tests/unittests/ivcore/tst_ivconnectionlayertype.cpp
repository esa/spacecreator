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

#include "ivcommonprops.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"

#include <QStandardPaths>
#include <QTest>

class tst_IVConnectionLayerType : public QObject
{
    Q_OBJECT

private:
    ivm::IVPropertyTemplateConfig *cfg { nullptr };

private Q_SLOTS:
    void initTestCase();
    void tst_layerRenaming();
    void tst_interfaceLayerName();
};

void tst_IVConnectionLayerType::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
}

void tst_IVConnectionLayerType::tst_layerRenaming()
{
    const QString nameAttr = ivm::meta::Props::token(ivm::meta::Props::Token::name);
    const QString customLayerName = QString("SomeNameForLayer");
    std::unique_ptr<ivm::IVConnectionLayerType> layer = std::make_unique<ivm::IVConnectionLayerType>();
    layer->rename(customLayerName);

    QVERIFY(layer->hasEntityAttribute(nameAttr));
    QCOMPARE(layer->entityAttributeValue(nameAttr).toString(), customLayerName);
    QCOMPARE(layer->title(), customLayerName);
}

void tst_IVConnectionLayerType::tst_interfaceLayerName()
{
    const QString layerNameAttr = ivm::meta::Props::token(ivm::meta::Props::Token::layer);
    const QString customLayerName = QString("SomeNameForInterfaceLayer");

    std::unique_ptr<ivm::IVFunction> function1(ivm::testutils::createFunction("Fn1"));
    std::unique_ptr<ivm::IVInterface> requiredInterfaceFunction1(
            ivm::testutils::createIface(function1.get(), ivm::IVInterface::InterfaceType::Required, "RI1"));

    QVERIFY(requiredInterfaceFunction1->hasEntityAttribute(layerNameAttr));
    QCOMPARE(requiredInterfaceFunction1->entityAttributeValue(layerNameAttr).toString(),
            ivm::IVConnectionLayerType::DefaultLayerName);
    QCOMPARE(requiredInterfaceFunction1->layerName(), ivm::IVConnectionLayerType::DefaultLayerName);

    requiredInterfaceFunction1->setLayerName(customLayerName);

    QVERIFY(requiredInterfaceFunction1->hasEntityAttribute(layerNameAttr));
    QCOMPARE(requiredInterfaceFunction1->entityAttributeValue(layerNameAttr).toString(), customLayerName);
    QCOMPARE(requiredInterfaceFunction1->layerName(), customLayerName);
}

QTEST_APPLESS_MAIN(tst_IVConnectionLayerType)

#include "tst_ivconnectionlayertype.moc"
