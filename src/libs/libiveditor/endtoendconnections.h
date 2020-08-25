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
    EndToEndConnections(QObject *parent = nullptr);
    ~EndToEndConnections();

    bool isDirty() const;

    QString path() const;

    QVector<QPair<QString, QString>> dataflow() const;

    static QVector<QPair<QString, QString>> readDataflow(const QString &file, bool isFile);

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
