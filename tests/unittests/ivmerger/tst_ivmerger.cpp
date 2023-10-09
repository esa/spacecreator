/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"
#include "ivmerger/ivmerger.h"

#include <QtTest>

class tst_IvMerger : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_simpleIvsMerge();
    void test_ivMergeWithReplace();
    void test_ivMergeNestedFunctions();

private:
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;
    const Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
};

void tst_IvMerger::initTestCase()
{
    ivm::initIVLibrary();
    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
}

void tst_IvMerger::test_simpleIvsMerge()
{
    ivm::IVModel targetModel(m_dynPropConfig);

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3");
    ivm::IVFunction *fn4 = ivm::testutils::createFunction("Fn4");

    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::IVConnection *c2 = ivm::testutils::createConnection(fn3, fn4, "C2");

    const QVector<ivm::IVObject *> targetObjects { fn1, fn2, fn3, fn4, c1, c2 };
    targetModel.addObjects(targetObjects);

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn1 = ivm::testutils::createFunction("SourceFn1");
    ivm::IVFunction *sourceFn2 = ivm::testutils::createFunction("SourceFn2");
    ivm::IVFunction *sourceFn3 = ivm::testutils::createFunction("SourceFn3");
    ivm::IVFunction *sourceFn4 = ivm::testutils::createFunction("SourceFn4");

    ivm::IVConnection *sourceC1 = ivm::testutils::createConnection(sourceFn1, sourceFn2, "SourceC1");
    ivm::IVConnection *sourceC2 = ivm::testutils::createConnection(sourceFn3, sourceFn4, "SourceC2");

    const QVector<ivm::IVObject *> sourceObjects { sourceFn1, sourceFn2, sourceFn3, sourceFn4, sourceC1, sourceC2 };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunctionType *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunctionType>();

    QCOMPARE(allTargetFunctions.size(), 8);
    QCOMPARE(targetModel.getFunction("Fn1", m_caseCheck), fn1);
    QCOMPARE(targetModel.getFunction("Fn2", m_caseCheck), fn2);
    QCOMPARE(targetModel.getFunction("Fn3", m_caseCheck), fn3);
    QCOMPARE(targetModel.getFunction("Fn4", m_caseCheck), fn4);
    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck), sourceFn1);
    QCOMPARE(targetModel.getFunction("SourceFn2", m_caseCheck), sourceFn2);
    QCOMPARE(targetModel.getFunction("SourceFn3", m_caseCheck), sourceFn3);
    QCOMPARE(targetModel.getFunction("SourceFn4", m_caseCheck), sourceFn4);
    QCOMPARE(targetModel.getConnection("C1", "Fn1", "Fn2", m_caseCheck), c1);
    QCOMPARE(targetModel.getConnection("C2", "Fn3", "Fn4", m_caseCheck), c2);
    QCOMPARE(targetModel.getConnection("SourceC1", "SourceFn1", "SourceFn2", m_caseCheck), sourceC1);
    QCOMPARE(targetModel.getConnection("SourceC2", "SourceFn3", "SourceFn4", m_caseCheck), sourceC2);
}

void tst_IvMerger::test_ivMergeWithReplace()
{
    ivm::IVModel targetModel(m_dynPropConfig);

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3");
    ivm::IVFunction *fn4 = ivm::testutils::createFunction("Fn4");

    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::IVConnection *c2 = ivm::testutils::createConnection(fn2, fn3, "C2");
    ivm::IVConnection *c3 = ivm::testutils::createConnection(fn3, fn4, "C3");

    const QVector<ivm::IVObject *> targetObjects { fn1, fn2, fn3, fn4, c1, c2, c3 };
    targetModel.addObjects(targetObjects);

    ivm::IVModel sourceModel(m_dynPropConfig);

    const QVector<ivm::IVObject *> sourceObjects { fn3, fn4, c3 };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunctionType *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunctionType>();

    QCOMPARE(allTargetFunctions.size(), 4);
    QCOMPARE(targetModel.getFunction("Fn1", m_caseCheck), fn1);
    QCOMPARE(targetModel.getFunction("Fn2", m_caseCheck), fn2);
    QCOMPARE(targetModel.getFunction("Fn3", m_caseCheck), fn3);
    QCOMPARE(targetModel.getFunction("Fn4", m_caseCheck), fn4);
    QCOMPARE(targetModel.getConnection("C1", "Fn1", "Fn2", m_caseCheck), c1);
    QCOMPARE(targetModel.getConnection("C2", "Fn2", "Fn3", m_caseCheck), c2);
    QCOMPARE(targetModel.getConnection("C3", "Fn3", "Fn4", m_caseCheck), c3);
}

void tst_IvMerger::test_ivMergeNestedFunctions()
{
    ivm::IVModel targetModel(m_dynPropConfig);

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");

    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");

    const QVector<ivm::IVObject *> targetObjects { fn1, fn2, c1 };
    targetModel.addObjects(targetObjects);

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn1 = ivm::testutils::createFunction("SourceFn1");
    ivm::IVFunction *sourceNestedFn1 = ivm::testutils::createFunction("SourceNestedFn1");
    ivm::IVFunction *sourceNestedFn2 = ivm::testutils::createFunction("SourceNestedFn2");
    ivm::IVConnection *nestedC1 = ivm::testutils::createConnection(sourceNestedFn1, sourceNestedFn2, "NestedC1");
    sourceFn1->addChild(sourceNestedFn1);
    sourceFn1->addChild(sourceNestedFn2);
    sourceFn1->addChild(nestedC1);

    const QVector<ivm::IVObject *> sourceObjects { sourceFn1, sourceNestedFn1, sourceNestedFn2, nestedC1 };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunctionType *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunctionType>();

    QCOMPARE(allTargetFunctions.size(), 5);
    QCOMPARE(targetModel.getFunction("Fn1", m_caseCheck), fn1);
    QCOMPARE(targetModel.getFunction("Fn2", m_caseCheck), fn2);
    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck), sourceFn1);
    QCOMPARE(targetModel.getFunction("SourceNestedFn1", m_caseCheck), sourceNestedFn1);
    QCOMPARE(targetModel.getFunction("SourceNestedFn2", m_caseCheck), sourceNestedFn2);
    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck)->functions().size(), 2);
    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck)->connections().size(), 1);
    QCOMPARE(targetModel.getConnection("C1", "Fn1", "Fn2", m_caseCheck), c1);
    QCOMPARE(targetModel.getConnection("NestedC1", "SourceNestedFn1", "SourceNestedFn2", m_caseCheck), nestedC1);
}

QTEST_APPLESS_MAIN(tst_IvMerger)

#include "tst_ivmerger.moc"