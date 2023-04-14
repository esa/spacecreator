/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "iveditattributesmodel.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"

#include <QObject>
#include <QtTest>

static const QString kAttrTemplate = R"(
        <Attrs version="1.0">
            <Attr label="Name" name="name">
                <Type>
                    <String validator="[a-zA-Z][a-zA-Z0-9_]*"/>
                </Type>
            </Attr>
            <Attr label="Kind" name="kind">
                <Type>
                    <Enumeration defaultValue="Sporadic">
                        <Entry value="Any"/>
                        <Entry value="Cyclic"/>
                        <Entry value="Sporadic"/>
                        <Entry value="Protected"/>
                        <Entry value="Unprotected"/>
                    </Enumeration>
                </Type>
                <Scopes>
                    <Required_Interface/>
                    <Provided_Interface/>
                </Scopes>
            </Attr>
            <Attr name="origin" editable="false" optional="true">
                <Type>
                    <String validator="\{?[0-9a-fA-F]{8}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{12}\}?"/>
                </Type>
            </Attr>
            <Attr name="is_type">
                <Type>
                    <Enumeration defaultValue="NO">
                        <Entry value="YES"/>
                        <Entry value="NO"/>
                    </Enumeration>
                </Type>
                <Scopes>
                    <Function/>
                </Scopes>
            </Attr>
            <Attr label="Instance of" name="instance_of">
                <Type>
                    <String validator="[a-zA-Z][a-zA-Z0-9_]*"/>
                </Type>
                <Scopes>
                    <Function>
                        <AttrValidator name="is_type" value="NO"/>
                    </Function>
                </Scopes>
            </Attr>
            <Attr label="Inherit from PI" name="Taste::InheritPI" type="property">
                <Type>
                    <Boolean defaultValue="True"/>
                </Type>
                <Scopes>
                    <Required_Interface/>
                </Scopes>
            </Attr>
            <Attr name="Taste::coordinates" type="property" visible="false" editable="false" export="UI">
                <Type>
                    <String validator="[0-9 ]+"/>
                </Type>
                <Scopes/>
            </Attr>
            <Attr name="Taste::RootCoordinates" type="property" visible="false" editable="false" export="UI">
                <Type>
                    <String validator="[0-9 ]+"/>
                </Type>
                <Scopes/>
            </Attr>
            <Attr name="Taste::Autonamed" type="property" visible="false">
                <Type>
                    <Boolean defaultValue="True"/>
                </Type>
                <Scopes>
                    <Required_Interface/>
                </Scopes>
            </Attr>
            <Attr name="AttrName" export="UI">
                <Type>
                    <String/>
                </Type>
                <Scopes/>
            </Attr>
            <Attr name="Namespace::PropertyName::Type::value" type="property" export="UI">
                <Type>
                    <String/>
                </Type>
                <Scopes/>
            </Attr>
        </Attrs>
    )";

class tst_IVEditAttributesModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanupTestCase();

    void testEmptyFunctions();
    void testEmptyInterfaces();
    void testReadFunctions();
    void testReadInterfaces();
    void testWriteFunctions();
    void testWriteInterfaces();

private:
    ivm::IVPropertyTemplateConfig *m_config {};
    std::unique_ptr<ivm::IVModel> m_ivModel {};
};

void tst_IVEditAttributesModel::initTestCase()
{
    m_config = ivm::IVPropertyTemplateConfig::instance();
    m_config->initFromData(kAttrTemplate);
}

void tst_IVEditAttributesModel::init()
{
    m_ivModel = std::make_unique<ivm::IVModel>(m_config);
}

void tst_IVEditAttributesModel::cleanupTestCase()
{
    ivm::IVPropertyTemplateConfig::cleanup();
    m_ivModel.reset();
}

void tst_IVEditAttributesModel::testEmptyFunctions()
{
    ive::IVEditAttributesModel model(ive::IVEditAttributesModel::Function, m_ivModel.get());
    QCOMPARE(model.rowCount(QModelIndex()), 0);
    QCOMPARE(model.columnCount(QModelIndex()), 0);
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QVariant());
}

