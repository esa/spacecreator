#pragma once

#include <QObject>

class QTimer;

namespace shared {

/*!
 * This class is used to have a bunch of methods emit the same signal,
 * but where there is only actually one signal fired at the end.
 *
 * The actual signal will be fired at the next event loop.
 */
class DelayedSignal : public QObject
{
    Q_OBJECT

public:
    explicit DelayedSignal(QObject *parent = nullptr);

Q_SIGNALS:
    void triggered(QObject *parent);

public Q_SLOTS:
    void triggerSignal();

    //! Update the delay to /a msec. Default is 0.
    void setInterval(int msec);

    void stop();

private Q_SLOTS:
    void triggerNow();

private:
    QTimer *m_timer;
};

}
