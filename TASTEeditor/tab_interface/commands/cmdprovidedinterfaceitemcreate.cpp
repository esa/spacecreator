#include "cmdprovidedinterfaceitemcreate.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdProvidedInterfaceItemCreate::CmdProvidedInterfaceItemCreate(AADLObjectsModel *model, const QRectF &geometry)
    : m_model(model)
    , m_geometry(geometry)
{
}

void CmdProvidedInterfaceItemCreate::redo() {}

void CmdProvidedInterfaceItemCreate::undo() {}

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