void tst_IVEditAttributesModel::testEmptyInterfaces()
{
    ive::IVEditAttributesModel model(ive::IVEditAttributesModel::Interface, m_ivModel.get());
    QCOMPARE(model.rowCount(QModelIndex()), 0);
    QCOMPARE(model.columnCount(QModelIndex()), 0);
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QVariant());
}

void tst_IVEditAttributesModel::testReadFunctions()
{
    auto function1 = ivm::testutils::createFunction("Function 1");
    auto function2 = ivm::testutils::createFunction("Function 2");

    m_ivModel->addObjects<ivm::IVObject *>({function1, function2});

    ive::IVEditAttributesModel model(ive::IVEditAttributesModel::Function, m_ivModel.get());
    QCOMPARE(model.rowCount(QModelIndex()), 2);
    QCOMPARE(model.columnCount(QModelIndex()), 2);

    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QLatin1String("Name"));
    QCOMPARE(model.headerData(1, Qt::Horizontal, Qt::DisplayRole), QLatin1String("Instance of"));

    // The order of the rows is randomized due to the hash storage, so use a set
    QSet<QString> obtainedNames;
    obtainedNames << model.data(model.index(0, 0), Qt::DisplayRole).toString();
    obtainedNames << model.data(model.index(1, 0), Qt::DisplayRole).toString();
    QSet<QString> expectedNames({QLatin1String("Function 1"), QLatin1String("Function 2")});
    QCOMPARE(obtainedNames, expectedNames);
}

void tst_IVEditAttributesModel::testReadInterfaces()
{
    auto function1 = ivm::testutils::createFunction("Function 1");
    auto function2 = ivm::testutils::createFunction("Function 2");
    auto interface1 = ivm::testutils::createProvidedIface(function1, "Interface 1");
    auto interface2 = ivm::testutils::createProvidedIface(function2, "Interface 2");

    m_ivModel->addObjects<ivm::IVObject *>({interface1, interface2});

    ive::IVEditAttributesModel model(ive::IVEditAttributesModel::Interface, m_ivModel.get());
    QCOMPARE(model.rowCount(QModelIndex()), 2);

    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("Function"));
    QCOMPARE(model.headerData(1, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("Name"));
    QCOMPARE(model.headerData(2, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("Kind"));
}

void tst_IVEditAttributesModel::testWriteFunctions()
{
    auto function = ivm::testutils::createFunction("Function");
    auto type = ivm::testutils::createFunctionType("Type");

    m_ivModel->addObjects<ivm::IVObject *>({function, type});

    shared::cmd::CommandsStackBase stack;
    shared::cmd::CommandsStackBase::Macro macro(&stack, "test");
    ive::IVEditAttributesModel model(ive::IVEditAttributesModel::Function, m_ivModel.get(), &macro);

    QVERIFY(model.setData(model.index(0, 0), QVariant("Renamed"), ive::IVEditAttributesModel::EditRole));
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole).toString(), QLatin1String("Renamed"));

    QVERIFY(model.setData(model.index(0, 1), QVariant("Type"), ive::IVEditAttributesModel::EditRole));
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole).toString(), QLatin1String("Type"));

    // An invalid option should not come from the delegate, but check that the
    // model rejects it anyway.
    QVERIFY(!model.setData(model.index(0, 1), QVariant("Invalid"), ive::IVEditAttributesModel::EditRole));
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole).toString(), QLatin1String("Type"));
}

void tst_IVEditAttributesModel::testWriteInterfaces()
{
    auto function = ivm::testutils::createFunction("Function 1");
    auto interface = ivm::testutils::createProvidedIface(function, "Interface 1");

    m_ivModel->addObjects<ivm::IVObject *>({interface});

    shared::cmd::CommandsStackBase stack;
    shared::cmd::CommandsStackBase::Macro macro(&stack, "test");
    ive::IVEditAttributesModel model(ive::IVEditAttributesModel::Interface, m_ivModel.get(), &macro);

    QCOMPARE(model.headerData(1, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("Name"));

    QVERIFY(model.setData(model.index(0, 1), QVariant("Renamed"), ive::IVEditAttributesModel::EditRole));
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole).toString(), QLatin1String("Renamed"));
}


QTEST_MAIN(tst_IVEditAttributesModel)

#include "tst_iveditattributesmodel.moc"
