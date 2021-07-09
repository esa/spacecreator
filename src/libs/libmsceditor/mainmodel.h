/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QVector>
#include <memory>

class QGraphicsScene;
class QFileInfo;
class QUndoStack;

namespace Asn1Acn {
class Asn1ModelStorage;
class Asn1SystemChecks;
}
namespace msc {
class ChartLayoutManager;
class DocumentItemModel;
class HierarchyViewModel;
class MscChart;
class MscCommandsStack;
class MscDocument;
class MscModel;

struct MainModelPrivate;

class MainModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(msc::MscDocument *selectedDocument READ selectedDocument WRITE setSelectedDocument NOTIFY
                    selectedDocumentChanged)
    Q_PROPERTY(QString currentFilePath READ currentFilePath WRITE setCurrentFilePath NOTIFY currentFilePathChanged)

public:
    static const QLatin1String MscChartMimeType;

    explicit MainModel(QObject *parent = nullptr);
    ~MainModel();

    void initialModel();

    QGraphicsScene *graphicsScene() const;
    QGraphicsScene *hierarchyScene() const;

    msc::DocumentItemModel *documentItemModel() const;

    QStringList mscErrorMessages() const;

    msc::ChartLayoutManager &chartViewModel() const;
    msc::HierarchyViewModel &hierarchyViewModel() const;

    QString modelText() const;
    msc::MscModel *mscModel() const;

    QString chartText(const msc::MscChart *chart) const;

    void setSelectedDocument(msc::MscDocument *document);
    msc::MscDocument *selectedDocument() const;

    void setCurrentFilePath(const QString &filePath);
    const QString &currentFilePath() const;

    QUndoStack *undoStack();
    msc::MscCommandsStack *commandsStack() const;
    bool needSave() const;

    QFileInfo asn1File() const;
    void setAsn1Check(Asn1Acn::Asn1SystemChecks *check);

Q_SIGNALS:
    void showChartVew();
    void modelDataChanged();
    void modelUpdated(msc::MscModel *);
    void selectedDocumentChanged(msc::MscDocument *selectedDocument);
    void currentFilePathChanged(const QString &);
    void asn1FileNameChanged(const QString &);
    void asn1ParameterErrorDetected(const QStringList &faultyMessages);

public Q_SLOTS:
    void showFirstChart();
    bool loadFile(const QString &filename);
    bool saveMsc(const QString &filename);
    void copyCurrentChart();
    void copyCurrentChartAsPicture();
    void pasteChart();

private Q_SLOTS:
    void showChartFromDocument(msc::MscDocument *document);

private:
    msc::MscChart *firstChart() const;
    msc::MscChart *firstChart(const QVector<msc::MscDocument *> &docs) const;
    void clearMscModel();
    void setNewModel(msc::MscModel *model);

    std::unique_ptr<MainModelPrivate> const d;
};

}
