/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QVariantMap>

namespace ive {

class InterfaceDocument;

class RemoteControlHandler : public QObject
{
    Q_OBJECT
public:
    enum class CommandType
    {
        Load,
        HighlightX,
        HighlightY,
        HighlightZ,
    };
    Q_ENUM(CommandType)

    explicit RemoteControlHandler(QObject *parent = nullptr);

    void setIvDocument(ive::InterfaceDocument *model);

public Q_SLOTS:
    void handleMessage(const QJsonObject &obj, const QString &peerName);
    void handleRemoteCommand(
            ive::RemoteControlHandler::CommandType commandType, const QVariantMap &params, const QString &peerName);

Q_SIGNALS:
    void commandDone(bool result, const QString &peerName, const QString &errorString);

private:
    bool handleLoad(const QVariantMap &params, QString *errorString);

    QPointer<ive::InterfaceDocument> m_document;
    QString m_lastErrorMessage;
};

}
