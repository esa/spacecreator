/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "patchersnippetsgenerator.h"

#include <QTextStream>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator {

PatcherSnippetsGenerator::PatcherSnippetsGenerator(
        Context &context, const seds::model::ContainerDataType &container, Asn1Acn::Types::Sequence *sequence)
    : m_context(context)
    , m_container(container)
    , m_sequence(sequence)
{
}

void PatcherSnippetsGenerator::generate()
{
    generatePatcherSnippets(m_container);

    if (!m_sequence->patcherSnippets().empty()) {
        const auto &sequenceName = m_sequence->identifier();

        m_sequence->setPostEncodingFunction(m_encodingFunctionTemplate.arg(sequenceName.toLower()));
        m_sequence->setPostDecodingValidator(m_decodingValidatorTemplate.arg(sequenceName).toLower());
    }
}

void PatcherSnippetsGenerator::generatePatcherSnippets(const seds::model::ContainerDataType &container)
{
    if (container.baseType()) {
        const auto baseType = m_context.findSedsType(*container.baseType());
        const auto baseContainerType = std::get_if<seds::model::ContainerDataType>(baseType);

        if (baseContainerType == nullptr) {
            auto errorMessage = QString("Container '%1' has a base type '%2' that is not a container")
                                        .arg(container.nameStr())
                                        .arg(dataTypeNameStr(*baseType));
            throw TranslationException(std::move(errorMessage));
        }

        generatePatcherSnippets(*baseContainerType);
    }

    for (const auto &entry : container.entries()) {
        // clang-format off
        std::visit(overloaded {
            [&](const seds::model::ErrorControlEntry &errorControlEntry) {
                buildErrorControlEntryFunction(errorControlEntry);
            },
            [&](const seds::model::LengthEntry &lengthEntry) {
                buildLengthEntryFunction(lengthEntry);
            },
            [&](const auto &e) {
                Q_UNUSED(e)
            }
        }, entry);
        // clang-format on
    }
}

void PatcherSnippetsGenerator::buildErrorControlEntryFunction(const seds::model::ErrorControlEntry &errorControlEntry)
{
    const auto bitCount = getErrorControlBitCount(errorControlEntry);

    auto encodingFunction = buildErrorControlEntryEncodingFunction(errorControlEntry, bitCount);
    auto decodingValidator = buildErrorControlEntryDecodingValidator(errorControlEntry, bitCount);

    m_sequence->addPatcherSnippet({ std::move(encodingFunction), std::move(decodingValidator) });
}

void PatcherSnippetsGenerator::buildLengthEntryFunction(const seds::model::LengthEntry &lengthEntry)
{
    const auto entryName = Escaper::escapeCFieldName(lengthEntry.nameStr());
    const auto &encoding = getLengthEncoding(lengthEntry);

    auto encodingFunction = buildLengthEntryEncodingFunction(encoding, entryName);
    auto decodingValidator = buildLengthEntryDecodingValidator(encoding, entryName);

    m_sequence->addPatcherSnippet({ std::move(encodingFunction), std::move(decodingValidator) });
}

QString PatcherSnippetsGenerator::buildErrorControlEntryEncodingFunction(
        const seds::model::ErrorControlEntry &entry, const uint64_t bitCount) const
{
    const auto sequenceName = Escaper::escapeCSequenceName(m_container.nameStr());
    const auto entryName = Escaper::escapeCFieldName(entry.nameStr());

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

    // clang-format off
    stream << "(pNullPos->" << sequenceName << "_" + entryName << ".buf, "
           << "pNullPos->" << sequenceName << "_" + entryName << ".currentByte);\n"
           << "Acn_Enc_Int_PositiveInteger_ConstSize("
           << "&pNullPos->" << sequenceName << "_" << entryName << ", "
           << "crcResult, "
           << bitCount
           << ");\n";
    // clang-format on

    return buffer;
}

QString PatcherSnippetsGenerator::buildErrorControlEntryDecodingValidator(
        const seds::model::ErrorControlEntry &entry, const uint64_t bitCount) const
{
    const auto sequenceName = Escaper::escapeCSequenceName(m_container.nameStr());
    const auto entryName = Escaper::escapeCFieldName(entry.nameStr());

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

    // clang-format off
    stream << "(pNullPos->" << sequenceName << "_" + entryName << ".buf, "
           << "pNullPos->" << sequenceName << "_" + entryName << ".currentByte);\n"
           << "asn1SccUint decodedCrc = 0;\n"
           << "ret = Acn_Dec_Int_PositiveInteger_ConstSize("
           << "&pNullPos->" << sequenceName << "_" << entryName << ", "
           << "&decodedCrc, "
           << bitCount
           << ");\n"
           << "if(!ret || crcResult != decodedCrc) {\n"
           << "\t*pErrCode = 1;\n"
           << "\treturn false;\n"
           << "}\n";
    // clang-format on

    return buffer;
}

QString PatcherSnippetsGenerator::buildLengthEntryEncodingFunction(
        const seds::model::IntegerDataEncoding &encoding, const QString &entryName) const
{
    const auto sequenceName = Escaper::escapeCSequenceName(m_container.nameStr());

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

QString PatcherSnippetsGenerator::buildLengthEntryDecodingValidator(
        const seds::model::IntegerDataEncoding &encoding, const QString &entryName) const
{
    const auto sequenceName = Escaper::escapeCSequenceName(m_container.nameStr());

    QString buffer;
    QTextStream stream(&buffer, QIODevice::WriteOnly);

    stream << "asn1SccUint decodedLengthInBytes = 0;\n"
           << "ret = Acn_Dec_Int_";

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
           << "&decodedLengthInBytes, "
           << encoding.bits()
           << ");\n"
           << "if(!ret || lengthInBytes != decodedLengthInBytes) {\n"
           << "\t*pErrCode = 1;\n"
           << "\treturn false;\n"
           << "}\n"
          ;
    // clang-format on

    return buffer;
}

uint64_t PatcherSnippetsGenerator::getErrorControlBitCount(const seds::model::ErrorControlEntry &entry) const
{
    const auto entryType = m_context.findSedsType(entry.type());
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
const seds::model::IntegerDataEncoding &PatcherSnippetsGenerator::getLengthEncoding(
        const seds::model::LengthEntry &entry) const
{
    const auto entryType = m_context.findSedsType(entry.type());
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
