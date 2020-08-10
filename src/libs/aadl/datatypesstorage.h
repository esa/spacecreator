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

#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QString>
#include <memory>

namespace Asn1Acn {
class File;
}

namespace aadl {

class DataTypesStorage : public QObject
{
    Q_OBJECT
public:
    DataTypesStorage(std::unique_ptr<Asn1Acn::File> &dataTypes);
    ~DataTypesStorage();

    static void init();
    static DataTypesStorage *instance();

    const std::unique_ptr<Asn1Acn::File> &asn1DataTypes() const;

    Q_SLOT void setFileName(const QFileInfo &fileName);
    const QFileInfo &fileName() const;

    Q_SLOT bool loadFile();

    void clear();

Q_SIGNALS:
    void dataTypesChanged();

private:
    static DataTypesStorage *m_instance;

    std::unique_ptr<Asn1Acn::File> m_asn1DataTypes;
    QFileInfo m_fileName;
};

}
