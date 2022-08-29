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

#include "observertype.h"
#include "specialized/neversequencetranslator.h"
#include "specialized/whensequencetranslator.h"

#include <asn1library/asn1/file.h>
#include <ivcore/ivmodel.h>
#include <msccore/mscchart.h>
#include <sdl/SdlModel/sdlmodel.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator for MSC charts
 */
class ChartTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel            Parent SDL model
     * @param   observerAsn1File    ASN.1 with observer dataview
     * @param   ivModel             IV model
     * @param   options             Conversion options
     */
    ChartTranslator(::sdl::SdlModel *sdlModel, const Asn1Acn::File *observerAsn1File, const ivm::IVModel *ivModel,
            const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    ChartTranslator(const ChartTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    ChartTranslator(ChartTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    ChartTranslator &operator=(const ChartTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    ChartTranslator &operator=(ChartTranslator &&) = delete;

public:
    /**
     * @brief   Translate given chart and add the result to the SDL model
     *
     * @param   mscChart    Chart to translate
     */
    auto translateChart(const msc::MscChart *mscChart) -> void;

private:
    inline static const QString m_observerNameNever = "never";
    inline static const QString m_observerNameWhen = "when";

    auto getObserverType(const QVector<msc::MscInstanceEvent *> &mscEvents) const -> ObserverType;

private:
    NeverSequenceTranslator m_neverSequenceTranslator;
    WhenSequenceTranslator m_whenSequenceTranslator;
};

} // namespace conversion::sdl::translator
