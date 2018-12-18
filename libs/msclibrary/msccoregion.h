#ifndef MSCCOREGION_H
#define MSCCOREGION_H

#include "mscinstanceevent.h"

namespace msc {

class MscCoregion : public MscInstanceEvent
{
    Q_OBJECT

public:
    enum class Type { Begin,
                      End };

    explicit MscCoregion(QObject *parent = nullptr);
    explicit MscCoregion(Type type, QObject *parent = nullptr);

    MscEntity::EntityType entityType() const { return MscEntity::EntityType::Timer; }

    void setType(Type type) { m_type = type; }
    Type type() const { return m_type; }

private:
    Type m_type;
};

}

#endif // MSCCOREGION_H
