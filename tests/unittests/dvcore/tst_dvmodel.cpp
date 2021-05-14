/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvmodel.h"
#include "dvnode.h"

#include <QScopedPointer>
#include <QtTest>

class tst_DVModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void testAddObject();
    void testAddObjects();
    void testRemoveObject();
    void testClear();
    void testInitFromObjects();

private:
    QScopedPointer<dvm::DVModel> m_model;
};

void tst_DVModel::init()
{
    m_model.reset(new dvm::DVModel());
}

void tst_DVModel::testAddObject()
{
    QCOMPARE(m_model->visibleObjects().size(), 0);

    auto obj = new dvm::DVNode();
    bool ok = m_model->addObject(obj);
    QCOMPARE(ok, true);
    QCOMPARE(m_model->visibleObjects().size(), 1);
    QCOMPARE(m_model->getObject(obj->id()), obj);

    // Do not add the same object twice
    ok = m_model->addObject(obj);
    QCOMPARE(ok, false);
    QCOMPARE(m_model->visibleObjects().size(), 1);
}

void tst_DVModel::testAddObjects()
{
    m_model->addObjects({ new dvm::DVNode(), new dvm::DVNode() });
    QCOMPARE(m_model->visibleObjects().size(), 2);

    m_model->addObjects({ new dvm::DVNode(), new dvm::DVNode(), new dvm::DVNode() });
    QCOMPARE(m_model->visibleObjects().size(), 5);
}

void tst_DVModel::testRemoveObject()
{
    auto obj = new dvm::DVNode();

    bool ok = m_model->removeObject(obj);
    QCOMPARE(ok, false);

    m_model->addObject(obj);
    QCOMPARE(m_model->getObject(obj->id()), obj);

    ok = m_model->removeObject(obj);
    QCOMPARE(ok, true);
    QCOMPARE(m_model->getObject(obj->id()), nullptr);
}

void tst_DVModel::testClear()
{
    auto obj = new dvm::DVNode();
    m_model->addObject(obj);
    m_model->setRootObject(obj->id());
    QCOMPARE(m_model->visibleObjects().size(), 1);
    QCOMPARE(m_model->rootObject(), obj);

    m_model->clear();
    QCOMPARE(m_model->visibleObjects().size(), 0);
    QCOMPARE(m_model->rootObject(), nullptr);
}

void tst_DVModel::testInitFromObjects()
{
    auto obj = new dvm::DVNode();
    m_model->addObject(obj);
    m_model->setRootObject(obj->id());

    m_model->initFromObjects({ new dvm::DVNode(), new dvm::DVNode() });
    QCOMPARE(m_model->visibleObjects().size(), 2);
    QCOMPARE(m_model->rootObject(), nullptr);
}

QTEST_APPLESS_MAIN(tst_DVModel)

#include "tst_dvmodel.moc"
