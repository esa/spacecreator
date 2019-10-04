#include "cmdprovidedinterfaceitemcreate.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

static int sCounter = 0;

CmdProvidedInterfaceItemCreate::CmdProvidedInterfaceItemCreate(AADLObjectsModel *model, AADLObjectContainer *container,
                                                               const QPointF &pos)
    : m_model(model)
    , m_pos(pos)
    , m_entity(new AADLObjectIfaceProvided(QObject::tr("PI_%1").arg(++sCounter), container))
    , m_parent(container)
{
}

void CmdProvidedInterfaceItemCreate::redo()
{
    m_entity->setCoordinates({ qRound(m_pos.x()), qRound(m_pos.y()) });
    if (m_parent)
        m_parent->addInterface(m_entity);
    if (m_model)
        m_model->addObject(m_entity);
}

void CmdProvidedInterfaceItemCreate::undo()
{
    if (m_parent)
        m_parent->removeInterface(m_entity);
    if (m_model)
        m_model->removeObject(m_entity);
}

bool CmdProvidedInterfaceItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdProvidedInterfaceItemCreate::id() const
{
    return CreateProvidedInterfaceEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
