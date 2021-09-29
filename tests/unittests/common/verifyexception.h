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

#define VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(expression, exceptionType, exceptionMessage) \
    do {\
        try {\
            try {\
                expression;\
                auto message = QByteArray() + "Expected exception of type " #exceptionType \
                               " to be thrown but no exception caught";\
                QTest::qFail(message.constData(), __FILE__, __LINE__);\
                return;\
            } catch (const exceptionType &ex) {\
                if (QString(ex.what()) != exceptionMessage) {\
                    auto message = QByteArray() + "Expected exception message \"" + exceptionMessage +\
                                "\" but got \"" + ex.what() + "\"";\
                    QTest::qFail(message.constData(), __FILE__, __LINE__);\
                    return;\
                }\
            }\
        } catch (const std::exception &ex) {\
            QByteArray message = QByteArray() + "Expected exception of type " #exceptionType \
                                 " to be thrown but std::exception caught with message: " + ex.what();\
            QTest::qFail(message.constData(), __FILE__, __LINE__);\
            return;\
        } catch (...) {\
            QByteArray message = QByteArray() + "Expected exception of type " #exceptionType \
                                 " to be thrown but unknown exception caught";\
            QTest::qFail(message.constData(), __FILE__, __LINE__);\
            return;\
        }\
    } while(false)
