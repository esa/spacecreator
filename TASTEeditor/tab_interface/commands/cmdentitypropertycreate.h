#pragma once

#include "tab_aadl/aadlobject.h"

#include <QUndoCommand>
#include <QPointer>

namespace taste3 {
namespace aadl {

class AADLObject;

namespace cmd {

class CmdEntityPropertyCreate : public QUndoCommand
{
public:
    explicit CmdEntityPropertyCreate(AADLObject *entity, const QVariantHash &props);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<AADLObject> m_entity;
    QVariantHash m_newProps;
    const QVariantHash m_oldProps;
};

} // namespace cmd
} // namespace aadl
} // namespace taste3
