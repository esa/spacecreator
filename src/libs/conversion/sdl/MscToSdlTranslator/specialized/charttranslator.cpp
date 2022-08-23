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

#include "specialized/charttranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <msccore/msccondition.h>

using conversion::translator::TranslationException;
using msc::MscChart;
using msc::MscCondition;
using msc::MscEntity;
using msc::MscInstanceEvent;
using sdl::SdlModel;

namespace conversion::sdl::translator {

ChartTranslator::ChartTranslator(SdlModel *sdlModel, const Options &options)
    : m_neverSequenceTranslator(sdlModel, options)
{
}

void ChartTranslator::translateChart(const MscChart *mscChart)
{
    const auto &mscEvents = mscChart->instanceEvents();

    const auto observerType = getObserverType(mscEvents);
    switch (observerType) {
    case ObserverType::Never:
        m_neverSequenceTranslator.createObserver(mscChart);
        break;
    case ObserverType::When: {
        auto errorMessage = QString("Cannot translate chart '%1' - \"when\" observers are not yet implemented")
                                    .arg(mscChart->name());
        throw TranslationException(std::move(errorMessage));
    } break;
    case ObserverType::None: {
        auto errorMessage = QString(
                "Cannot translate chart '%1' - first chart item is not a \"never\" nor \"when\" shared condition")
                                    .arg(mscChart->name());
        throw TranslationException(std::move(errorMessage));
    } break;
    };
}

ObserverType ChartTranslator::getObserverType(const QVector<MscInstanceEvent *> &mscEvents) const
{
    const auto firstMscEvent = mscEvents.front();

    if (firstMscEvent->entityType() != MscEntity::EntityType::Condition) {
        return ObserverType::None;
    }

    const auto &mscCondition = dynamic_cast<const MscCondition *>(firstMscEvent);

    if (!mscCondition->shared()) {
        return ObserverType::None;
    }

    const auto &mscConditionName = mscCondition->name();

    if (mscConditionName.toLower() == m_observerNameNever) {
        return ObserverType::Never;
    } else if (mscConditionName.toLower() == m_observerNameWhen) {
        return ObserverType::When;
    } else {
        return ObserverType::None;
    }
}

} // namespace conversion::sdl::translator
