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

class MscChart;
class MscCondition;
class MscDocument;
class MscInstance;
class MscMessage;
class MscModel;

class MscWriter : public QObject
{
    Q_OBJECT

public:
    MscWriter(QObject *parent = nullptr);

    void saveModel(const MscModel *model, const QString &fileName);
    void saveChart(const MscChart *chart, const QString &fileName);

protected:
    QString serialize(const MscInstance *instance,
                      const QVector<MscMessage *> &messages,
                      const QVector<MscCondition *> &conditions,
                      int tabsSize = 0);
    QString serialize(const MscMessage *message, const MscInstance *instance, int tabsSize = 0);
    QString serialize(const MscCondition *condition, int tabsSize = 0);
    QString serialize(const MscChart *chart, int tabsSize = 0);
    QString serialize(const MscDocument *document, int tabsSize = 0);

private:
    QString tabs(int tabsSize);
};

} // namespace msc

#endif // MSCWRITER_H
