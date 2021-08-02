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

#include <seds/SedsCommon/basetypesmappings.h>

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class CommandRefAttributes;
class NamedArgumentValue;
class OnCommandPrimitive;
class OnParameterPrimitive;
class ParameterRefAttributes;
class PrimitiveSink;
class PrimitiveSource;
class SendCommandPrimitive;
class SendParameterPrimitive;
class SinkArgumentValue;
class TimerSink;

enum class ParameterOperation;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for component primitives
 */
class ComponentPrimitivesParser final
{
  public:
    /**
     * @brief   Constructs NamedArgumentValue object
     * @return  NamedArgumentValue
     */
    static auto readNamedArgumentValue(QXmlStreamReader& xmlReader) -> model::NamedArgumentValue;

    /**
     * @brief   Constructs SendCommandPrimitive object
     * @return  SendCommandPrimitive
     */
    static auto readSendCommandPrimitive(QXmlStreamReader& xmlReader) -> model::SendCommandPrimitive;
    /**
     * @brief   Constructs SendParameterPrimitive object
     * @return  SendParameterPrimitive
     */
    static auto readSendParameterPrimitive(QXmlStreamReader& xmlReader) -> model::SendParameterPrimitive;

    /**
     * @brief   Constructs OnCommandPrimitive object
     * @return  OnCommandPrimitive
     */
    static auto readOnCommandPrimitive(QXmlStreamReader& xmlReader) -> model::OnCommandPrimitive;
    /**
     * @brief   Constructs OnParameterPrimitive object
     * @return  OnParameterPrimitive
     */
    static auto readOnParameterPrimitive(QXmlStreamReader& xmlReader) -> model::OnParameterPrimitive;
    /**
     * @brief   Constructs TimerSink object
     * @return  TimerSink
     */
    static auto readTimerSink(QXmlStreamReader& xmlReader) -> model::TimerSink;

    /**
     * @brief   Processes attribute for ParameterRefAttributes
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForParameterRefAttributes(model::ParameterRefAttributes* object,
                                                 const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for ParameterRefAttributes
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForParameterRefAttributes(model::ParameterRefAttributes* object, QXmlStreamReader& xmlReader)
            -> bool;

  private:
    /**
     * @brief   Constructs SinkArgumentValue object
     * @return  SinkArgumentValue
     */
    static auto readSinkArgumentValue(QXmlStreamReader& xmlReader) -> model::SinkArgumentValue;

    /**
     * @brief   Parses given string as a parameter operation
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseParameterOperation(QStringRef valueStr) -> model::ParameterOperation;

    /**
     * @brief   Processes attribute for CommandRefAttributes
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForCommandRefAttributes(model::CommandRefAttributes* object,
                                               const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for CommandRefAttributes
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForCommandRefAttributes(model::CommandRefAttributes* object, QXmlStreamReader& xmlReader)
            -> bool;
    /**
     * @brief   Processes attribute for PrimitiveSink
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForPrimitiveSink(model::PrimitiveSink* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for PrimitiveSink
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForPrimitiveSink(model::PrimitiveSink* object, QXmlStreamReader& xmlReader) -> bool;
    /**
     * @brief   Processes attribute for PrimitiveSource
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForPrimitiveSource(model::PrimitiveSource* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for PrimitiveSource
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForPrimitiveSource(model::PrimitiveSource* object, QXmlStreamReader& xmlReader) -> bool;
};

} // namespace seds::parser
