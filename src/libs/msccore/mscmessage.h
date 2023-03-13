/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscinstanceevent.h"
#include "mscparameterlist.h"

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscInstance;

class MscMessage : public MscInstanceEvent
{
    Q_OBJECT
    Q_PROPERTY(QString messageInstanceName READ messageInstanceName WRITE setMessageInstanceName NOTIFY
                    messageInstanceNameChanged)
    Q_PROPERTY(msc::MscInstance *sourceInstance READ sourceInstance WRITE setSourceInstance NOTIFY sourceChanged)
    Q_PROPERTY(msc::MscInstance *targetInstance READ targetInstance WRITE setTargetInstance NOTIFY targetChanged)
    Q_PROPERTY(QString parameterString READ paramString NOTIFY parameterStringChanged)

public:
    enum class MessageType
    {
        Message = 0,
        Create

        // TODO: add call/reply/method/etc?
    };
    Q_ENUM(MessageType)

    enum class EndType
    {
        SOURCE_TAIL,
        TARGET_HEAD
    };
    Q_ENUM(EndType)

    explicit MscMessage(QObject *parent = nullptr);
    explicit MscMessage(const QString &name, QObject *parent = nullptr);
    MscMessage(const QString &name, MscInstance *source, MscInstance *target, QObject *parent = nullptr);

    const QString &messageInstanceName() const;
    void setMessageInstanceName(const QString &name);

    QString fullName() const;

    MscInstance *sourceInstance() const;
    void setSourceInstance(MscInstance *source);
    QString sourceName() const;

    MscInstance *targetInstance() const;
    void setTargetInstance(MscInstance *target);
    QString targetName() const;

    MscEntity::EntityType entityType() const override;

    const MscParameterList &parameters() const;
    void setParameters(const MscParameterList &parameters);

    bool isSame(const msc::MscMessage *message) const;

    MscMessage::MessageType messageType() const;

    bool relatesTo(const MscInstance *instance) const override;

    // Begin of parsing helper block
    struct Flow {
        MscInstance *from = nullptr;
        MscInstance *to = nullptr;

        bool isComplete() const { return from && to; }
        bool operator==(const Flow &other) const { return from == other.from && to == other.to; }

        struct SourceLineInfo {
            size_t m_line = 0;
            size_t m_pos = 0;
        } m_sourceLineInfo;
    };
    Flow m_descrIn, m_descrOut;
    bool isGlobal() const;
    bool isOrphan() const;
    bool isConnected() const;
    // End of parsing helper block

    QString toDbgString() const override;

    cif::CifBlockShared cifMessage() const;
    QVector<QPoint> cifPoints() const;
    void setCifPoints(const QVector<QPoint> &points);

    QString paramString() const;

Q_SIGNALS:
    void messageInstanceNameChanged();
    void sourceChanged(msc::MscInstance *source);
    void targetChanged(msc::MscInstance *target);
    void parameterStringChanged();
    void cifPointsChanged();

protected:
    MscMessage::MessageType m_msgType = MscMessage::MessageType::Message;

private:
    static const cif::CifLine::CifType m_messageCifMainType;
    QString m_messageInstanceName;
    MscInstance *m_source = nullptr;
    MscInstance *m_target = nullptr;
    MscParameterList m_parameters;
};
} // msc
