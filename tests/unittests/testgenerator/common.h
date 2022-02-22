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

#include <QVector>
#include <functional>

static auto elementsEqualByTitle = [](const auto &v1, int i, const auto &v2, int j) -> bool {
    return v1.at(i)->title().compare(v2.at(j)->title()) == 0;
};

static auto elementsEqualByName = [](const auto &v1, int i, const auto &v2, int j) -> bool {
    return v1.at(i).name().compare(v2.at(j).name()) == 0;
};

template<typename QVectorT>
static QVector<int> createQVectorToQVectorMap(const QVectorT &source, const QVectorT &destination,
        std::function<bool(const QVectorT &source, int i, const QVectorT &destination, int j)> elementsEqual)
{
    assert(source.size() == destination.size());

    const int size = source.size();

    QVector<int> map(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (elementsEqual(source, i, destination, j)) {
                map[i] = j;
            }
        }
    }

    return map;
}
