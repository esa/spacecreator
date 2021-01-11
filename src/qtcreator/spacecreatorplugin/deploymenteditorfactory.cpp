#include "deploymenteditorfactory.h"

#include "deploymenteditordata.h"

#include <QGuiApplication>

namespace spctr {

DeploymentEditorFactory::DeploymentEditorFactory(QObject *parent)
    : IEditorFactory(parent)
{
}

Core::IEditor *spctr::DeploymentEditorFactory::createEditor()
{
    return editorData()->createEditor();
}

DeploymentEditorData *DeploymentEditorFactory::editorData() const
{
    if (!m_editorData) {
        m_editorData = new DeploymentEditorData;
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        m_editorData->fullInit();
        QGuiApplication::restoreOverrideCursor();
    }
    return m_editorData;
}

} // namespace spctr
