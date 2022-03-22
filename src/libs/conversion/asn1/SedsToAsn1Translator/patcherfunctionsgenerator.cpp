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
                auto patcherFunction = buildErrorControlEntryFunction(errorControlEntry);
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
        const seds::model::ErrorControlEntry &errorControlEntry) const
{
    auto encodingFunction = buildErrorControlEntryEncodingFunction(errorControlEntry);
    auto decodingValidator = buildErrorControlEntryDecodingValidator(errorControlEntry);

    return { std::move(encodingFunction), std::move(decodingValidator) };
}

Asn1Acn::PatcherFunction PatcherFunctionsGenerator::buildLengthEntryFunction(
        const seds::model::LengthEntry &lengthEntry, const QString &sequenceName) const
{
    const auto &encoding = getLengthEntryEncoding(lengthEntry);

    auto encodingFunction = buildLengthEntryEncodingFunction(encoding, sequenceName, lengthEntry.nameStr());
    auto decodingValidator = buildLengthEntryDecodingValidator(encoding);

    return { std::move(encodingFunction), std::move(decodingValidator) };
}

QString PatcherFunctionsGenerator::buildErrorControlEntryEncodingFunction(
        const seds::model::ErrorControlEntry &errorControlEntry) const
{
    Q_UNUSED(errorControlEntry);
    return "";
}

QString PatcherFunctionsGenerator::buildErrorControlEntryDecodingValidator(
        const seds::model::ErrorControlEntry &errorControlEntry) const
{
    Q_UNUSED(errorControlEntry);
    return "";
}

QString PatcherFunctionsGenerator::buildLengthEntryEncodingFunction(const seds::model::IntegerDataEncoding &encoding,
        const QString &sequenceName, const QString &lengthEntryName) const
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
                case seds::model::CoreIntegerEncoding::OnesComplement:
                case seds::model::CoreIntegerEncoding::PackedBcd:
                case seds::model::CoreIntegerEncoding::SignMagnitude:
                default: {
                    auto errorMessage = QString("Unsupported encoding for length entry \"%1\" - use unsigned, two's complement or BCD").arg(lengthEntryName);
                    throw TranslationException(std::move(errorMessage));
                } break;
            }
        }
    }, encoding.encoding());
    // clang-format on

    // clang-format off
    stream << "_ConstSize("
           << "&pNullPos->" << sequenceName << "_" << lengthEntryName << ", "
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

const seds::model::IntegerDataEncoding &PatcherFunctionsGenerator::getLengthEntryEncoding(
        const seds::model::LengthEntry &lengthEntry) const
{
    const auto &lengthEntryTypeRef = lengthEntry.type();

    const auto sedsPackage = lengthEntryTypeRef.packageStr()
            ? SedsToAsn1Translator::getSedsPackage(*lengthEntryTypeRef.packageStr(), m_sedsPackages)
            : m_sedsPackage;

    const auto lengthEntryType = sedsPackage->dataType(lengthEntryTypeRef.nameStr());

    const auto lengthEntryIntegerType = std::get_if<seds::model::IntegerDataType>(lengthEntryType);

    if (!lengthEntryIntegerType) {
        auto errorMessage =
                QString("Length entry \"%1\" has to have integer as an underlying type").arg(lengthEntry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto &lengthEntryIntegerEncoding = lengthEntryIntegerType->encoding();

    if (!lengthEntryIntegerEncoding.has_value()) {
        auto errorMessage = QString("Length entry \"%1\" has to have encoding").arg(lengthEntry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return lengthEntryIntegerEncoding.value();
}

} // namespace conversion::asn1::translator
