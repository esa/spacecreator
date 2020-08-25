#include "endtoendconnections.h"

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

    QVector<QPair<QString, QString>> dataflow;
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
    return instance->name();
}

//! Get the end to end dataflow. If the dirty flag is set, this will read from the file
//! It will return something like this:
//! QVector(QPair("Start_Transaction","User_Interface"), QPair("Open_File","File_Manager"), QPair("Access_Database","Database_Manager"))
QVector<QPair<QString, QString>> EndToEndConnections::dataflow() const
{
    if (d->dirty) {
        // Read the stuff
        d->dataflow = readDataflow(d->path, true);
        d->dirty = false;
    }

    return d->dataflow;
}

static QVector<QPair<QString, QString>> readDataFlowFromDocument(msc::MscDocument *document)
{
    for (auto chart : document->charts()) {
        QVector<QPair<QString, QString>> dataflow;
        QString lastInstance;
        for (auto event : chart->instanceEvents()) {
            auto msg = dynamic_cast<msc::MscMessage *>(event);
            if (msg != nullptr && msg->messageType() == msc::MscMessage::MessageType::Message) {
                const QString source = instanceName(msg->sourceInstance());
                const QString target = instanceName(msg->targetInstance());
                const QString message = msg->name();

                // The source must match, except for the first call which can be from global or any instance
                if (source == lastInstance || dataflow.isEmpty()) {
                    dataflow.append({ message, target });
                    lastInstance = target;
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
QVector<QPair<QString, QString>> EndToEndConnections::readDataflow(const QString &file, bool isFile)
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
