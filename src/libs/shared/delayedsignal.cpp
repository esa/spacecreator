#include "delayedsignal.h"

#include <QTimer>

namespace shared {

DelayedSignal::DelayedSignal(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), SLOT(triggerNow()));
}

void DelayedSignal::triggerSignal()
{
    m_timer->start();
}

void DelayedSignal::setInterval(int msec)
{
    m_timer->setInterval(msec);
}

void DelayedSignal::triggerNow()
{
    Q_EMIT triggered(parent());
}

}
