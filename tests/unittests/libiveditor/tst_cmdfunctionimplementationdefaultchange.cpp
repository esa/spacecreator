/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commands/cmdfunctionimplementationdefaultchange.h"
#include "commands/implementationshandler.h"
#include "ivcommonprops.h"
#include "iveditor.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivpropertytemplateconfig.h"

#include <QDir>
#include <QObject>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QtTest>

class tst_CmdFunctionImplementationDefaultChange : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void test_switchDefaultImplementation();
    void test_undoDefaultImplementation();
    void test_resetDefaultImplementation();

private:
    std::unique_ptr<QTemporaryDir> m_projectDir;
    std::unique_ptr<ivm::IVFunction> m_function;
    std::unique_ptr<ive::cmd::CmdFunctionImplementationDefaultChange> m_cmd;
    std::unique_ptr<ive::ImplementationsHandler> m_handler;
};

void tst_CmdFunctionImplementationDefaultChange::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
    ive::initIVEditor();
    auto dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    dynPropConfig->init(shared::interfaceCustomAttributesFilePath());
}

void tst_CmdFunctionImplementationDefaultChange::init()
{
    m_projectDir = std::make_unique<QTemporaryDir>();
    m_function = std::make_unique<ivm::IVFunction>();
    m_function->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), "MyFunc");
    EntityAttribute impl1("MyCode", "Ada", EntityAttribute::Type::Attribute);
    m_function->insertImplementation(0, impl1);
    EntityAttribute impl2("SimpleCode", "SDL", EntityAttribute::Type::Attribute);
    m_function->insertImplementation(1, impl2);
    m_function->setDefaultImplementation("MyCode");

    m_handler = std::make_unique<ive::ImplementationsHandler>(m_projectDir->path(), m_function.get());

    QDir dir;
    dir.mkpath(m_handler->implementationPath("MyCode", "Ada"));
    QFile::link(m_handler->functionBasePath()+"/Ada", dir.absolutePath());

    m_cmd = std::make_unique<ive::cmd::CmdFunctionImplementationDefaultChange>(m_projectDir->path(), m_function.get(), "SimpleCode", "SDL");
}

void tst_CmdFunctionImplementationDefaultChange::test_switchDefaultImplementation()
{
    const QString defaultTarget = m_handler->implementationPath("MyCode", "Ada");
    QFileInfo linkInfo(m_handler->functionBasePath() + "/Ada");

    QCOMPARE(linkInfo.exists(), true);
    QCOMPARE(linkInfo.isSymbolicLink(), true);
    QCOMPARE(linkInfo.symLinkTarget(), defaultTarget);

    m_cmd->redo();

    // old is gone
    linkInfo.refresh();
    QCOMPARE(linkInfo.exists(), false);
    //new is set up
    QFileInfo linkInfo2(m_handler->functionBasePath() + "/SDL");
    QCOMPARE(linkInfo2.exists(), true);
    QCOMPARE(linkInfo2.isSymbolicLink(), true);
    const QString newTarget = m_handler->implementationPath("SimpleCode", "SDL");
    QCOMPARE(linkInfo2.symLinkTarget(), newTarget);
}

void tst_CmdFunctionImplementationDefaultChange::test_undoDefaultImplementation()
{
    m_cmd->redo();
    m_cmd->undo();

    // back to default
    const QString defaultTarget = m_handler->implementationPath("MyCode", "Ada");
    QFileInfo linkInfo(m_handler->functionBasePath() + "/Ada");
    QCOMPARE(linkInfo.exists(), true);
    QCOMPARE(linkInfo.isSymbolicLink(), true);
    QCOMPARE(linkInfo.symLinkTarget(), defaultTarget);
    // old link is gone
    QFileInfo linkInfo2(m_handler->functionBasePath() + "/SDL");
    QCOMPARE(linkInfo2.exists(), false);
}

void tst_CmdFunctionImplementationDefaultChange::test_resetDefaultImplementation()
{
    m_cmd = std::make_unique<ive::cmd::CmdFunctionImplementationDefaultChange>(m_projectDir->path(), m_function.get(), "MyCode", "Ada");
    QCOMPARE(m_cmd->isObsolete(), true);
}

QTEST_MAIN(tst_CmdFunctionImplementationDefaultChange)

#include "tst_cmdfunctionimplementationdefaultchange.moc"
