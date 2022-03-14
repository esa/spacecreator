/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "../common.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <memory>
#include <qobjectdefs.h>

// GdbConnector includes
#include <QProcess>

// TODO: remove this include
#include <QDebug>

namespace tests::testgenerator {

class tst_gdbconnector final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
};

void tst_gdbconnector::initTestCase()
{
    //
}

namespace gdbconnector {

std::unique_ptr<QProcess> makeAndStartGdbServer(
        const QString &server, const QStringList &serverArgs, const QString &binaryLocalization)
{
    // 0. run gdb server on target:
    auto gdbserver = std::make_unique<QProcess>();
    gdbserver->setProgram(server);
    gdbserver->setArguments(serverArgs);
    gdbserver->setWorkingDirectory(binaryLocalization);
    gdbserver->start();
    gdbserver->waitForStarted();

    return gdbserver;
}

} // namespace gdbconnector

QString getTestResults()
{
    const QString server = "gdbserver";
    const QString serverNamePort = "host:1234";
    const QString programToRun = "hostpartition";
    const QString binaryLocalization = "/home/taste/example-projects/testharness/work/binaries";

    const QString debugger = "gdb";
    const QString script = "x86-linux-cpp.gdb";
    const QStringList clientArgs({ "-batch", "-x", script });

    const std::unique_ptr<QProcess> gdbserver =
            gdbconnector::makeAndStartGdbServer(server, { serverNamePort, programToRun }, binaryLocalization);

    // 1. execute script on a specified console application
    QProcess client;
    client.setProgram(debugger);
    client.setArguments(clientArgs);
    client.start();
    client.waitForFinished();

    gdbserver->close();

    const auto out = QString(client.readAllStandardOutput());
    client.close();

    const QStringList outList = out.split("\n");

    if (outList.length() > 2) {
        const int oneBeforeLast = outList.length() - 2;
        return outList.at(oneBeforeLast);
    } else {
        return "";
    }
}

void tst_gdbconnector::testNominal()
{
    const auto res = getTestResults();

    qDebug() << " >>> results: " << res;

    QFAIL("this shall happen");
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_gdbconnector)

#include "tst_gdbconnector.moc"
