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

#include "ivconnection.h"
#include "ivconnectionchain.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivxmlreader.h"
#include "propertytemplateconfig.h"

#include <QtTest>

class tst_AADLConnectionChain : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test3StraightConnections();
    void testTargetSplitsIntoTwo();
    void testTargetJoinIntone();
    void testChainCreationMultiChainOnInterfaces();
    void testContains();
    void testGetNames();

private:
    ivm::IVConnection *selectConnection(
            const QString &sourceName, const QString &targetName, QList<ivm::IVConnection *> connections) const;
    bool checkChain(ivm::IVConnectionChain *chain, const QStringList &functionNames) const;

private:
    ivm::PropertyTemplateConfig *conf { nullptr };
};

ivm::IVConnection *tst_AADLConnectionChain::selectConnection(
        const QString &sourceName, const QString &targetName, QList<ivm::IVConnection *> connections) const
{
    for (auto co : connections) {
        if (co->sourceName() == sourceName && co->targetName() == targetName) {
            return co;
        }
    }

    return {};
}

bool tst_AADLConnectionChain::checkChain(ivm::IVConnectionChain *chain, const QStringList &functionNames) const
{
    if (chain == nullptr) {
        return false;
    }

    if (functionNames.size() < 2) {
        return true;
    }

    if (chain->connections().size() != functionNames.size() - 1) {
        return false;
    }

    const QList<ivm::IVConnection *> &connections = chain->connections();
    if (connections[0]->sourceName() != functionNames[0]) {
        return false;
    }

    int i = 1;
    for (const ivm::IVConnection *c : connections) {
        if (c->targetName() != functionNames[i]) {
            return false;
        }
        ++i;
    }

    return true;
}

// MiniA -> BlockA -> BlockB ->MiniB
void tst_AADLConnectionChain::initTestCase()
{
    ivm::initIVLibrary();
    conf = ivm::PropertyTemplateConfig::instance();
    conf->init(QLatin1String("default_attrinbutes.xml"));
}

void tst_AADLConnectionChain::test3StraightConnections()
{
    const QStringList expectedChain = { "MiniA", "BlockA", "BlockB", "MiniB" };

    ivm::IVModel model(conf);
    ivm::IVXMLReader parser;
    connect(&parser, &ivm::IVXMLReader::objectsParsed, &model, &ivm::IVModel::initFromObjects);
    parser.readFile(QFINDTESTDATA("connectionchains01.xml"));
    QCOMPARE(model.objects().size(), 11);

    QList<ivm::IVConnection *> allConnections = model.allObjectsByType<ivm::IVConnection>().toList();
    ivm::IVConnection *startConnection = selectConnection("MiniA", "BlockA", allConnections);
    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(startConnection, allConnections);
    QCOMPARE(chains.size(), 1);
    QVERIFY(checkChain(chains[0], expectedChain));
    qDeleteAll(chains);
    chains.clear();

    startConnection = selectConnection("BlockA", "BlockB", allConnections);
    chains = ivm::IVConnectionChain::build(startConnection, allConnections);
    QCOMPARE(chains.size(), 1);
    QVERIFY(checkChain(chains[0], expectedChain));
    qDeleteAll(chains);
    chains.clear();

    startConnection = selectConnection("BlockB", "MiniB", allConnections);
    chains = ivm::IVConnectionChain::build(startConnection, allConnections);
    QCOMPARE(chains.size(), 1);
    QVERIFY(checkChain(chains[0], expectedChain));
    qDeleteAll(chains);
    chains.clear();

    chains = ivm::IVConnectionChain::build(model);
    QCOMPARE(chains.size(), 1);
    QVERIFY(checkChain(chains[0], expectedChain));
    qDeleteAll(chains);
    chains.clear();
}

// MiniA -> BlockA -> BlockB -> MiniB1 -> MacroB1
// MiniA -> BlockA -> BlockB -> MiniB2
void tst_AADLConnectionChain::testTargetSplitsIntoTwo()
{
    ivm::IVModel model(conf);
    ivm::IVXMLReader parser;
    connect(&parser, &ivm::IVXMLReader::objectsParsed, &model, &ivm::IVModel::initFromObjects);
    parser.readFile(QFINDTESTDATA("connectionchains02.xml"));
    QCOMPARE(model.objects().size(), 18);

    QList<ivm::IVConnection *> allConnections = model.allObjectsByType<ivm::IVConnection>().toList();
    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(model);
    QCOMPARE(chains.size(), 2);
    qDeleteAll(chains);
    chains.clear();
}

