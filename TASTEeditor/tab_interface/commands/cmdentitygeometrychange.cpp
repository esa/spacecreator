#include "cmdentitygeometrychange.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

static inline QVector<qint32> convertCoordinates(const QVector<QPointF> &points)
{
    QVector<qint32> coordinates;
    for (const QPointF &point : points)
        coordinates << qRound(point.x()) << qRound(point.y());
    return coordinates;
}

namespace taste3 {
namespace aadl {
namespace cmd {

CmdEntityGeometryChange::CmdEntityGeometryChange(AADLObject *object, const QVector<QPointF> &points)
    : QUndoCommand()
    , m_entity(object)
    , m_prevCoordinates(object->coordinates())
    , m_newCoordinates(convertCoordinates(points))
{
}

void CmdEntityGeometryChange::redo()
{
    if (m_entity)
        m_entity->setCoordinates(m_newCoordinates);
}

void CmdEntityGeometryChange::undo()
{
    if (m_entity)
        m_entity->setCoordinates(m_prevCoordinates);
}

bool CmdEntityGeometryChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntityGeometryChange::id() const
{
    return ChangeEntityGeometry;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
