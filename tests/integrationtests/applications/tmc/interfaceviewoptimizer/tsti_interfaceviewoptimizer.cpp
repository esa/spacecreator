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

#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc {

class tsti_InterfaceViewOptimizer : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_interfaceViewOptimizer();
    void test_interfaceViewOptimizer_standalone();
    void test_interfaceViewOptimizer_keep();
    void test_interfaceViewOptimizer_ergo_agent();
    void test_interfaceViewOptimizer_ergo_battery();
    void test_interfaceViewOptimizer_only_one_function();
};

void tsti_InterfaceViewOptimizer::test_interfaceViewOptimizer()
{
    const int result = system("./test_interface_view_optimizer.sh");
    QCOMPARE(result, 0);
}

void tsti_InterfaceViewOptimizer::test_interfaceViewOptimizer_standalone()
{
    const int result = system("./test_interface_view_optimizer_standalone.sh");
    QCOMPARE(result, 0);
}

void tsti_InterfaceViewOptimizer::test_interfaceViewOptimizer_keep()
{
    const int result = system("./test_interface_view_optimizer_keep.sh");
    QCOMPARE(result, 0);
}

void tsti_InterfaceViewOptimizer::test_interfaceViewOptimizer_ergo_agent()
{
    const int result = system("./test_interface_view_optimizer_ergo_agent.sh");
    QCOMPARE(result, 0);
}

void tsti_InterfaceViewOptimizer::test_interfaceViewOptimizer_ergo_battery()
{
    const int result = system("./test_interface_view_optimizer_ergo_battery.sh");
    QCOMPARE(result, 0);
}

void tsti_InterfaceViewOptimizer::test_interfaceViewOptimizer_only_one_function()
{
    const int result = system("./test_only_one_function_with_sync_interfaces.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(tmc::tsti_InterfaceViewOptimizer)

#include "tsti_interfaceviewoptimizer.moc"
