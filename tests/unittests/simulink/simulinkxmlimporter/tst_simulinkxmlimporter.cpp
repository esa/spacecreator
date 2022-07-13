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

#include <simulink/SimulinkXMLImporter/importer.h>
#include <simulink/SimulinkOptions/options.h>

using simulink::importer::SimulinkXmlImporter;
using conversion::simulink::SimulinkOptions;

namespace simulink::test {

class tst_SimulinkXmlImporter : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_SimulinkXmlImporter() = default;

private Q_SLOTS:
    void testImporter();
};

void tst_SimulinkXmlImporter::testImporter()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    try {
        importer.importModel(options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

} // namespace simulink::test

QTEST_MAIN(simulink::test::tst_SimulinkXmlImporter)

#include "tst_simulinkxmlimporter.moc"
