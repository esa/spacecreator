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
