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

#include "commands/cmdfunctionimplementationremove.h"
#include "implementationshandler.h"
#include "ivcommonprops.h"
#include "iveditor.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivpropertytemplateconfig.h"
#include "standardpaths.h"

#include <QDir>
#include <QObject>
#include <QTemporaryDir>
#include <QtTest>

class tst_CmdFunctionImplementationRemove : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void test_removeImplementation();
    void test_removeUndo();
    void test_removeDefaultImplementation();

private:
    std::unique_ptr<QTemporaryDir> m_projectDir;
    std::unique_ptr<ivm::IVFunction> m_function;
    std::unique_ptr<ive::cmd::CmdFunctionImplementationRemove> m_cmd;
    std::unique_ptr<ive::ImplementationsHandler> m_handler;
};

void tst_CmdFunctionImplementationRemove::initTestCase()
{
    shared::StandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
    ive::initIVEditor();
    auto dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    dynPropConfig->init(shared::interfaceCustomAttributesFilePath());
}

void tst_CmdFunctionImplementationRemove::init()
{
    m_projectDir = std::make_unique<QTemporaryDir>();
    m_function = std::make_unique<ivm::IVFunction>();
    m_function->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), "MyFunc");
    EntityAttribute impl("MyCode", "Ada", EntityAttribute::Type::Attribute);
    m_function->insertImplementation(0, impl);

    m_handler = std::make_unique<ive::ImplementationsHandler>(m_projectDir->path(), m_function.get());

    QDir dir;
    dir.mkpath(m_handler->implementationPath("MyCode", "Ada"));

    m_cmd = std::make_unique<ive::cmd::CmdFunctionImplementationRemove>(m_projectDir->path(), m_function.get(), 0);
}

void tst_CmdFunctionImplementationRemove::test_removeImplementation()
{
    QCOMPARE(m_function->implementations().size(), 1);
    QDir dir(m_handler->functionImplementaionsBasePath());
    QCOMPARE(dir.isEmpty(), false);

    m_cmd->redo();
    QCOMPARE(m_function->implementations().size(), 0);
    QCOMPARE(dir.isEmpty(), true);
}

void tst_CmdFunctionImplementationRemove::test_removeUndo()
{
    m_cmd->redo();
    m_cmd->undo();

    QCOMPARE(m_function->implementations().size(), 1);
    QDir dir(m_handler->functionImplementaionsBasePath());
    QCOMPARE(dir.isEmpty(), false);
}

void tst_CmdFunctionImplementationRemove::test_removeDefaultImplementation()
{
    m_function->setDefaultImplementation("MyCode");
    m_cmd = std::make_unique<ive::cmd::CmdFunctionImplementationRemove>(m_projectDir->path(), m_function.get(), 0);
    QCOMPARE(m_cmd->isObsolete(), true);
}

QTEST_MAIN(tst_CmdFunctionImplementationRemove)

#include "tst_cmdfunctionimplementationremove.moc"
