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
        QString from;
        QString to;
        QString message;
    };

    EndToEndConnections(QObject *parent = nullptr);
    ~EndToEndConnections();

    bool isDirty() const;

    QString path() const;

    QVector<Connection> dataflow() const;

    static QVector<Connection> readDataflow(const QString &file, bool isFile);

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
