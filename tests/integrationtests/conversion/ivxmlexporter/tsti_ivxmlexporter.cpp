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
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlExporter/exporter.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <libiveditor/iveditor.h>
#include <memory>

using conversion::Options;
using conversion::iv::IvOptions;
using conversion::iv::exporter::IvXmlExporter;
using ivm::IVModel;
using ivm::IVPropertyTemplateConfig;

namespace conversion::iv::test {

class tsti_IvXmlExporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanup();
    void testValid();

private:
    IVPropertyTemplateConfig *m_config;
};

void tsti_IvXmlExporter::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
    m_config = IVPropertyTemplateConfig::instance();
    m_config->init("config.xml");
}

void tsti_IvXmlExporter::cleanup()
{
    QFile file("output.xml");
    file.remove();
}

void tsti_IvXmlExporter::testValid()
{
    QFileInfo outputFileInfo("output.xml");
    QVERIFY(!outputFileInfo.exists());

    auto *function = new ivm::IVFunction(nullptr, QUuid::createUuid());
    IVModel model(m_config);
    model.addObject(function);

    Options options;
    options.add(IvOptions::outputFilepath, "output.xml");

    IvXmlExporter ivExporter;

    try {
        ivExporter.exportModel(&model, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QVERIFY(outputFileInfo.exists());
}

} // namespace conversion::iv::test

QTEST_MAIN(conversion::iv::test::tsti_IvXmlExporter)

#include "tsti_ivxmlexporter.moc"
