#include "msccoregion.h"

namespace msc {

MscCoregion::MscCoregion(QObject *parent)
    : MscInstanceEvent(parent), m_type(Type::Begin)
{
}

MscCoregion::MscCoregion(Type type, QObject *parent)
    : MscInstanceEvent(parent), m_type(type)
{
}

}
