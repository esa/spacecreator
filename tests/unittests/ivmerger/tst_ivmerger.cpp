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
    void test_capabilityRemovedFromTailoring();
    void test_ivMergeDataPoolConnections();

private:
    ivm::IVFunction *findFunction(QVector<ivm::IVFunction *> &list, const QString &name);
    ivm::IVInterface *findInterface(QVector<ivm::IVInterface *> &list, const QString &name);

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

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1", &targetModel);
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2", &targetModel);
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3", &targetModel);
    ivm::IVFunction *fn4 = ivm::testutils::createFunction("Fn4", &targetModel);

    ivm::testutils::createRequiredIface(fn1, "C1");
    ivm::testutils::createProvidedIface(fn2, "C1");
    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::testutils::createRequiredIface(fn3, "C2");
    ivm::testutils::createProvidedIface(fn4, "C2");
    ivm::IVConnection *c2 = ivm::testutils::createConnection(fn3, fn4, "C2");

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn1 = ivm::testutils::createFunction("SourceFn1", &sourceModel);
    ivm::IVFunction *sourceFn2 = ivm::testutils::createFunction("SourceFn2", &sourceModel);
    ivm::IVFunction *sourceFn3 = ivm::testutils::createFunction("SourceFn3", &sourceModel);
    ivm::IVFunction *sourceFn4 = ivm::testutils::createFunction("SourceFn4", &sourceModel);

    ivm::testutils::createRequiredIface(sourceFn1, "SourceC1");
    ivm::testutils::createProvidedIface(sourceFn2, "SourceC1");
    ivm::testutils::createConnection(sourceFn1, sourceFn2, "SourceC1");
    ivm::testutils::createRequiredIface(sourceFn3, "SourceC2");
    ivm::testutils::createProvidedIface(sourceFn4, "SourceC2");
    ivm::testutils::createConnection(sourceFn3, sourceFn4, "SourceC2");

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

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1", &targetModel);
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2", &targetModel);
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3", &targetModel, f3Id);
    ivm::IVFunction *fn4 = ivm::testutils::createFunction("Fn4", &targetModel, f4Id);

    ivm::testutils::createRequiredIface(fn1, "C1");
    ivm::testutils::createProvidedIface(fn2, "C1");
    ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::testutils::createRequiredIface(fn2, "C2");
    ivm::testutils::createProvidedIface(fn3, "C2");
    ivm::testutils::createConnection(fn2, fn3, "C2");
    ivm::testutils::createRequiredIface(fn3, "C3");
    ivm::testutils::createProvidedIface(fn4, "C3");
    ivm::testutils::createConnection(fn3, fn4, "C3");

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn3 = ivm::testutils::createFunction("Fn3", &sourceModel, f3Id);
    ivm::IVFunction *sourceFn4 = ivm::testutils::createFunction("Fn4", &sourceModel, f4Id);
    ivm::testutils::createRequiredIface(sourceFn3, "C3");
    ivm::testutils::createRequiredIface(sourceFn4, "C3");
    ivm::testutils::createConnection(sourceFn3, sourceFn4, "C3");

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

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1", &targetModel);
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2", &targetModel);

    ivm::testutils::createRequiredIface(fn1, "C1");
    ivm::testutils::createProvidedIface(fn2, "C1");
    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");

    ivm::IVModel sourceModel(m_dynPropConfig);

    ivm::IVFunction *sourceFn1 = ivm::testutils::createFunction("SourceFn1", &sourceModel);
    ivm::IVFunction *sourceNestedFn1 = ivm::testutils::createFunction("SourceNestedFn1", sourceFn1);
    ivm::IVFunction *sourceNestedFn2 = ivm::testutils::createFunction("SourceNestedFn2", sourceFn1);
    ivm::testutils::createRequiredIface(sourceNestedFn1, "NestedC1");
    ivm::testutils::createRequiredIface(sourceNestedFn2, "NestedC1");
    ivm::testutils::createConnection(sourceNestedFn1, sourceNestedFn2, "NestedC1");

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

