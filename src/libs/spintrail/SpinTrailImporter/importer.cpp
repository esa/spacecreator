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

#include "importer.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <spintrail/SpinTrailModel/spintrailmodel.h>
#include <spintrail/SpinTrailOptions/options.h>

using namespace conversion::spintrail;

using conversion::ConversionException;
using conversion::FileNotFoundException;
using spintrail::model::SpinTrailModel;

namespace spintrail::importer {
std::unique_ptr<conversion::Model> SpinTrailImporter::importModel(const conversion::Options &options) const
{
    std::unique_ptr<SpinTrailModel> model = std::make_unique<SpinTrailModel>();

    std::optional<QString> inputFilepath = options.value(SpinTrailOptions::inputFilepath);

    if (inputFilepath) {
        const QFileInfo inputFileinfo(inputFilepath.value());

        if (!inputFileinfo.exists()) {
            throw FileNotFoundException(inputFilepath.value(), "while importing");
        }

        QFile inputFile(inputFileinfo.absoluteFilePath());
        if (!inputFile.open(QFileDevice::ReadOnly | QFileDevice::Text)) {
            throw ConversionException(QString("Cannot open file: %1").arg(inputFileinfo.absoluteFilePath()));
        }

        QTextStream input(&inputFile);

        while (!input.atEnd()) {
            QString line = input.readLine();
            processLine(*model, line);
        }
    }

    return model;
}

void SpinTrailImporter::processLine(spintrail::model::SpinTrailModel &model, const QString &line) const
{
    qDebug() << "SpinTrailImporter: parsing line : " << line;
    Q_UNUSED(model);
}
}
