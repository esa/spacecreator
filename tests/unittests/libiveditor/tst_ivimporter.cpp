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

#include "interfacedocument.h"
#include "iveditor.h"
#include "ivimporter.h"
#include "ivlibrary.h"
#include "ivmodel.h"

#include <QApplication>
#include <QClipboard>
#include <QGraphicsView>
#include <QtTest>
#include <veobject.h>

class tst_IVImporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testPasteItemTwice();

private:
    std::unique_ptr<ive::InterfaceDocument> m_doc;
    std::unique_ptr<QGraphicsView> m_graphicsView;
    std::unique_ptr<ive::IVImporter> m_importer;
};

void tst_IVImporter::initTestCase()
{
    ive::initIVEditor();
    ivm::initIVLibrary();
}

void tst_IVImporter::init()
{
    m_doc = std::make_unique<ive::InterfaceDocument>();
    m_graphicsView = std::make_unique<QGraphicsView>();
    m_importer = std::make_unique<ive::IVImporter>(m_doc.get(), m_graphicsView.get());
}

void tst_IVImporter::cleanup() { }

void tst_IVImporter::testPasteItemTwice()
{
    QCOMPARE(m_doc->objects().size(), 0);
    const QString xml = R"(<InterfaceView version="1.3" modifierHash="f5069713f">
            <Function id="{9ea53945-5904-49d1-87ea-9d28cbe8062f}" name="FManager" is_type="NO" language="SDL"
                default_implementation="default">
                <Implementations><Implementation name="default" language="SDL" /></Implementations>
            </Function>
            <Layer name="default" is_visible="true" />
        </InterfaceView>)";
    qApp->clipboard()->setText(xml);

    m_importer->pasteItems();
    QCOMPARE(m_doc->objects().size(), 1);

    // raises an error as name is duplicated - not imported
    m_importer->pasteItems();
    QCOMPARE(m_doc->objects().size(), 1);

    // rename object, so import can be done
    shared::VEObject *obj = m_doc->objectsModel()->getObjectByName("FManager");
    QCOMPARE_NE(obj, nullptr);
    obj->setEntityAttribute("name", "FReceiver");
    m_importer->pasteItems();
    QCOMPARE(m_doc->objects().size(), 2);
}

QTEST_MAIN(tst_IVImporter)

#include "tst_ivimporter.moc"