void tst_IvMerger::test_capabilityRemovedFromTailoring()
{
    // required ids
    shared::Id routerId = shared::createId();
    shared::Id applicationId = shared::createId();
    shared::Id capabilityRouterId = shared::createId();
    shared::Id capability1Id = shared::createId();
    shared::Id capability2Id = shared::createId();

    // target model
    ivm::IVFunction *router = ivm::testutils::createFunction("router", nullptr, routerId);
    ivm::IVInterface *routerTc = ivm::testutils::createRequiredIface(router, "tc");
    routerTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *app = ivm::testutils::createFunction("application", nullptr, applicationId);
    ivm::IVInterface *appTc = ivm::testutils::createProvidedIface(app, "tc");
    appTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *capabilityRouter =
            ivm::testutils::createFunction("capability_router", nullptr, capabilityRouterId);
    ivm::IVInterface *capabilityRouterTc = ivm::testutils::createProvidedIface(capabilityRouter, "tc");
    capabilityRouterTc->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *capabilityRouterReq1 = ivm::testutils::createRequiredIface(capabilityRouter, "req_1");
    capabilityRouterReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *capabilityRouterReq2 = ivm::testutils::createRequiredIface(capabilityRouter, "req_2");
    capabilityRouterReq2->setKind(ivm::IVInterface::OperationKind::Protected);
    app->addChild(capabilityRouter);

    ivm::IVFunction *capability1 = ivm::testutils::createFunction("capability_1", nullptr, capability1Id);
    ivm::IVInterface *capability1Req1 = ivm::testutils::createProvidedIface(capability1, "req_1");
    capability1Req1->setKind(ivm::IVInterface::OperationKind::Protected);
    app->addChild(capability1);

    ivm::IVFunction *capability2 = ivm::testutils::createFunction("capability_2", nullptr, capability2Id);
    ivm::IVInterface *capability2Req2 = ivm::testutils::createProvidedIface(capability2, "req21");
    capability2Req2->setKind(ivm::IVInterface::OperationKind::Protected);
    app->addChild(capability2);

    ivm::testutils::createConnection(capabilityRouter, capability1, "req_1");
    ivm::testutils::createConnection(capabilityRouter, capability2, "req_2");

    ivm::IVConnection *connAppTc = ivm::testutils::createConnection(appTc, capabilityRouterTc);
    app->addChild(connAppTc);

    ivm::IVConnection *connTc = ivm::testutils::createConnection(routerTc, appTc);

    ivm::IVModel targetModel(m_dynPropConfig);
    const QVector<ivm::IVObject *> targetObjects { router, app, connTc };
    targetModel.addObjects(targetObjects);

    // source model
    ivm::IVFunction *sourceRouter = ivm::testutils::createFunction("router", nullptr, routerId);
    ivm::IVInterface *sourceRouterTc = ivm::testutils::createRequiredIface(sourceRouter, "tc");
    sourceRouterTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *sourceApp = ivm::testutils::createFunction("application", nullptr, applicationId);
    ivm::IVInterface *sourceAppTc = ivm::testutils::createProvidedIface(sourceApp, "tc");
    sourceAppTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *sourceCapabilityRouter =
            ivm::testutils::createFunction("capability_router", nullptr, capabilityRouterId);
    ivm::IVInterface *sourceCapabilityRouterTc = ivm::testutils::createProvidedIface(sourceCapabilityRouter, "tc");
    sourceAppTc->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *sourceCapabilityRouterReq1 = ivm::testutils::createRequiredIface(sourceCapabilityRouter, "req_1");
    sourceCapabilityRouterReq1->setKind(ivm::IVInterface::OperationKind::Protected);

    sourceApp->addChild(sourceCapabilityRouter);

    ivm::IVFunction *sourceCapability1 = ivm::testutils::createFunction("capability_1", nullptr, capability1Id);
    ivm::IVInterface *sourceCapabilityReq1 = ivm::testutils::createProvidedIface(sourceCapability1, "req_1");
    sourceCapabilityReq1->setKind(ivm::IVInterface::OperationKind::Protected);

    sourceApp->addChild(sourceCapability1);

    ivm::testutils::createConnection(sourceCapabilityRouter, sourceCapability1, "req_1");

    ivm::IVConnection *sourceConnAppTc = ivm::testutils::createConnection(appTc, sourceCapabilityRouterTc);
    sourceApp->addChild(sourceConnAppTc);

    ivm::IVConnection *sourceConnTc = ivm::testutils::createConnection(sourceRouterTc, appTc);

    ivm::IVModel sourceModel(m_dynPropConfig);
    const QVector<ivm::IVObject *> sourceObjects { sourceRouter, sourceApp, sourceConnTc };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunction *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunction>();

    QCOMPARE(allTargetFunctions.size(), 2);

    QCOMPARE(targetModel.allObjectsByType<ivm::IVConnection>().size(), 1);

    ivm::IVFunction *resultRouter = findFunction(allTargetFunctions, "router");

    ivm::IVFunction *resultApplication = findFunction(allTargetFunctions, "application");

    QVERIFY(resultRouter != nullptr);
    QVERIFY(resultApplication != nullptr);

    QCOMPARE(resultRouter->interfaces().size(), 1);
    QCOMPARE(resultRouter->interfaces().front()->title(), "tc");
    QVERIFY(resultRouter->interfaces().front()->isRequired());
    QCOMPARE(resultRouter->interfaces().front()->kind(), ivm::IVInterface::OperationKind::Protected);

    QCOMPARE(resultApplication->interfaces().size(), 1);
    QCOMPARE(resultApplication->interfaces().front()->title(), "tc");
    QVERIFY(resultApplication->interfaces().front()->isProvided());
    QCOMPARE(resultApplication->interfaces().front()->kind(), ivm::IVInterface::OperationKind::Protected);

    QCOMPARE(targetModel.allObjectsByType<ivm::IVConnection>().first()->source()->title(), "router");
    QCOMPARE(targetModel.allObjectsByType<ivm::IVConnection>().first()->target()->title(), "application");

    QVector<ivm::IVFunction *> applicationFunctions = resultApplication->functions();

    QCOMPARE(applicationFunctions.size(), 2);

    ivm::IVFunction *resultCapabilityRouter = findFunction(applicationFunctions, "capability_router");
    ivm::IVFunction *resultCapability = findFunction(applicationFunctions, "capability_1");

    QVERIFY(resultCapabilityRouter != nullptr);
    QVERIFY(resultCapability != nullptr);

    QVector<ivm::IVInterface *> capabilityRouterInterfaces = resultCapabilityRouter->interfaces();
    QCOMPARE(capabilityRouterInterfaces.size(), 2);
    ivm::IVInterface *resultCapabilityTc = findInterface(capabilityRouterInterfaces, "tc");
    ivm::IVInterface *resultCapabilityReq1 = findInterface(capabilityRouterInterfaces, "req_1");

    QVERIFY(resultCapabilityTc != nullptr);
    QVERIFY(resultCapabilityReq1 != nullptr);
    QCOMPARE(resultCapabilityTc->kind(), ivm::IVInterface::OperationKind::Protected);
    QVERIFY(resultCapabilityTc->isProvided());
    QCOMPARE(resultCapabilityReq1->kind(), ivm::IVInterface::OperationKind::Protected);
    QVERIFY(resultCapabilityReq1->isRequired());

    QCOMPARE(resultCapability->interfaces().size(), 1);
    QCOMPARE(resultCapability->interfaces().front()->title(), "req_1");
    QCOMPARE(resultCapability->interfaces().front()->kind(), ivm::IVInterface::OperationKind::Protected);
    QVERIFY(resultCapability->interfaces().front()->isProvided());

    QCOMPARE(resultApplication->connections().size(), 2);

    ivm::IVConnection *firstConn = resultApplication->connections().first();
    ivm::IVConnection *lastConn = resultApplication->connections().last();

    if (firstConn->targetName() == "capability_router") {
        QCOMPARE(firstConn->targetName(), "capability_router");
        QCOMPARE(firstConn->targetInterfaceName(), "tc");
        QCOMPARE(firstConn->sourceName(), "application");
        QCOMPARE(firstConn->sourceInterfaceName(), "tc");

        QCOMPARE(lastConn->targetName(), "capability_1");
        QCOMPARE(lastConn->targetInterfaceName(), "req_1");
        QCOMPARE(lastConn->sourceName(), "capability_router");
        QCOMPARE(lastConn->sourceInterfaceName(), "req_1");
    } else {
        QCOMPARE(lastConn->targetName(), "capability_router");
        QCOMPARE(lastConn->targetInterfaceName(), "tc");
        QCOMPARE(lastConn->sourceName(), "application");
        QCOMPARE(lastConn->sourceInterfaceName(), "tc");

        QCOMPARE(firstConn->targetName(), "capability_1");
        QCOMPARE(firstConn->targetInterfaceName(), "req_1");
        QCOMPARE(firstConn->sourceName(), "capability_router");
        QCOMPARE(firstConn->sourceInterfaceName(), "req_1");
    }
}

