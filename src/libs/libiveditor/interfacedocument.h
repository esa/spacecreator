/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "common.h"
#include "entityattribute.h"

#include <QAction>
#include <QPointer>
#include <QQueue>

class QGraphicsScene;
class QGraphicsItem;
class QItemSelection;
class QItemSelectionModel;
class QMenu;
class QUndoStack;

namespace Asn1Acn {
class Asn1ModelStorage;
class Asn1SystemChecks;
}

namespace shared {
class VEObject;
}

namespace ivm {
class AbstractSystemChecks;
class IVInterface;
class IVModel;
class IVObject;
class IVConnectionLayerType;
class IVPropertyTemplateConfig;
class ArchetypeObject;
class ArchetypeModel;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class IVComponentModel;
class IVAppWidget;
class IVCommentGraphicsItem;
class IVConnectionGraphicsItem;
class IVExporter;
class IVFunctionGraphicsItem;
class IVFunctionTypeGraphicsItem;
class IVInterfaceGraphicsItem;
class IVItemModel;
class IVVisualizationModelBase;
class InterfaceTabGraphicsScene;

class InterfaceDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mscFileName READ mscFileName WRITE setMscFileName NOTIFY mscFileNameChanged)

public:
    explicit InterfaceDocument(QObject *parent = nullptr);
    ~InterfaceDocument() override;

    void init();

    IVExporter *exporter() const;

    QGraphicsScene *scene() const;

    QUndoStack *undoStack() const;
    cmd::CommandsStack *commandsStack() const;

    bool create(const QString &path = QString());
    bool load(const QString &path);
    void loadAvailableComponents();
    bool exportSelectedFunctions();
    bool exportSelectedType();
    void close();

    void showAll();

    bool hasHideableItems();
    void hideSelectedItems();
    bool hasUnselectedItems();
    void hideUnselectedItems();

    QString path() const;
    void setPath(const QString &path);

    void setProjectName(const QString &projectName);
    const QString &projectName() const;

    void setAsn1FileName(const QString &newAsnfile, const QString &oldAsnfile = {});
    QStringList asn1FilesNames() const;
    QStringList asn1FilesPaths() const;

    QString uiFileName() const;
    void setUIFileName(const QString &filePath);

    void setMscFileName(const QString &mscFile);
    const QString &mscFileName() const;
    QString mscFilePath() const;

    bool isDirty() const;

    QList<QAction *> customActions() const;

    // objectsModel
    QHash<shared::Id, shared::VEObject *> objects() const;
    ivm::IVModel *objectsModel() const;

    IVItemModel *itemsModel() const;

    IVComponentModel *componentModel() const;
    IVComponentModel *sharedTypesModel() const;

    ivm::IVModel *layersModel() const;
    QHash<shared::Id, shared::VEObject *> layersObjects() const;
    ivm::ArchetypeModel *archetypesModel() const;

    IVVisualizationModelBase *visualisationModel() const;

    QItemSelectionModel *objectsSelectionModel() const;

    //    IVVisualizationModelBase *componentModel() const;
    //    IVVisualizationModelBase *sharedVisualisationModel() const;
    IVVisualizationModelBase *layerVisualisationModel() const;

    void setAsn1Check(Asn1Acn::Asn1SystemChecks *check);
    Asn1Acn::Asn1SystemChecks *asn1Check() const;
    void setIvCheck(ivm::AbstractSystemChecks *checks);
    ivm::AbstractSystemChecks *ivCheck() const;

    QString supportedFileExtensions() const;

    bool checkInterfaceAsn1Compliance(const ivm::IVInterface *interface) const;
    bool checkAllInterfacesForAsn1Compliance();

    QList<shared::VEObject *> prepareSelectedObjectsForExport(QString &name, bool silent = false);

    ivm::IVPropertyTemplateConfig *dynPropConfig() const;

    auto updateLayersModel() const -> void;
    void loadArchetypes();
    //    ivm::IVObject *reloadComponent(ivm::IVObject *object);

    QString componentPath(const shared::Id &id) const;
    QString sharedTypePath(const shared::Id &id) const;

Q_SIGNALS:
    void dirtyChanged(bool dirty);
    void titleChanged();

    void asn1FileNameChanged(const QString &asn1FileName);
    void mscFileNameChanged(const QString &mscFileName);
    void uiFileNameChanged(const QString &uiFileName);

public Q_SLOTS:
    void onSavedExternally(const QString &filePath, bool saved);
    void setObjects(
            const QVector<ivm::IVObject *> &objects, const QHash<shared::Id, EntityAttributes> &externalAttrs = {});
    void setLayers(const QVector<ivm::IVObject *> &layers);
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

    void onDataTypesMenuInvoked();
    void prepareEntityNameForEditing(const shared::Id &id);
    void showInfoMessage(const QString &title, const QString &message);

private:
    void checkReferencedASN1Files(ivm::IVObject *object);
    bool exportImpl(QString &targetPath, const QList<shared::VEObject *> &objects);
    bool loadImpl(const QString &path);
    QString getComponentName(const QStringList &exportNames);
    bool loadComponentModel(ivm::IVModel *model, const QString &path, QHash<shared::Id, QString> *pathMappings);
    void generateArchetypeLibrary(
            QVector<ivm::ArchetypeObject *> &archetypeObjects, const QString &archetypeLibraryName);

    //    bool reloadComponentModel();
    //    bool reloadSharedTypeModel();

    void onSceneSelectionChanged(const QList<shared::Id> &selectedObjects);
    void onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void showNIYGUI(const QString &title = QString());
    void createProFile(const QString &path);
    void initTASTEEnv(const QString &path);

    QList<ivm::IVObject *> getSelectedObjects();
    QList<ivm::IVObject *> getUnselectedObjects();

    struct InterfaceDocumentPrivate;
    InterfaceDocumentPrivate *d;
};

}
