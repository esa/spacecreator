/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "xmlreader.h"

#include <QObject>
#include <QVector>
#include <memory>

class QXmlStreamReader;

namespace dvm {
class DVObject;
class DVProcessor;
struct DVXMLReaderPrivate;

class DVXMLReader : public shared::XmlReader
{
    Q_OBJECT
public:
    explicit DVXMLReader(QObject *parent = nullptr);
    ~DVXMLReader() override;

    QVector<DVObject *> parsedObjects() const;

protected:
    void processTagOpen(QXmlStreamReader &xml) override;
    void processTagClose(QXmlStreamReader &xml) override;
    QString rootElementName() const override;

private:
    const std::unique_ptr<DVXMLReaderPrivate> d;

    DVProcessor *createProcessor(const QString &name);
};

} // namespace dvm
