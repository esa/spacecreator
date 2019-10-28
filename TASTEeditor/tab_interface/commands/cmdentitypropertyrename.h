#pragma once

#include "tab_aadl/aadlobject.h"

#include <QUndoCommand>
#include <QPointer>

namespace taste3 {
namespace aadl {

class AADLObject;

namespace cmd {

class CmdEntityPropertyRename : public QUndoCommand
{
public:
    explicit CmdEntityPropertyRename(AADLObject *entity, const QHash<QString,QString> &props);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<AADLObject> m_entity;
    QHash<QString,QString> m_newProps;
};

} // namespace cmd
} // namespace aadl
} // namespace taste3
