#include "mscinstanceevent.h"

namespace msc {

MscInstanceEvent::MscInstanceEvent(QObject *parent)
    : MscEntity(parent)
{
}

MscInstanceEvent::MscInstanceEvent(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

MscInstanceEvent::~MscInstanceEvent()
{
}

}
