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

#pragma once

#include "specialized/charttranslator.h"

#include <asn1library/asn1/file.h>
#include <ivcore/ivmodel.h>
#include <msccore/mscdocument.h>
#include <sdl/SdlModel/sdlmodel.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator for MSC documents
 */
class DocumentTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   asn1File    ASN.1 file
     * @param   ivModel     IV model
     * @param   options     Conversion options
     */
    DocumentTranslator(::sdl::SdlModel *sdlModel, const Asn1Acn::File *asn1File, const ivm::IVModel *ivModel,
            const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    DocumentTranslator(const DocumentTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    DocumentTranslator(DocumentTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    DocumentTranslator &operator=(const DocumentTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    DocumentTranslator &operator=(DocumentTranslator &&) = delete;

public:
    /**
     * @brief   Translate given document and add the result to the SDL model
     *
     * @param   mscDocument     Document to translate
     */
    auto translateDocument(const msc::MscDocument *mscDocument) -> void;

private:
    ChartTranslator m_chartTranslator;
};

} // namespace conversion::sdl::translator
