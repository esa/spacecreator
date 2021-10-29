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

#include "testbase.h"

#include <QtTest>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1reader.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/converter/converter.h>
#include <conversion/seds/SedsRegistrar/registrar.h>
#include <seds/SedsOptions/options.h>

using namespace conversion;
using namespace conversion::asn1;
using namespace conversion::seds;

namespace conversion::asn1::test {

void TestBase::initTestCase()
{
    Asn1Registrar asn1Registrar;
    auto result = asn1Registrar.registerCapabilities(m_registry);
    if (!result) {
        QFAIL("Failed to register ASN.1 capabilities");
    }

    SedsRegistrar sedsRegistrar;
    result = sedsRegistrar.registerCapabilities(m_registry);
    if (!result) {
        QFAIL("Failed to register SEDS capabilities");
    }
}

void TestBase::cleanup()
{
    const auto asnFilename = QString("%1.asn").arg(m_currentTestFilename);
    const auto acnFilename = QString("%1.acn").arg(m_currentTestFilename);

    QFile(asnFilename).remove();
    QFile(acnFilename).remove();
}

void TestBase::runConversionTest(QString testFilenameBase)
{
    m_currentTestFilename = std::move(testFilenameBase);

    const auto sedsFilename = QString("%1.xml").arg(m_currentTestFilename);
    const auto asnFilename = QString("%1.asn").arg(m_currentTestFilename);
    const auto acnFilename = QString("%1.acn").arg(m_currentTestFilename);

    Options options;
    options.add(SedsOptions::inputFilename, sedsFilename);
    options.add(SedsOptions::skipValidation);

    Converter converter(m_registry, std::move(options));
    converter.convert({ ModelType::Seds }, ModelType::Asn1, {});

    QVERIFY(QFile(asnFilename).exists());
    QVERIFY(QFile(acnFilename).exists());

    validateAsn1(asnFilename);
}

void TestBase::validateAsn1(const QString &filename)
{
    Asn1Acn::Asn1Reader asn1Reader;
    QObject::connect(&asn1Reader, &Asn1Acn::Asn1Reader::parseError,
            [](const QString &error) { QFAIL(error.toStdString().c_str()); });

    QStringList errors;
    auto asn1File = asn1Reader.parseAsn1File(".", filename, &errors);

    if (!errors.isEmpty()) {
        for (const auto &error : errors) {
            QWARN(error.toStdString().c_str());
        }

        QFAIL("ASN.1 parsing errors");
    }
}

} // namespace conversion::asn1::test
