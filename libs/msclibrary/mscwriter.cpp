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

    int tabCount = 0;
    for (const auto *doc : model->documents())
        out << serialize(doc, tabCount++);

    for (const auto *chart : model->charts())
        out << serialize(chart);

    mscFile.close();
}

void MscWriter::saveChart(const MscChart *chart, const QString &fileName)
{
    QString mscText;

    if (chart == nullptr || fileName.isEmpty())
        return;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mscFile);

    out << serialize(chart);

    mscFile.close();
}

QString MscWriter::serialize(const MscInstance *instance, const QVector<MscMessage *> &messages, int tabsSize)
{
    if (instance == nullptr)
        return "";

    QString events;

    QString tabString = tabs(tabsSize);
    QString header = QString("%1instance %2").arg(tabString, instance->name());
    QString footer = tabString + "endinstance;\n";

    if (!instance->kind().isEmpty()) {
        header += QString(": %1 %2;\n").arg(instance->kind(), instance->inheritance());
    } else
        header += ";\n";

    for (const auto &message : messages) {
        if (message->sourceInstance() == instance || message->targetInstance() == instance)
            events += serialize(message, instance, tabsSize + 1);
    }

    return header + events + footer;
}

QString MscWriter::serialize(const MscMessage *message, const MscInstance *instance, int tabsSize)
{
    if (message == nullptr)
        return "";

    QString direction = tabs(tabsSize);
    QString name = message->name();
    QString instanceName;

    if (message->sourceInstance() == instance) {
        direction += "out %1 to %2;\n";
        instanceName = message->targetInstance() != nullptr ? message->targetInstance()->name() : "env";
    } else {
        direction += "in %1 from %2;\n";
        instanceName = message->sourceInstance() != nullptr ? message->sourceInstance()->name() : "env";
    }

    if (!message->parameters().name.isEmpty())
        name += QString(",%1").arg(message->parameters().name);

    if (!message->parameters().expression.isEmpty() || !message->parameters().patern.isEmpty())
        name += QString("(%1)").arg(!message->parameters().expression.isEmpty() ? message->parameters().expression : message->parameters().patern);

    return QString(direction).arg(name, instanceName);
}

QString MscWriter::serialize(const MscChart *chart, int tabsSize)
{
    if (chart == nullptr)
        return "";

    QString instances;

    for (const auto *instance : chart->instances())
        instances += serialize(instance, chart->messages(), tabsSize + 1);

    QString tabString = tabs(tabsSize);
    return QString("%1msc %2;\n%3%1endmsc;\n").arg(tabString, chart->name(), instances);
}

QString MscWriter::serialize(const MscDocument *document, int tabsSize)
{
    if (document == nullptr)
        return "";

    QString instances;

    int tabCount = tabsSize + 1;
    for (const auto *doc : document->documents())
        instances += serialize(doc, tabCount++);

    for (const auto *chart : document->charts())
        instances += serialize(chart, tabsSize + 1);

    QString tabString = tabs(tabsSize);
    return QString("%1mscdocument %2;\n%3%1endmscdocument;\n").arg(tabString, document->name(), instances);
}

QString MscWriter::tabs(int tabsSize)
{
    const static QString TABS = "   ";

    QString tabsString;

    for (int x = 0; x < tabsSize; ++x)
        tabsString += TABS;

    return tabsString;
}

} // namespace msc
