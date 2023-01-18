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

#include "remotecontrolwebserver.h"

#include <QObject>
#include <QPointer>
#include <QVariantMap>

class QUndoStack;

namespace msc {

class MscChart;
class MscModel;
class ChartLayoutManager;

class RemoteControlHandler : public QObject
{
    Q_OBJECT
public:
    explicit RemoteControlHandler(QObject *parent = nullptr);

    void setMscModel(MscModel *model);
    void setUndoStack(QUndoStack *undoStack);
    void setLayoutManager(ChartLayoutManager *layoutManager);
    void setChart(msc::MscChart *mscChart);

public Q_SLOTS:
    void handleRemoteCommand(
            RemoteControlWebServer::CommandType commandType, const QVariantMap &params, const QString &peerName);

Q_SIGNALS:
    void commandDone(RemoteControlWebServer::CommandType commandType, bool result, const QString &peerName,
            const QString &errorString);

private:
    bool handleInstanceCommand(const QVariantMap &params, QString *errorString);
    bool handleInstanceStopCommand(const QVariantMap &params, QString *errorString);
    bool handleMessageCommand(const QVariantMap &params, QString *errorString);
    bool handleTimerCommand(const QVariantMap &params, QString *errorString);
    bool handleActionCommand(const QVariantMap &params, QString *errorString);
    bool handleConditionCommand(const QVariantMap &params, QString *errorString);
    bool handleMessageDeclarationCommand(const QVariantMap &params, QString *errorString);

    bool saveMsc(const QString &fileName, const QString &asn1FileName);

    QPointer<MscModel> m_mscModel;
    QPointer<ChartLayoutManager> m_layoutManager;
    QPointer<QUndoStack> m_undoStack;
    QPointer<msc::MscChart> m_mscChart;
};

}
