#include "msctimer.h"

namespace msc {

MscTimer::MscTimer(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscTimer::MscTimer(const QString &name, QObject *parent)
    : MscInstanceEvent(name, parent)
{
}

}
