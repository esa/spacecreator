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

class tst_IVEditAttributesModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanupTestCase();

    void testEmpty();
    void testReadFunctions();
    void testReadInterfaces();

private:
    ivm::IVPropertyTemplateConfig *m_config {};
    std::unique_ptr<ivm::IVModel> m_ivModel {};
};

void tst_IVEditAttributesModel::initTestCase()
{
    m_config = ivm::IVPropertyTemplateConfig::instance();
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

void tst_IVEditAttributesModel::testEmpty()
{
    ive::IVEditAttributesModel model(m_ivModel.get(), ive::IVEditAttributesModel::Function);
    QCOMPARE(model.rowCount(QModelIndex()), 0);
    QCOMPARE(model.columnCount(QModelIndex()), 0);
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QVariant());
}

void tst_IVEditAttributesModel::testReadFunctions()
{
    auto function1 = ivm::testutils::createFunction("Function 1");
    auto function2 = ivm::testutils::createFunction("Function 2");

    m_ivModel->addObjects<ivm::IVObject *>({function1, function2});

    {
        ive::IVEditAttributesModel model(m_ivModel.get(), ive::IVEditAttributesModel::Function);
        QCOMPARE(model.rowCount(QModelIndex()), 2);
        QCOMPARE(model.columnCount(QModelIndex()), 1); // The name attribute is always there.

        QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QString("name"));
    }

    function1->setEntityAttribute("example", "value 1");
    function2->setEntityAttribute("example", "value 2");

    {
        ive::IVEditAttributesModel model(m_ivModel.get(), ive::IVEditAttributesModel::Function);
        QCOMPARE(model.rowCount(QModelIndex()), 2);
        QCOMPARE(model.columnCount(QModelIndex()), 2); // Name attribute plus the one added.

        QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QString("name"));
        QCOMPARE(model.headerData(1, Qt::Horizontal, Qt::DisplayRole), QString("example"));
    }

}

void tst_IVEditAttributesModel::testReadInterfaces()
{
    {
        ive::IVEditAttributesModel model(m_ivModel.get(), ive::IVEditAttributesModel::Interface);
        QCOMPARE(model.rowCount(QModelIndex()), 0);
        QCOMPARE(model.columnCount(QModelIndex()), 0);

        QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QVariant());
    }

    auto function1 = ivm::testutils::createFunctionType("Function 1");
    auto function2 = ivm::testutils::createFunctionType("Function 2");
    auto interface1 = ivm::testutils::createRequiredIface(function1, "Interface 1");
    auto interface2 = ivm::testutils::createRequiredIface(function2, "Interface 2");

    m_ivModel->addObjects<ivm::IVObject *>({interface1, interface2});

    {
        ive::IVEditAttributesModel model(m_ivModel.get(), ive::IVEditAttributesModel::Interface);
        QCOMPARE(model.rowCount(QModelIndex()), 2);
        QCOMPARE(model.columnCount(QModelIndex()), 3); // name, layer, kind

        QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DisplayRole), QString("name"));
    }

}


QTEST_MAIN(tst_IVEditAttributesModel)

#include "tst_iveditattributesmodel.moc"
