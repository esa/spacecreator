#include "mscinstanceevent.h"

namespace msc {

/*!
    \class msc::MscInstanceEvent
    This is the base class for conditions, messages and timers. In the grammar this is called
    InstanceEvent
 */

MscInstanceEvent::MscInstanceEvent(QObject *parent)
    : MscInstanceEvent(DefaultName, parent)
{
}

MscInstanceEvent::MscInstanceEvent(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

MscInstanceEvent::~MscInstanceEvent() { }

}
