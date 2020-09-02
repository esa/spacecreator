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

#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>

namespace aadl {
class AADLObjectConnection;
}

namespace aadlinterface {

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

    struct ConnectionWithEnvironment {
        inline bool operator==(const ConnectionWithEnvironment &other) const
        {
            return instance == other.instance && interface == other.interface && toInstance == other.toInstance;
        }
        inline bool operator!=(const ConnectionWithEnvironment &other) const { return !(*this == other); }

        QString instance; // Function name
        QString interface; // RI or PI
        bool toInstance; // True if from env to instance, false if from instance to env
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
        bool isEmpty() const
        {
            return connections.isEmpty() && envConnections.isEmpty() && internalConnections.isEmpty();
        }
        inline bool operator==(const Dataflow &d) const
        {
            return connections == d.connections && envConnections == d.envConnections
                    && internalConnections == d.internalConnections;
        }
        inline bool operator!=(const Dataflow &d) const { return !(*this == d); }

        QVector<Connection> connections;
        QVector<ConnectionWithEnvironment> envConnections;
        QVector<ConnectionInsideFunction> internalConnections;
    };

    EndToEndConnections(QObject *parent = nullptr);
    ~EndToEndConnections();

    bool isDirty() const;

    QString path() const;

    Dataflow dataflow() const;

    static Dataflow readDataflow(const QString &file, bool isFile);

    static bool isInDataflow(const Dataflow &dataflow, aadl::AADLObjectConnection *connection);

Q_SIGNALS:
    void pathChanged(const QString &path);
    void dirtyChanged(bool dirty);

public Q_SLOTS:
    void setPath(const QString &path);

private:
    struct EndToEndConnectionsPrivate;
    EndToEndConnectionsPrivate *d;
};

}