// MiniA1 -> BlockA -> BlockB -> MiniB1 -> MacroB1
// MiniA2 -> BlockA -> BlockB -> MiniB1 -> MacroB1
void tst_AADLConnectionChain::testTargetJoinIntone()
{

    ivm::IVModel model(conf);
    ivm::IVXMLReader parser;
    connect(&parser, &ivm::IVXMLReader::objectsParsed, &model, &ivm::IVModel::initFromObjects);
    parser.readFile(QFINDTESTDATA("connectionchains03.xml"));
    QCOMPARE(model.objects().size(), 20);

    QList<ivm::IVConnection *> allConnections = model.allObjectsByType<ivm::IVConnection>().toList();
    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(model);
    QCOMPARE(chains.size(), 2);
    qDeleteAll(chains);
    chains.clear();
}

// PI_1 on "Block B" has 2 input and 2 output connections - resulting in 4 chains
// MiniA2.RI_1->BlockA.RI_2->BlockB.PI_1->MiniB1.PI_1->MacroB1.PI_1
// MiniA2.RI_1->BlockA.RI_2->BlockB.PI_1->MiniB2.PI_1
// MiniA1.RI_1->BlockA.RI_1->BlockB.PI_1->MiniB1.PI_1->MacroB1.PI_1
// MiniA1.RI_1->BlockA.RI_1->BlockB.PI_1->MiniB2.PI_1
void tst_AADLConnectionChain::testChainCreationMultiChainOnInterfaces()
{
    ivm::IVModel model(conf);
    ivm::IVXMLReader parser;
    connect(&parser, &ivm::IVXMLReader::objectsParsed, &model, &ivm::IVModel::initFromObjects);
    parser.readFile(QFINDTESTDATA("connectionchains04.xml"));
    QCOMPARE(model.objects().size(), 21);

    QList<ivm::IVConnection *> allConnections = model.allObjectsByType<ivm::IVConnection>().toList();
    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(model);
    QCOMPARE(chains.size(), 3);
    qDeleteAll(chains);
    chains.clear();
}

// MiniA -> BlockA -> BlockB -> MiniB
void tst_AADLConnectionChain::testContains()
{

    ivm::IVModel model(conf);
    ivm::IVXMLReader parser;
    connect(&parser, &ivm::IVXMLReader::objectsParsed, &model, &ivm::IVModel::initFromObjects);
    parser.readFile(QFINDTESTDATA("connectionchains01.xml"));
    QCOMPARE(model.objects().size(), 11);

    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(model);
    QCOMPARE(chains.size(), 1);
    ivm::IVConnectionChain *chain = chains.at(0);

    QCOMPARE(chain->contains("PI_1", "BlockB", "MiniB"), true); // One connection only
    QCOMPARE(chain->contains("PI_1", "BlockA", "MiniB"), true); // Sub-chain
    QCOMPARE(chain->contains("PI_1", "MiniA", "MiniB"), true); // Whole chain

    QCOMPARE(chain->contains("dummy", "foo", "bar"), false); // Totally bogus
    QCOMPARE(chain->contains("dummy", "BlockB", "MiniB"), false); // Wrong connection name
    QCOMPARE(chain->contains("PI_1", "foo", "MiniB"), false); // Wrong source
    QCOMPARE(chain->contains("PI_1", "MiniA", "foo"), false); // Wrong target

    // message from/to environment
    QCOMPARE(chain->contains("PI_1", "", "MiniB"), true);
    QCOMPARE(chain->contains("PI_1", "MiniA", ""), true);

    qDeleteAll(chains);
    chains.clear();
}

// MiniA -> BlockA -> BlockB -> MiniB
void tst_AADLConnectionChain::testGetNames()
{

    ivm::IVModel model(conf);
    ivm::IVXMLReader parser;
    connect(&parser, &ivm::IVXMLReader::objectsParsed, &model, &ivm::IVModel::initFromObjects);
    parser.readFile(QFINDTESTDATA("connectionchains01.xml"));

    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(model);
    QCOMPARE(chains.size(), 1);
    ivm::IVConnectionChain *chain = chains.at(0);

    QCOMPARE(chain->connectionNames("MiniA", "MiniB"), { QString("PI_1") });
    QCOMPARE(chain->connectionNames("MiniA", "BlockB"), { QString("PI_2") });
    QCOMPARE(chain->connectionNames("BlockA", "MiniB"), { QString("PI_1") });
    QStringList result = { QString("PI_2"), QString("PI_1") };
    QCOMPARE(chain->connectionNames("BlockA", ""), result);
}

QTEST_APPLESS_MAIN(tst_AADLConnectionChain)

#include "tst_aadlconnectionchain.moc"
