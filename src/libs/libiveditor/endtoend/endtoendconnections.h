/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QDebug>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>

namespace ivm {
class AADLConnectionChain;
class AADLConnection;
}

namespace msc {
class MscDocument;
}

namespace ive {

/*!
 * \brief The EndToEndConnections class reads an MSC file and returns the list of messages->instances.
 */
class EndToEndConnections : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool dirty READ isDirty NOTIFY dirtyChanged)

public:
    struct Connection {
        inline bool operator==(const Connection &other) const
        {
            return from == other.from && to == other.to && message == other.message;
        }
        inline bool operator!=(const Connection &other) const { return !(*this == other); }

        QString from;
        QString to;
        QString message;
    };

    struct ConnectionInsideFunction {
        inline bool operator==(const ConnectionInsideFunction &other) const
        {
            return instance == other.instance && interface1 == other.interface1 && interface2 == other.interface2;
        }
        inline bool operator!=(const ConnectionInsideFunction &other) const { return !(*this == other); }

        QString instance;
        QString interface1;
        QString interface2;
    };

    struct Dataflow {
        inline bool isEmpty() const { return connections.isEmpty() && internalConnections.isEmpty(); }
        inline bool operator==(const Dataflow &d) const
        {
            return connections == d.connections && internalConnections == d.internalConnections;
        }
        inline bool operator!=(const Dataflow &d) const { return !(*this == d); }

        QVector<Connection> connections;
        QVector<ConnectionInsideFunction> internalConnections;
    };

    EndToEndConnections(QObject *parent = nullptr);
    ~EndToEndConnections();

    bool isDirty() const;

    QString path() const;

    Dataflow dataflow() const;
    Dataflow dataflow(msc::MscDocument *document) const;

    static Dataflow readDataflow(const QString &file, bool isFile);
    static Dataflow readDataFlowFromDocument(msc::MscDocument *document);

    static bool isInDataflow(const Dataflow &dataflow, const QList<ivm::AADLConnectionChain *> &chains,
            ivm::AADLConnection *connection);

Q_SIGNALS:
    void pathChanged(const QString &path);
    void dirtyChanged(bool dirty);

public Q_SLOTS:
    void setPath(const QString &path);

private:
    struct EndToEndConnectionsPrivate;
    EndToEndConnectionsPrivate *d;
};

QDebug operator<<(QDebug debug, const EndToEndConnections::Connection &connection);
QDebug operator<<(QDebug debug, const EndToEndConnections::ConnectionInsideFunction &connection);
QDebug operator<<(QDebug debug, const EndToEndConnections::Dataflow &dataflow);

}
