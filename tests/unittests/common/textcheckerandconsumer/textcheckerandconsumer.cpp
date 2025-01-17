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

#include "textcheckerandconsumer.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QVector>
#include <vector>

namespace tests::common {

void TextCheckerAndConsumer::checkSequenceAndConsume(
        const QStringList &expectedOutput, QTextStream &actualConsumableOutput)
{
    for (int i = 0; i < expectedOutput.size(); i++) {
        const auto &expectedLine = expectedOutput.at(i);
        if (doesStreamContainRequested(actualConsumableOutput, expectedLine)) {
            continue;
        } else {
            const QString message =
                    QString("the generated file does not contain '%1' substring (line %2)").arg(expectedLine).arg(i);
            throw std::logic_error(message.toStdString().c_str());
        }
    }
}

const QStringList TextCheckerAndConsumer::readLinesFromFile(const QString &filename)
{
    QFile expectedOutFile(filename);
    if (!expectedOutFile.open(QIODevice::ReadOnly)) {
        const QString message = (QString("requested file (%1) cannot be found").arg(filename).toStdString().c_str());
        throw std::logic_error(message.toStdString().c_str());
    }

    const QByteArray expectedData = expectedOutFile.readAll();
    const QString expectedText = QString::fromStdString(expectedData.toStdString());
    QStringList expectedStringList = expectedText.split("\n");

    for (auto &out : expectedStringList) {
        out = out.trimmed();
    }
    return expectedStringList;
}

bool TextCheckerAndConsumer::doesStreamContainRequested(QTextStream &streamToVerify, const QString &requestedString)
{
    QString line;
    do {
        line = streamToVerify.readLine();
        if (line.contains(requestedString)) {
            return true;
        }
    } while (!streamToVerify.atEnd());

    return false;
}

} // namespace tests::common
