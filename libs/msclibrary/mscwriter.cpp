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

#include <QFile>
#include <QTextStream>

#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscwriter.h"

namespace msc {

MscWriter::MscWriter(QObject *parent)
    : QObject(parent)
{
}

void MscWriter::saveModel(const MscModel *model, const QString &fileName)
{
    QString mscText;

    if (model == nullptr || fileName.isEmpty())
        return;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mscFile);

    for (const auto *doc : model->documents())
        out << serialize(doc);

    for (const auto *chart : model->charts())
        out << serialize(chart);

    mscFile.close();
}

QString MscWriter::serialize(const MscInstance *instance)
{
    if (instance == nullptr)
        return "";

    QString events;
    QString header = QString("instance %1").arg(instance->name());

    if (!instance->kind().isEmpty()) {
        header += QString(": %1 %2;\n").arg(instance->kind(), instance->inheritance());
    } else
        header += ";\n";

    for (const auto &event : instance->events())
        events += serialize(event, instance);

    return header + events + "endinstance;\n";
}

QString MscWriter::serialize(const MscMessage *message, const MscInstance *instance)
{
    if (message == nullptr)
        return "";

    QString direction;
    QString instanceName;

    if (message->sourceInstance() == instance) {
        direction = "out %1 to %2;\n";
        instanceName = message->targetInstance() != nullptr ? message->targetInstance()->name() : "env";
    } else {
        direction = "in %1 from %2;\n";
        instanceName = message->sourceInstance() != nullptr ? message->sourceInstance()->name() : "env";
    }

    return QString(direction).arg(message->name(), instanceName);
}

QString MscWriter::serialize(const MscChart *chart)
{
    if (chart == nullptr)
        return "";

    QString instances;

    for (const auto *instance : chart->instances())
        instances += serialize(instance);

    return QString("msc %1;\n %2 endmsc;\n").arg(chart->name(), instances);
}

QString MscWriter::serialize(const MscDocument *document)
{
    if (document == nullptr)
        return "";

    QString instances;

    for (const auto *doc : document->documents())
        instances += serialize(doc);

    for (const auto *chart : document->charts())
        instances += serialize(chart);

    return QString("mscdocument %1;\n %2 endmscdocument;\n").arg(document->name(), instances);
}

} // namespace msc
