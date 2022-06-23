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

#include <conversion/common/import/modelimporter.h>

#include <memory>

class QXmlStreamReader;

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace simulink::model {
class ModelInterface;
class Workspace;
class WorkspaceElement;
class MemberSet;
class Member;
class Inports;
class Inport;
class Outports;
class Outport;
} // namespace simulink::model

namespace simulink::importer {

/**
 * @brief   Importer that reads SIMULINK files
 */
class SimulinkXmlImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Read given SIMULINK file and produce SIMULINK model
     *
     * @param   options     List of options
     *
     * @throws  conversion::importer::ImportException
     * @throws  msc::FileNotFoundException
     * 
     * @return  Imported SIMULINK model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;

private:
    /**
     * @brief   Validate given preprocessed SEDS file
     *
     * Uses XmlValidator
     *
     * @param   preprocessedFilename    File to validate
     * @param   options                 Importer options
     *
     * @throws  conversion::importer::FileNotFound
     * @throws  seds::validator::ValidatorException
     */
    //auto validate(const QString &preprocessedFilepath, const conversion::Options &options) const -> void;

    static auto parse(const QString &inputSimulinkFilename) -> std::unique_ptr<conversion::Model>;
    static auto readModelInterface(QXmlStreamReader &xmlReader) -> model::ModelInterface;
    static auto readWorkspace(QXmlStreamReader &xmlReader) -> model::Workspace;
    static auto readWorkspaceElement(QXmlStreamReader &xmlReader) -> model::WorkspaceElement;
    static auto readWorkspaceElementMembers(QXmlStreamReader &xmlReader) -> model::MemberSet;
    static auto readMember(QXmlStreamReader &xmlReader) -> model::Member;
    static auto readInports(QXmlStreamReader &xmlReader) -> model::Inports;
    static auto readInport(QXmlStreamReader &xmlReader) -> model::Inport;
    static auto readOutports(QXmlStreamReader &xmlReader) -> model::Outports;
    static auto readOutport(QXmlStreamReader &xmlReader) -> model::Outport;
};

} // namespace seds::importer
