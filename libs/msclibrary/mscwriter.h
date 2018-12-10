/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef MSCWRITER_H
#define MSCWRITER_H

#include <QObject>

namespace msc {

class MscModel;
class MscInstance;
class MscMessage;
class MscChart;
class MscDocument;

class MscWriter : public QObject
{
    Q_OBJECT

public:
    MscWriter(QObject *parent = nullptr);

    void saveModel(const MscModel *model, const QString &fileName);

private:
    QString serialize(const MscInstance *instance);
    QString serialize(const MscMessage *message, const MscInstance *instance);
    QString serialize(const MscChart *chart);
    QString serialize(const MscDocument *document);
};

} // namespace msc

#endif // MSCWRITER_H
