#include "msctimer.h"

namespace msc {

MscTimer::MscTimer(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscTimer::MscTimer(const QString &name, TimerType type, QObject *parent)
    : MscInstanceEvent(name, parent), m_timerType(type)
{
}

}
