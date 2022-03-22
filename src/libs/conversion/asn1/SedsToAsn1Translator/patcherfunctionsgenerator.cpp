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

#include "patcherfunctionsgenerator.h"

#include <QTextStream>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/types/integerdatatype.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator {

PatcherFunctionsGenerator::PatcherFunctionsGenerator(
        const seds::model::Package *sedsPackage, const std::vector<seds::model::Package> &sedsPackages)
    : m_sedsPackage(sedsPackage)
    , m_sedsPackages(sedsPackages)
{
}

std::vector<Asn1Acn::PatcherFunction> PatcherFunctionsGenerator::generate(
        const seds::model::ContainerDataType &sedsType) const
{
    const auto sequenceName = Escaper::escapeCSequenceName(sedsType.nameStr());

    std::vector<Asn1Acn::PatcherFunction> result;

    for (const auto &entry : sedsType.entries()) {
        // clang-format off
        std::visit(overloaded {
            [&](const seds::model::ErrorControlEntry &errorControlEntry) {
                auto patcherFunction = buildErrorControlEntryFunction(errorControlEntry, sequenceName);
                result.push_back(std::move(patcherFunction));
            },
            [&](const seds::model::LengthEntry &lengthEntry) {
                auto patcherFunction = buildLengthEntryFunction(lengthEntry, sequenceName);
                result.push_back(std::move(patcherFunction));
            },
            [&](const auto &e) {
                Q_UNUSED(e)
            }
        }, entry);
        // clang-format on
    }

    return result;
}

Asn1Acn::PatcherFunction PatcherFunctionsGenerator::buildErrorControlEntryFunction(
        const seds::model::ErrorControlEntry &errorControlEntry, const QString &sequenceName) const
{
    const auto bits = getErrorControlBitsCount(errorControlEntry);

    auto encodingFunction = buildErrorControlEntryEncodingFunction(errorControlEntry, bits, sequenceName);
    auto decodingValidator = buildErrorControlEntryDecodingValidator(errorControlEntry, bits);

    return { std::move(encodingFunction), std::move(decodingValidator) };
}

Asn1Acn::PatcherFunction PatcherFunctionsGenerator::buildLengthEntryFunction(
        const seds::model::LengthEntry &lengthEntry, const QString &sequenceName) const
{
    const auto &encoding = getLengthEncoding(lengthEntry);

    auto encodingFunction = buildLengthEntryEncodingFunction(encoding, sequenceName, lengthEntry.nameStr());
    auto decodingValidator = buildLengthEntryDecodingValidator(encoding);

    return { std::move(encodingFunction), std::move(decodingValidator) };
}

QString PatcherFunctionsGenerator::buildErrorControlEntryEncodingFunction(
        const seds::model::ErrorControlEntry &entry, const uint64_t bits, const QString &sequenceName) const
{
    Q_UNUSED(bits);

    QString buffer;
    QTextStream stream(&buffer, QIODevice::WriteOnly);

    stream << "asn1SccUint crcResult = calculate";

    // clang-format off
    std::visit(overloaded {
        [&](const seds::model::CoreErrorControl &coreErrorControl) {
            switch(coreErrorControl) {
                case seds::model::CoreErrorControl::Crc16:
                    stream << "Crc16";
                    break;
                case seds::model::CoreErrorControl::Crc8:
                    stream << "Crc8";
                    break;
                case seds::model::CoreErrorControl::Checksum:
                    stream << "Checksum";
                    break;
                case seds::model::CoreErrorControl::ChecksumLongitundinal:
                    stream << "ChecksumLongitundinal";
                    break;
                default: {
                    auto errorMessage = QString("Unsupported encoding for error control entry entry \"%1\" - use unsigned, two's complement or BCD").arg(entry.nameStr());
                    throw TranslationException(std::move(errorMessage));
                } break;
            }
        }
    }, entry.errorControl());
    // clang-format on

    stream << "(pNullPos->" << sequenceName << "_" + entry.nameStr() << ".buf, "
           << "pNullPos->" << sequenceName << "_" + entry.nameStr() << ".currentByte);\n"
           << '\n'
           << "Acn_Enc_Int_PositiveInteger_ConstSize("
           << "&pNullPos->" << sequenceName << "_" << entry.nameStr() << ", "
           << "crcResult, " << bits << ");\n";

    return buffer;
}

