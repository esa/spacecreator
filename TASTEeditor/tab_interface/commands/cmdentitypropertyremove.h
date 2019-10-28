#pragma once

#include "tab_aadl/aadlobject.h"

#include <QVector>
#include <QUndoCommand>
#include <QPointer>

namespace taste3 {
namespace aadl {

class AADLObject;

namespace cmd {

class CmdEntityPropertyRemove : public QUndoCommand
{
public:
    explicit CmdEntityPropertyRemove(AADLObject *entity, const QStringList &props);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<AADLObject> m_entity;
    QStringList m_names;
    QVariantHash m_props;
};

} // namespace cmd
} // namespace aadl
} // namespace taste3
