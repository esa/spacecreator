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
#include "ivmerger/ivmerger.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"

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

    ivm::IVInterface *fn1c1 = ivm::testutils::createRequiredIface(fn1, "C1");
    fn1->addChild(fn1c1);
    ivm::IVInterface *fn2c1 = ivm::testutils::createProvidedIface(fn2, "C1");
    fn2->addChild(fn2c1);
    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::IVInterface *fn3c2 = ivm::testutils::createRequiredIface(fn3, "C2");
    fn3->addChild(fn3c2);
    ivm::IVInterface *fn4c2 = ivm::testutils::createProvidedIface(fn4, "C2");
    fn4->addChild(fn4c2);
    ivm::IVConnection *c2 = ivm::testutils::createConnection(fn3, fn4, "C2");

    const QVector<ivm::IVObject *> targetObjects { fn1, fn2, fn3, fn4, c1, c2 };
    targetModel.addObjects(targetObjects);

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn1 = ivm::testutils::createFunction("SourceFn1");
    ivm::IVFunction *sourceFn2 = ivm::testutils::createFunction("SourceFn2");
    ivm::IVFunction *sourceFn3 = ivm::testutils::createFunction("SourceFn3");
    ivm::IVFunction *sourceFn4 = ivm::testutils::createFunction("SourceFn4");

    ivm::IVInterface *sourcefn1c1 = ivm::testutils::createRequiredIface(sourceFn1, "SourceC1");
    sourceFn1->addChild(sourcefn1c1);
    ivm::IVInterface *sourcefn2c1 = ivm::testutils::createProvidedIface(sourceFn2, "SourceC1");
    sourceFn2->addChild(sourcefn2c1);
    ivm::IVConnection *sourceC1 = ivm::testutils::createConnection(sourceFn1, sourceFn2, "SourceC1");
    ivm::IVInterface *sourcefn3c2 = ivm::testutils::createRequiredIface(sourceFn3, "SourceC2");
    sourceFn3->addChild(sourcefn3c2);
    ivm::IVInterface *sourcefn4c2 = ivm::testutils::createProvidedIface(sourceFn4, "SourceC2");
    sourceFn4->addChild(sourcefn4c2);
    ivm::IVConnection *sourceC2 = ivm::testutils::createConnection(sourceFn3, sourceFn4, "SourceC2");

    const QVector<ivm::IVObject *> sourceObjects {
        sourceFn1,
        sourceFn2,
        sourceFn3,
        sourceFn4,
        sourceC1,
        sourceC2,
    };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunctionType *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunctionType>();
    QVector<ivm::IVConnection *> allConnections = targetModel.allObjectsByType<ivm::IVConnection>();

    QCOMPARE(allTargetFunctions.size(), 8);
    QCOMPARE(allConnections.size(), 4);
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
    QVERIFY(!targetModel.getConnectionsBetweenFunctions(sourceFn1->id(), sourceFn2->id()).empty());
    QVERIFY(!targetModel.getConnectionsBetweenFunctions(sourceFn3->id(), sourceFn4->id()).empty());
    QVERIFY(targetModel.getConnectionsBetweenFunctions(sourceFn1->id(), sourceFn2->id()).front() != nullptr);
    QVERIFY(targetModel.getConnectionsBetweenFunctions(sourceFn3->id(), sourceFn4->id()).front() != nullptr);
}

void tst_IvMerger::test_ivMergeWithReplace()
{
    ivm::IVModel targetModel(m_dynPropConfig);

    shared::Id f3Id = shared::createId();
    shared::Id f4Id = shared::createId();

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3", nullptr, f3Id);
    ivm::IVFunction *fn4 = ivm::testutils::createFunction("Fn4", nullptr, f4Id);

    ivm::IVInterface *fn1c1 = ivm::testutils::createRequiredIface(fn1, "C1");
    fn1->addChild(fn1c1);
    ivm::IVInterface *fn2c1 = ivm::testutils::createProvidedIface(fn2, "C1");
    fn2->addChild(fn2c1);
    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::IVInterface *fn2c2 = ivm::testutils::createRequiredIface(fn2, "C2");
    fn2->addChild(fn2c2);
    ivm::IVInterface *fn3c2 = ivm::testutils::createProvidedIface(fn3, "C2");
    fn3->addChild(fn3c2);
    ivm::IVConnection *c2 = ivm::testutils::createConnection(fn2, fn3, "C2");
    ivm::IVInterface *fn3c3 = ivm::testutils::createRequiredIface(fn3, "C3");
    fn3->addChild(fn3c3);
    ivm::IVInterface *fn4c3 = ivm::testutils::createProvidedIface(fn4, "C3");
    fn4->addChild(fn4c3);
    ivm::IVConnection *c3 = ivm::testutils::createConnection(fn3, fn4, "C3");

    const QVector<ivm::IVObject *> targetObjects { fn1, fn2, fn3, fn4, c1, c2, c3 };
    targetModel.addObjects(targetObjects);

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn3 = ivm::testutils::createFunction("Fn3", nullptr, f3Id);
    ivm::IVFunction *sourceFn4 = ivm::testutils::createFunction("Fn4", nullptr, f4Id);
    ivm::IVInterface *sourcefn3c3 = ivm::testutils::createRequiredIface(sourceFn3, "C3");
    sourceFn3->addChild(sourcefn3c3);
    ivm::IVInterface *sourcefn4c3 = ivm::testutils::createRequiredIface(sourceFn4, "C3");
    sourceFn4->addChild(sourcefn4c3);
    ivm::IVConnection *sourceC3 = ivm::testutils::createConnection(sourceFn3, sourceFn4, "C3");

    const QVector<ivm::IVObject *> sourceObjects { sourceFn3, sourceFn4, sourceC3 };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunctionType *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunctionType>();

    QCOMPARE(allTargetFunctions.size(), 4);
    QCOMPARE(targetModel.getFunction("Fn1", m_caseCheck), fn1);
    QCOMPARE(targetModel.getFunction("Fn2", m_caseCheck), fn2);
    QCOMPARE(targetModel.getFunction("Fn3", m_caseCheck), sourceFn3);
    QCOMPARE(targetModel.getFunction("Fn4", m_caseCheck), sourceFn4);
    QVERIFY(targetModel.getConnection("C1", "Fn1", "Fn2", m_caseCheck) != nullptr);
    QVERIFY(targetModel.getConnection("C3", "Fn3", "Fn4", m_caseCheck) != nullptr);
}

