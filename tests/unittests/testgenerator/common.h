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

namespace testgenerator {

/**
 * @brief  determine if elements pointed to by pointers from vector are equal, based on title()
 *
 * @param v1  vector of pointers to elements
 * @param i   index of the pointer from v1 to be compared
 * @param v2  vector of pointers to elements
 * @param j   index of the pointer from v2 to be compared
 *
 * @return  true if equal, false otherwise
 */
static auto elementsEqualByTitle = [](const auto &v1, int i, const auto &v2, int j) -> bool {
    return v1.at(i)->title().compare(v2.at(j)->title()) == 0;
};

/**
 * @brief  determine if given vector elements are equal, based on name()
 *
 * @param v1  vector of elements
 * @param i   index of the element from v1 to be compared
 * @param v2  vector of elements
 * @param j   index of the element from v2 to be compare
 *
 * @return  true if equal, false otherwise
 */
static auto elementsEqualByName = [](const auto &v1, int i, const auto &v2, int j) -> bool {
    return v1.at(i).name().compare(v2.at(j).name()) == 0;
};

/**
 * @brief  create map from one QVector to another, using provided function
 *
 * @tparam QVectorT     type of QVector
 * @param source        source vector (mapping FROM)
 * @param destination   destination vector (mapping TO)
 * @param elementsEqual comparison function
 *
 * @return map (a vector with indexes of corresponding `destination` elements on `source` indexes)
 */
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

} // namespace testgenerator
