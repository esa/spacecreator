#include "mscinstanceevent.h"

#include "mscchart.h"

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

/**
 * Returns the chart this event belongs to
 */
MscChart *MscInstanceEvent::chart() const
{
    return qobject_cast<MscChart *>(parent());
}

}