void tst_IvMerger::test_ivMergeNestedFunctions()
{
    ivm::IVModel targetModel(m_dynPropConfig);

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");

    ivm::IVInterface *fn1c1 = ivm::testutils::createRequiredIface(fn1, "C1");
    fn1->addChild(fn1c1);
    ivm::IVInterface *fn2c1 = ivm::testutils::createProvidedIface(fn2, "C1");
    fn2->addChild(fn2c1);
    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");

    const QVector<ivm::IVObject *> targetObjects { fn1, fn2, c1 };
    targetModel.addObjects(targetObjects);

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn1 = ivm::testutils::createFunction("SourceFn1");
    ivm::IVFunction *sourceNestedFn1 = ivm::testutils::createFunction("SourceNestedFn1");
    ivm::IVFunction *sourceNestedFn2 = ivm::testutils::createFunction("SourceNestedFn2");
    ivm::IVInterface *sourceNestedF1NestedC1 = ivm::testutils::createRequiredIface(sourceNestedFn1, "NestedC1");
    sourceNestedFn1->addChild(sourceNestedF1NestedC1);
    ivm::IVInterface *sourceNestedF2NestedC1 = ivm::testutils::createRequiredIface(sourceNestedFn2, "NestedC1");
    sourceNestedFn2->addChild(sourceNestedF2NestedC1);
    ivm::IVConnection *nestedC1 = ivm::testutils::createConnection(sourceNestedFn1, sourceNestedFn2, "NestedC1");
    sourceFn1->addChild(sourceNestedFn1);
    sourceFn1->addChild(sourceNestedFn2);
    sourceFn1->addChild(nestedC1);

    const QVector<ivm::IVObject *> sourceObjects { sourceFn1 };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunctionType *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunctionType>();

    QCOMPARE(allTargetFunctions.size(), 3);
    QCOMPARE(targetModel.getFunction("Fn1", m_caseCheck), fn1);
    QCOMPARE(targetModel.getFunction("Fn2", m_caseCheck), fn2);
    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck), sourceFn1);
    QVector<ivm::IVFunction *> fns = sourceFn1->functions();
    auto iter = std::find_if(fns.begin(), fns.end(), [](ivm::IVFunction *fn) {
        return fn->title().compare("SourceNestedFn1", Qt::CaseSensitivity::CaseInsensitive) == 0;
    });
    QVERIFY(iter != fns.end());
    QCOMPARE(*iter, sourceNestedFn1);
    iter = std::find_if(fns.begin(), fns.end(), [](ivm::IVFunction *fn) {
        return fn->title().compare("SourceNestedFn2", Qt::CaseSensitivity::CaseInsensitive) == 0;
    });
    QVERIFY(iter != fns.end());
    QCOMPARE(*iter, sourceNestedFn2);

    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck)->functions().size(), 2);
    QCOMPARE(targetModel.getFunction("SourceFn1", m_caseCheck)->connections().size(), 1);
    QCOMPARE(targetModel.getConnection("C1", "Fn1", "Fn2", m_caseCheck), c1);
    QVector<ivm::IVConnection *> conns = sourceFn1->connections();
    QCOMPARE(conns.size(), 1);
    auto citer = conns.begin();
    QCOMPARE((*citer)->source()->title(), "SourceNestedFn1");
    QCOMPARE((*citer)->target()->title(), "SourceNestedFn2");
}

QTEST_APPLESS_MAIN(tst_IvMerger)

#include "tst_ivmerger.moc"
