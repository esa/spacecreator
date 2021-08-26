/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QFileInfo>
#include <QObject>
#include <QtTest>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlExporter/exporter.h>
#include <ivcore/ivmodel.h>

using conversion::Options;
using conversion::iv::exporter::IvXmlExporter;
using ivm::IVModel;

namespace iv::test {

class tsti_IvXmlExporter : public QObject
{
    Q_OBJECT

public:
    virtual ~tsti_IvXmlExporter() = default;

private Q_SLOTS:
    void testValid();
};

void tsti_IvXmlExporter::testValid()
{
    Options options;

    IvXmlExporter ivExporter;

    try {
        ivExporter.exportModel(nullptr, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

} // namespace iv::test

QTEST_MAIN(iv::test::tsti_IvXmlExporter)

#include "tsti_ivxmlexporter.moc"
