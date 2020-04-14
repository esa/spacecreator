#pragma once

#include <QObject>

class QTimer;

namespace taste3 {
namespace util {

/*!
 * This class is used to have a bunch of methods emit the same signal,
 * but where there is only actually one signal fired at the end.
 *
 * The actual signal will be fired at the next event loop.
 */
class DelayedSignal : public QObject {
    Q_OBJECT

public:
    explicit DelayedSignal(QObject* parent = nullptr);

signals:
    void triggered(QObject* parent);

public slots:
    void triggerSignal();

private slots:
    void triggerNow();

private:
    QTimer* mTimer;
};


}
}