void tst_IvMerger::test_ivMergeDataPoolConnections()
{
    // required ids
    shared::Id routerId = shared::createId();
    shared::Id dataPoolId = shared::createId();
    shared::Id applicationId = shared::createId();
    shared::Id capabilityRouterId = shared::createId();
    shared::Id capability1Id = shared::createId();
    shared::Id capability2Id = shared::createId();
    shared::Id capability3Id = shared::createId();

    // target model
    ivm::IVFunction *router = ivm::testutils::createFunction("router", nullptr, routerId);
    ivm::IVInterface *routerTc = ivm::testutils::createRequiredIface(router, "tc");
    routerTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *app = ivm::testutils::createFunction("application", nullptr, applicationId);
    ivm::IVInterface *appTc = ivm::testutils::createProvidedIface(app, "tc");
    appTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *capabilityRouter =
            ivm::testutils::createFunction("capability_router", nullptr, capabilityRouterId);
    ivm::IVInterface *capabilityRouterTc = ivm::testutils::createProvidedIface(capabilityRouter, "tc");
    capabilityRouterTc->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *capabilityRouterReq1 = ivm::testutils::createRequiredIface(capabilityRouter, "req_1");
    capabilityRouterReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *capabilityRouterReq2 = ivm::testutils::createRequiredIface(capabilityRouter, "req_2");
    capabilityRouterReq2->setKind(ivm::IVInterface::OperationKind::Protected);

    app->addChild(capabilityRouter);

    ivm::IVFunction *capability1 = ivm::testutils::createFunction("capability_1", nullptr, capability1Id);
    ivm::IVInterface *capability1Req1 = ivm::testutils::createProvidedIface(capability1, "req_1");
    capability1Req1->setKind(ivm::IVInterface::OperationKind::Protected);

    app->addChild(capability1);

    ivm::IVFunction *capability2 = ivm::testutils::createFunction("capability_2", nullptr, capability2Id);
    ivm::IVInterface *capability2Req2 = ivm::testutils::createProvidedIface(capability2, "req_2");
    capability2Req2->setKind(ivm::IVInterface::OperationKind::Protected);

    app->addChild(capability2);

    ivm::testutils::createConnection(capabilityRouter, capability1, "req_1");
    ivm::testutils::createConnection(capabilityRouter, capability2, "req_2");

    ivm::testutils::createConnection(app, capabilityRouter, "tc");

    ivm::IVConnection *connTc = ivm::testutils::createConnection(router, app, "tc");

    ivm::IVModel targetModel(m_dynPropConfig);
    const QVector<ivm::IVObject *> targetObjects { router, app, connTc };
    targetModel.addObjects(targetObjects);

    // source model
    ivm::IVFunction *sourceRouter = ivm::testutils::createFunction("router", nullptr, routerId);
    ivm::IVInterface *sourceRouterTc = ivm::testutils::createRequiredIface(sourceRouter, "tc");
    sourceRouterTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *sourceDataPool = ivm::testutils::createFunction("data_pool", nullptr, dataPoolId);
    ivm::IVInterface *sourceDataPoolIface1 = ivm::testutils::createProvidedIface(sourceDataPool, "data_pool_load_parameter_value");
    sourceDataPoolIface1->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *sourceDataPoolIface2 = ivm::testutils::createProvidedIface(sourceDataPool, "data_pool_store_parameter_value");
    sourceDataPoolIface2->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *sourceApp = ivm::testutils::createFunction("application", nullptr, applicationId);
    ivm::IVInterface *sourceAppTc = ivm::testutils::createProvidedIface(sourceApp, "tc");
    sourceAppTc->setKind(ivm::IVInterface::OperationKind::Protected);

    ivm::IVFunction *sourceCapabilityRouter =
            ivm::testutils::createFunction("capability_router", nullptr, capabilityRouterId);
    ivm::IVInterface *sourceCapabilityRouterTc = ivm::testutils::createProvidedIface(sourceCapabilityRouter, "tc");
    sourceCapabilityRouterTc->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *sourceCapabilityRouterReq1 = ivm::testutils::createRequiredIface(sourceCapabilityRouter, "req_1");
    sourceCapabilityRouterReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::testutils::createRequiredIface(sourceCapabilityRouter, "req_2");
    sourceCapabilityRouterReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::testutils::createRequiredIface(sourceCapabilityRouter, "req_3");
    sourceCapabilityRouterReq1->setKind(ivm::IVInterface::OperationKind::Protected);

    sourceApp->addChild(sourceCapabilityRouter);

    ivm::IVFunction *sourceCapability1 = ivm::testutils::createFunction("capability_1", nullptr, capability1Id);
    ivm::IVInterface *sourceCapability1Req1 = ivm::testutils::createProvidedIface(capability1, "req_1");
    sourceCapability1Req1->setKind(ivm::IVInterface::OperationKind::Protected);

    sourceApp->addChild(sourceCapability1);

    ivm::IVFunction *sourceCapability2 = ivm::testutils::createFunction("capability_2", nullptr, capability2Id);
    ivm::IVInterface *sourceCapability2Req2 = ivm::testutils::createProvidedIface(sourceCapability2, "req_2");
    sourceCapability2Req2->setKind(ivm::IVInterface::OperationKind::Protected);

    sourceApp->addChild(sourceCapability2);

    ivm::IVFunction *sourceCapability3 = ivm::testutils::createFunction("capability_3", nullptr, capability3Id);
    ivm::IVInterface *sourceCapability3Req3 = ivm::testutils::createProvidedIface(sourceCapability3, "req_3");
    sourceCapability3Req3->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *sourceCapability3DataPoolReq1 = ivm::testutils::createRequiredIface(sourceCapability3, "data_pool_load_parameter_value");
    sourceCapability3DataPoolReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *sourceCapability3DataPoolReq2 = ivm::testutils::createRequiredIface(sourceCapability3, "data_pool_store_parameter_value");
    sourceCapability3DataPoolReq2->setKind(ivm::IVInterface::OperationKind::Protected);

    sourceApp->addChild(sourceCapability3);

    ivm::IVInterface *sourceAppDataPoolReq1 = ivm::testutils::createRequiredIface(sourceApp, "data_pool_load_parameter_value");
    sourceAppDataPoolReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    sourceApp->addChild(sourceAppDataPoolReq1);
    ivm::IVInterface *sourceAppDataPoolReq2 = ivm::testutils::createRequiredIface(sourceApp, "data_pool_store_parameter_value");
    sourceAppDataPoolReq1->setKind(ivm::IVInterface::OperationKind::Protected);
    new ivm::IVConnection(sourceCapability3DataPoolReq1, sourceAppDataPoolReq1);
    new ivm::IVConnection(sourceCapability3DataPoolReq2, sourceAppDataPoolReq2);

    ivm::testutils::createConnection(sourceCapabilityRouter, sourceCapability1, "req_1");
    ivm::testutils::createConnection(sourceCapabilityRouter, sourceCapability2, "req_2");
    ivm::testutils::createConnection(sourceCapabilityRouter, sourceCapability3, "req_3");

    ivm::testutils::createConnection(appTc, sourceCapabilityRouterTc);

    ivm::IVConnection *sourceConnTc = ivm::testutils::createConnection(sourceRouterTc, appTc);

    ivm::IVConnection *sourceDataPoolConn1 =
            ivm::testutils::createConnection(sourceApp, sourceDataPool, "data_pool_load_parameter_value");
    ivm::IVConnection *sourceDataPoolConn2 =
            ivm::testutils::createConnection(sourceApp, sourceDataPool, "data_pool_store_parameter_value");

    ivm::IVModel sourceModel(m_dynPropConfig);
    const QVector<ivm::IVObject *> sourceObjects { sourceRouter, sourceDataPool, sourceApp, sourceConnTc,
        sourceDataPoolConn1, sourceDataPoolConn2 };
    sourceModel.addObjects(sourceObjects);

    ivmerger::IvMerger merger;
    merger.mergeInterfaceViews(targetModel, sourceModel);

    QVector<ivm::IVFunction *> allTargetFunctions = targetModel.allObjectsByType<ivm::IVFunction>();

    QCOMPARE(allTargetFunctions.size(), 3);

    ivm::IVFunction *resultRouter = findFunction(allTargetFunctions, "router");
    ivm::IVFunction *resultApplication = findFunction(allTargetFunctions, "application");
    ivm::IVFunction *resultDataPool = findFunction(allTargetFunctions, "data_pool");

    QVERIFY(resultRouter != nullptr);
    QVERIFY(resultApplication != nullptr);
    QVERIFY(resultDataPool != nullptr);

    QVector<ivm::IVInterface *> resultApplicationInterfaces = resultApplication->interfaces();
    QCOMPARE(resultApplicationInterfaces.size(), 3);

    ivm::IVInterface *resultAppInterface1 = findInterface(resultApplicationInterfaces, "data_pool_load_parameter_value");
    ivm::IVInterface *resultAppInterface2 = findInterface(resultApplicationInterfaces, "data_pool_store_parameter_value");

    QVERIFY(resultAppInterface1 != nullptr);
    QVERIFY(resultAppInterface2 != nullptr);

    QVector<ivm::IVInterface *> resultDataPoolInterfaces = resultDataPool->interfaces();
    QCOMPARE(resultDataPoolInterfaces.size(), 2);

    ivm::IVInterface *resultDataPoolInterface1 = findInterface(resultDataPoolInterfaces, "data_pool_load_parameter_value");
    ivm::IVInterface *resultDataPoolInterface2 = findInterface(resultDataPoolInterfaces, "data_pool_store_parameter_value");

    QVERIFY(resultDataPoolInterface1 != nullptr);
    QVERIFY(resultDataPoolInterface2 != nullptr);

    QCOMPARE(targetModel.allObjectsByType<ivm::IVConnection>().size(), 3);

    for (ivm::IVConnection *conn : targetModel.allObjectsByType<ivm::IVConnection>())
    {
        if(conn->targetName() == "application")
        {
            continue;
        }
        QCOMPARE(conn->targetName(), "data_pool");

        if(conn->sourceInterfaceName() != "data_pool_load_parameter_value")
        {
            QCOMPARE(conn->sourceInterfaceName(), "data_pool_store_parameter_value");
        }
        if(conn->sourceInterfaceName() != "data_pool_store_parameter_value")
        {
            QCOMPARE(conn->sourceInterfaceName(), "data_pool_load_parameter_value");
        }
    }
}

ivm::IVFunction *tst_IvMerger::findFunction(QVector<ivm::IVFunction *> &list, const QString &name)
{
    auto iter = std::find_if(
            list.begin(), list.end(), [&name](ivm::IVFunction *fn) { return fn->title().compare(name) == 0; });
    return iter != list.end() ? *iter : nullptr;
}

ivm::IVInterface *tst_IvMerger::findInterface(QVector<ivm::IVInterface *> &list, const QString &name)
{
    auto iter = std::find_if(
            list.begin(), list.end(), [&name](ivm::IVInterface *iface) { return iface->title().compare(name) == 0; });
    return iter != list.end() ? *iter : nullptr;
}

QTEST_APPLESS_MAIN(tst_IvMerger)

#include "tst_ivmerger.moc"
