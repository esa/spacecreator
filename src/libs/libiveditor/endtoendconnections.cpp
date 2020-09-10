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

#include "endtoendconnections.h"

#include "aadlnamevalidator.h"
#include "aadlobjectconnection.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscreader.h"

#include <QFileSystemWatcher>
#include <memory>

namespace aadlinterface {

struct EndToEndConnections::EndToEndConnectionsPrivate {
    void setDirty(bool dirty, EndToEndConnections *q)
    {
        if (this->dirty != dirty) {
            this->dirty = dirty;
            Q_EMIT q->dirtyChanged(dirty);
        }
    }

    QString path;
    QFileSystemWatcher fileWatcher;

    bool dirty { false };

    Dataflow dataflow;
};

EndToEndConnections::EndToEndConnections(QObject *parent)
    : QObject(parent)
    , d(new EndToEndConnectionsPrivate)
{
    connect(&d->fileWatcher, &QFileSystemWatcher::fileChanged, [this]() { d->setDirty(true, this); });
}

EndToEndConnections::~EndToEndConnections()
{
    delete d;
    d = nullptr;
}

//! Returns true if reading the list will result in parsing the file again
bool EndToEndConnections::isDirty() const
{
    return d->dirty;
}

//! Get the path of the MSC file
QString EndToEndConnections::path() const
{
    return d->path;
}

static QString instanceName(msc::MscInstance *instance)
{
    if (instance == nullptr) {
        return {};
    }
    return aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Function, instance->name());
}

//! Get the end to end dataflow. If the dirty flag is set, this will read from the file
//! It will return something like this in the connections list:
//! QVector((""; "Start_Transaction","User_Interface"), QPair("Open_File","File_Manager"), QPair("Access_Database","Database_Manager"))
EndToEndConnections::Dataflow EndToEndConnections::dataflow() const
{
    if (d->dirty) {
        // Read the stuff
        d->dataflow = readDataflow(d->path, true);
        d->dirty = false;
    }

    return d->dataflow;
}

static EndToEndConnections::Dataflow readDataFlowFromDocument(msc::MscDocument *document)
{
    for (auto chart : document->charts()) {
        EndToEndConnections::Dataflow dataflow;
        QString lastInstance;
        QString lastRI;
        for (auto event : chart->instanceEvents()) {
            auto msg = dynamic_cast<msc::MscMessage *>(event);
            if (msg != nullptr && msg->messageType() == msc::MscMessage::MessageType::Message) {
                const QString source = instanceName(msg->sourceInstance());
                const QString target = instanceName(msg->targetInstance());
                const QString message =
                        aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::RequiredInterface, msg->name());

                // The source must match, except for the first call which can be from global or any instance
                if (!message.isEmpty() && !source.isEmpty() && !target.isEmpty()
                        && (source == lastInstance || dataflow.isEmpty())) {
                    dataflow.connections.append({ source, target, message });

                    if (source != target && !message.isEmpty()) {
                        // A message came in to this instance and now a new message is leaving it. Show this in the
                        // dataflow
                        dataflow.internalConnections.append({ source, lastRI, message });
                    }

                    lastInstance = target;
                    lastRI = message;
                }
            }
        }

        if (!dataflow.isEmpty()) {
            // If there is a flow in this chart, assume it's the right one
            return dataflow;
        }
    }
    return {};
}

//! Read the dataflow from a file or string. If file is a filename, isFile should be true.
//! If file is the contents of the file, isFile should be false
EndToEndConnections::Dataflow EndToEndConnections::readDataflow(const QString &file, bool isFile)
{
    msc::MscReader reader;
    std::unique_ptr<msc::MscModel> model(isFile ? reader.parseFile(file) : reader.parseText(file));

    // We assume to work on the first leaf with a chart that has messages (at least for now)
    msc::MscDocument *document = nullptr;
    if (model) {
        QList<msc::MscDocument *> documents = model->documents().toList();
        while (document == nullptr && !documents.isEmpty()) {
            auto doc = documents.takeFirst();
            if (doc != nullptr && doc->hierarchyType() == msc::MscDocument::HierarchyLeaf && !doc->charts().isEmpty()) {
                // Read this document
                auto dataflow = readDataFlowFromDocument(doc);
                if (!dataflow.isEmpty()) {
                    // We found a flow! Use it
                    return dataflow;
                }
            } else {
                // We should also check the children
                documents.append(doc->documents().toList());
            }
        }
    }

    // We did not find anything
    return {};
}

bool EndToEndConnections::isInDataflow(const Dataflow &dataflow, aadl::AADLObjectConnection *connection)
{
    // Just to be on the save side
    if (connection == nullptr) {
        return false;
    }

    Connection c = { connection->sourceName(), connection->targetName(), connection->targetInterfaceName() };
    return dataflow.connections.contains(c);
}

//! Set the path of the MSC file
void EndToEndConnections::setPath(const QString &path)
{
    if (path != d->path) {
        if (!d->fileWatcher.files().isEmpty()) {
            // Remove the old path
            d->fileWatcher.removePath(d->path);
        }

        if (!path.isEmpty()) {
            // Don't add an empty path
            d->fileWatcher.addPath(path);
        }

        d->path = path;
        Q_EMIT pathChanged(path);

        // Changing the path means dirty as well
        d->setDirty(true, this);
    }
}

}
