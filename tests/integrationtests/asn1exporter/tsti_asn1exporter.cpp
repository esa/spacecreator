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
#include <QUuid>
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Exporter/Asn1Options/options.h>
#include <conversion/asn1/Asn1Exporter/exporter.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <memory>

using Asn1Acn::Asn1Model;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::asn1::exporter::Asn1Exporter;

namespace asn1::test {

class tsti_Asn1Exporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanup();
    void testValid();

private:
    //     IVPropertyTemplateConfig *m_config;
};

void tsti_Asn1Exporter::initTestCase()
{
    //     ivm::initIVLibrary();
    //     ive::initIVEditor();
    //     m_config = IVPropertyTemplateConfig::instance();
    //     m_config->init("config.xml");
}

void tsti_Asn1Exporter::cleanup()
{
    QFile file("output.asn");
    file.remove();
}

void tsti_Asn1Exporter::testValid()
{
    QFileInfo outputFileInfo("output.asn");
    QVERIFY(!outputFileInfo.exists());

    auto *function = new ivm::IVFunction(nullptr, QUuid::createUuid());
    Asn1Model model(m_config);
    model.addObject(function);

    Options options;
    options.add(Asn1Options::outputFilename, "output.asn");

    Asn1Exporter asn1Exporter;

    try {
        asn1Exporter.exportModel(&model, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QVERIFY(outputFileInfo.exists());
}

} // namespace asn1::test

QTEST_MAIN(asn1::test::tsti_Asn1Exporter)

#include "tsti_asn1exporter.moc"
