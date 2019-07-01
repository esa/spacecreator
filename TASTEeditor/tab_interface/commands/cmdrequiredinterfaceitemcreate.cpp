#include "cmdrequiredinterfaceitemcreate.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdRequiredInterfaceItemCreate::CmdRequiredInterfaceItemCreate(AADLObjectsModel *model, const QRectF &geometry)
    : m_model(model)
    , m_geometry(geometry)
{
}

void CmdRequiredInterfaceItemCreate::redo() {}

void CmdRequiredInterfaceItemCreate::undo() {}

bool CmdRequiredInterfaceItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdRequiredInterfaceItemCreate::id() const
{
    return CreateRequiredInterfaceEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
