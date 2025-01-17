/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include "../acnparameter.h"

#include <QString>
#include <memory>
#include <vector>

namespace Asn1Acn {
namespace Types {

template<typename T>
class AcnParameterizableCollection
{
public:
    AcnParameterizableCollection() = default;
    AcnParameterizableCollection(const AcnParameterizableCollection<T> &other);

    using Components = std::vector<std::unique_ptr<T>>;

    const Components &components() const { return m_components; }
    Components &components() { return m_components; }
    const T *component(const QString &name) const;
    T *component(const QString &name);
    void addComponent(std::unique_ptr<T> component);

    const AcnParameterPtrs &acnParameters() const { return m_parameters; }
    void addParameter(AcnParameterPtr parameter);

protected:
    Components m_components;
    AcnParameterPtrs m_parameters;
};

template<typename T>
inline const T *AcnParameterizableCollection<T>::component(const QString &name) const
{
    auto found = find_if(
            m_components.begin(), m_components.end(), [name](const auto &item) { return item->name() == name; });

    return found == m_components.end() ? nullptr : (*found).get();
}

template<typename T>
inline T *AcnParameterizableCollection<T>::component(const QString &name)
{
    auto found = find_if(
            m_components.begin(), m_components.end(), [name](const auto &item) { return item->name() == name; });

    return found == m_components.end() ? nullptr : (*found).get();
}

template<typename T>
inline AcnParameterizableCollection<T>::AcnParameterizableCollection(const AcnParameterizableCollection<T> &other)
{
    for (const auto &parameter : other.m_parameters)
        addParameter(std::make_unique<AcnParameter>(*parameter));

    for (const auto &component : other.m_components)
        addComponent(component->clone());
}

template<typename T>
inline void AcnParameterizableCollection<T>::addComponent(std::unique_ptr<T> component)
{
    m_components.push_back(std::move(component));
}

template<typename T>
inline void AcnParameterizableCollection<T>::addParameter(AcnParameterPtr parameter)
{
    m_parameters.push_back(std::move(parameter));
}

}
}