QString PatcherFunctionsGenerator::buildErrorControlEntryDecodingValidator(
        const seds::model::ErrorControlEntry &entry, const uint64_t bits) const
{
    Q_UNUSED(entry);
    Q_UNUSED(bits);
    return "";
}

QString PatcherFunctionsGenerator::buildLengthEntryEncodingFunction(
        const seds::model::IntegerDataEncoding &encoding, const QString &sequenceName, const QString &entryName) const
{
    QString buffer;
    QTextStream stream(&buffer, QIODevice::WriteOnly);

    stream << "Acn_Enc_Int_";

    // clang-format off
    std::visit(overloaded {
        [&](const seds::model::CoreIntegerEncoding &coreEncoding) {
            switch(coreEncoding) {
                case seds::model::CoreIntegerEncoding::Unsigned:
                    stream << "PositiveInteger";
                    break;
                case seds::model::CoreIntegerEncoding::TwosComplement:
                    stream << "TwosComplement";
                    break;
                case seds::model::CoreIntegerEncoding::Bcd:
                    stream << "BCD";
                    break;
                default: {
                    auto errorMessage = QString("Unsupported encoding for length entry \"%1\" - use unsigned, two's complement or BCD").arg(entryName);
                    throw TranslationException(std::move(errorMessage));
                } break;
            }
        }
    }, encoding.encoding());
    // clang-format on

    // clang-format off
    stream << "_ConstSize("
           << "&pNullPos->" << sequenceName << "_" << entryName << ", "
           << "lengthInBytes, "
           << encoding.bits()
           << ");\n";
    // clang-format on

    return buffer;
}

QString PatcherFunctionsGenerator::buildLengthEntryDecodingValidator(
        const seds::model::IntegerDataEncoding &encoding) const
{
    Q_UNUSED(encoding);
    return "";
}

uint64_t PatcherFunctionsGenerator::getErrorControlBitsCount(const seds::model::ErrorControlEntry &entry) const
{
    const auto &entryTypeRef = entry.type();

    const auto sedsPackage = entryTypeRef.packageStr()
            ? SedsToAsn1Translator::getSedsPackage(*entryTypeRef.packageStr(), m_sedsPackages)
            : m_sedsPackage;

    const auto entryType = sedsPackage->dataType(entryTypeRef.nameStr());

    const auto entryBinaryType = std::get_if<seds::model::BinaryDataType>(entryType);

    if (!entryBinaryType) {
        auto errorMessage =
                QString("Error control entry \"%1\" has to have binary as an underlying type").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    if (!entryBinaryType->hasFixedSize()) {
        auto errorMessage =
                QString("Error control entry \"%1\" underlying type has to be fixed size").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return entryBinaryType->bits();
}

const seds::model::IntegerDataEncoding &PatcherFunctionsGenerator::getLengthEncoding(
        const seds::model::LengthEntry &entry) const
{
    const auto &entryTypeRef = entry.type();

    const auto sedsPackage = entryTypeRef.packageStr()
            ? SedsToAsn1Translator::getSedsPackage(*entryTypeRef.packageStr(), m_sedsPackages)
            : m_sedsPackage;

    const auto entryType = sedsPackage->dataType(entryTypeRef.nameStr());

    const auto entryIntegerType = std::get_if<seds::model::IntegerDataType>(entryType);

    if (!entryIntegerType) {
        auto errorMessage =
                QString("Length entry \"%1\" has to have integer as an underlying type").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto &entryIntegerEncoding = entryIntegerType->encoding();

    if (!entryIntegerEncoding.has_value()) {
        auto errorMessage = QString("Entry \"%1\" has to have encoding").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return entryIntegerEncoding.value();
}

} // namespace conversion::asn1::translator
