/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "archetypes/archetypeintegrityhelper.h"
#include "archetypes/archetypelibrary.h"
#include "archetypes/archetypemodel.h"
#include "archetypes/archetypeobject.h"
#include "archetypes/archetypexmlreader.h"
#include "archetypes/functionarchetype.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"
#include "ivxmlreader.h"
#include "parameter.h"

#include <QPointer>
#include <QStandardPaths>
#include <QTest>
#include <QVector>

class tst_ArchtypeIntegrityHelper : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void tst_checkArchetypeIntegrity();
    void tst_checkArchetypeIntegrityWithInterfaceNameError();
    void tst_checkArchetypeIntegrityWithParameterNameError();

private:
    void setParameterType(ivm::IVInterface *interface, QString parameterName, QString newParameterType);

private:
    ivm::IVPropertyTemplateConfig *conf { nullptr };
    QPointer<ivm::ArchetypeModel> archetypeModel;
    QPointer<ivm::IVModel> model;
};

void tst_ArchtypeIntegrityHelper::initTestCase()
{
    ivm::initIVLibrary();
    conf = ivm::IVPropertyTemplateConfig::instance();
    conf->init(QLatin1String("default_attrinbutes.xml"));

    archetypeModel = new ivm::ArchetypeModel();
    ivm::ArchetypeXMLReader archetypeReader;
    QVERIFY(archetypeReader.readFile(QFINDTESTDATA("archetype_library.xml")));
    auto archetypeObjects = archetypeReader.parsedObjects();
    ivm::ArchetypeLibrary *archetypelibrary = new ivm::ArchetypeLibrary("Lib1");
    for (auto archetypeObject : archetypeObjects) {
        if (auto functionArchetype = archetypeObject->as<ivm::FunctionArchetype *>()) {
            archetypelibrary->addFunction(functionArchetype);
        }
    }
    archetypeObjects.append(archetypelibrary);
    archetypeModel->initFromObjects(archetypeObjects);
    QCOMPARE(archetypeModel->objects().size(), 11);

    model = new ivm::IVModel(conf);
    ivm::IVXMLReader reader;
    QVERIFY(reader.readFile(QFINDTESTDATA("iv_archetypes.xml")));
    model->initFromObjects(reader.parsedObjects());
    QCOMPARE(model->objects().size(), 4);
}

void tst_ArchtypeIntegrityHelper::tst_checkArchetypeIntegrity()
{
    QStringList warningList =
            ivm::ArchetypeIntegrityHelper::checkArchetypeIntegrity(model->objects().values(), archetypeModel);

    QCOMPARE(warningList.size(), 0);
}

void tst_ArchtypeIntegrityHelper::tst_checkArchetypeIntegrityWithInterfaceNameError()
{
    ivm::IVInterface *interface = model->getIfaceByName("Interface1", ivm::IVInterface::InterfaceType::Provided);
    QVERIFY(interface != nullptr);
    interface->setTitle("InterfaceTest");

    QStringList warningList =
            ivm::ArchetypeIntegrityHelper::checkArchetypeIntegrity(model->objects().values(), archetypeModel);

    QCOMPARE(warningList.size(), 1);
    QCOMPARE(warningList[0],
            "The archetype interface ArchFunc1::Interface1 is not implemented in function Function_Type_1");

    interface->setTitle("Interface1");
}

void tst_ArchtypeIntegrityHelper::tst_checkArchetypeIntegrityWithParameterNameError()
{
    ivm::IVInterface *interface = model->getIfaceByName("Interface2", ivm::IVInterface::InterfaceType::Required);
    QVERIFY(interface != nullptr);

    setParameterType(interface, "Parameter22", "T-UInt32");

    QStringList warningList =
            ivm::ArchetypeIntegrityHelper::checkArchetypeIntegrity(model->objects().values(), archetypeModel);

    QCOMPARE(warningList.size(), 1);
    QCOMPARE(warningList[0],
            "The archetype interface ArchFunc1::Interface2 is not implemented in function Function_Type_1");

    setParameterType(interface, "Parameter22", "T-Int8");
}

void tst_ArchtypeIntegrityHelper::setParameterType(
        ivm::IVInterface *interface, QString parameterName, QString newParameterType)
{
    if (interface == nullptr) {
        return;
    }

    QVector<shared::InterfaceParameter> parameters = interface->params();
    QVector<shared::InterfaceParameter> newParameters;
    for (auto param : parameters) {
        if (param.name() == parameterName) {
            param.setParamTypeName(newParameterType);
        }
        newParameters.append(param);
    }

    interface->setParams(newParameters);
}

QTEST_APPLESS_MAIN(tst_ArchtypeIntegrityHelper)

#include "tst_archetypeintegrityhelper.moc"
