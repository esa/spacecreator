/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "model.h"
#include "modeltype.h"
#include "options.h"
#include "translation/exceptions.h"

#include <memory>
#include <set>
#include <vector>

namespace conversion::translator {

/**
 * @brief   Interface for converter translators
 */
class Translator
{
public:
    /**
     * @brief   Default constructor
     */
    Translator() = default;
    /**
     * @brief   Default destructor
     */
    virtual ~Translator() = default;
    /**
     * @brief   Deleted copy constructor
     */
    Translator(const Translator &) = delete;
    /**
     * @brief   Default move constructor
     */
    Translator(Translator &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Translator &operator=(const Translator &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Translator &operator=(Translator &&) = default;

public:
    /**
     * @brief   Translate given source model into a target model
     *
     * @param   sourceModels    Vector of source models
     * @param   options         Options for export configuration
     *
     * @return  Target model translated from the source model
     */
    virtual auto translateModels(std::vector<Model *> sourceModels, const Options &options) const
            -> std::vector<std::unique_ptr<Model>> = 0;

    /**
     * @brief   Return a model type that is a source of this translator
     *
     * @return  Source model type
     */
    virtual auto getSourceModelType() const -> ModelType = 0;
    /**
     * @brief   Returns a model type that is a target of this translator
     *
     * @return  Target model type
     */
    virtual auto getTargetModelType() const -> ModelType = 0;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    virtual auto getDependencies() const -> std::set<ModelType> = 0;
    /**
     * @brief   Provides a set of all source model types that are optional for the translation
     *
     * The default implementation returns empty set of dependencies.
     *
     * @return  Set of optional models
     */
    virtual auto getOptionalDependencies() const -> std::set<ModelType>;
    /**
     * @brief Get the specified model from the vector of models
     *
     * @tparam ModelT  type of required model
     * @param models   vector of models
     *
     * @return a model of the required type
     * @throws conversion::translator::TranslationException
     */
    template<typename ModelT>
    static auto getModel(const std::vector<Model *> &models) -> ModelT *;
    /**
     * @brief Get the specified model from the vector of models
     *
     * @tparam ModelT  type of required model
     * @param models   vector of models
     *
     * @return a model of the required type
     * @throws conversion::translator::TranslationException
     */
    template<typename ModelT>
    static auto getModel(const std::vector<std::unique_ptr<Model>> &models) -> ModelT *;

    /**
     * @brief Get the specified model from the vector of models
     *
     * @tparam ModelT  type of required model
     * @param models   vector of models
     *
     * @return a model of the required type or nullptr
     */
    template<typename ModelT>
    static auto getOptionalModel(const std::vector<Model *> &models) -> ModelT *;
    /**
     * @brief Get the specified model from the vector of models
     *
     * @tparam ModelT  type of required model
     * @param models   vector of models
     *
     * @return a model of the required type or nullptr
     */
    template<typename ModelT>
    static auto getOptionalModel(const std::vector<std::unique_ptr<Model>> &models) -> ModelT *;

protected:
    auto checkSourceModelCount(const std::vector<Model *> &models) const -> void;
};

template<typename ModelT>
ModelT *Translator::getModel(const std::vector<Model *> &models)
{
    std::vector<Model *> foundModels;
    std::copy_if(models.begin(), models.end(), std::back_inserter(foundModels),
            [](Model *model) { return dynamic_cast<ModelT *>(model) != nullptr; });

    if (foundModels.empty()) {
        auto message = QString("Missing source %1 model").arg(ModelProperties<ModelT>::name);
        throw conversion::translator::TranslationException(std::move(message));
    } else if (foundModels.size() > 1) {
        auto message = QString("More than 1 source %1 model passed").arg(ModelProperties<ModelT>::name);
        throw conversion::translator::TranslationException(std::move(message));
    } else {
        return dynamic_cast<ModelT *>(foundModels[0]);
    }
}

template<typename ModelT>
ModelT *Translator::getModel(const std::vector<std::unique_ptr<Model>> &models)
{
    std::vector<conversion::Model *> rawOutModels;
    for_each(models.begin(), models.end(), [&rawOutModels](auto &model) { rawOutModels.push_back(model.get()); });

    return getModel<ModelT>(rawOutModels);
}

template<typename ModelT>
ModelT *Translator::getOptionalModel(const std::vector<Model *> &models)
{
    std::vector<Model *> foundModels;
    std::copy_if(models.begin(), models.end(), std::back_inserter(foundModels),
            [](Model *model) { return dynamic_cast<ModelT *>(model) != nullptr; });

    if (foundModels.empty()) {
        return nullptr;
    } else if (foundModels.size() > 1) {
        auto message = QString("More than 1 source %1 model passed").arg(ModelProperties<ModelT>::name);
        throw conversion::translator::TranslationException(std::move(message));
    } else {
        return dynamic_cast<ModelT *>(foundModels[0]);
    }
}

template<typename ModelT>
ModelT *Translator::getOptionalModel(const std::vector<std::unique_ptr<Model>> &models)
{
    std::vector<conversion::Model *> rawOutModels;
    for_each(models.begin(), models.end(), [&rawOutModels](auto &model) { rawOutModels.push_back(model.get()); });

    return getOptionalModel<ModelT>(rawOutModels);
}

} // namespace conversion::translator
