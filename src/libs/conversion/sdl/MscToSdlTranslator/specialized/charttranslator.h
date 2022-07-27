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
#include "specialized/neverobservertranslator.h"

#include <msccore/mscchart.h>
#include <sdl/SdlModel/sdlmodel.h>

namespace conversion::sdl::translator {

class ChartTranslator final
{
public:
    ChartTranslator(::sdl::SdlModel *sdlModel);

    ChartTranslator(const ChartTranslator &) = delete;
    ChartTranslator(ChartTranslator &&) = delete;

    ChartTranslator &operator=(const ChartTranslator &) = delete;
    ChartTranslator &operator=(ChartTranslator &&) = delete;

public:
    auto translateChart(const msc::MscChart *mscChart) const -> void;

private:
    inline static const QString m_observerNameNever = "never";
    inline static const QString m_observerNameWhen = "when";

    auto getObserverType(const QVector<msc::MscInstanceEvent *> &mscEvents) const -> ObserverType;

private:
    NeverObserverTranslator m_neverObserverTranslator;
};

} // namespace conversion::sdl::translator
