/*
   Copyright (C): GMV Aerospace and Defence, S.A.U.
   Author: GMV Aerospace and Defence, S.A.U.

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

#include "dvcore/dvconnection.h"
#include "dvcore/dvmessage.h"
#include "mscdocument.h"

#include "ivfunction.h"

#include "dvcore/dvfunction.h"
#include "dvcore/dvnode.h"

#include <QCheckBox>
#include <QFileInfo>
#include <QInputDialog>
#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <functional>
#include <QTableWidgetItem>

namespace msc {
class MscModel;
}

namespace dvm{
class DVFunction;
}

namespace ive {

class InterfaceDocument;

class MBSEQualityModelCheckerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MBSEQualityModelCheckerWindow(InterfaceDocument *document, const QString projectDir, QWidget *parent = nullptr);
    ~MBSEQualityModelCheckerWindow();
    void setVisible(bool visible) override;
    //To store other models
    void DvModelsPaths(QStringList dvModelPaths);
    void DvModelStore(QVector<dvm::DVPartition* > partitions, QVector<dvm::DVFunction* > functions, QVector<dvm::DVNode* > nodes, QVector<dvm::DVMessage* > messages, QVector<dvm::DVConnection *> conections, QVector<dvm::DVDevice *> devices, QVector<dvm::DVPort *> ports, QVector<dvm::DVSystemFunction *> systemfunctions, QVector<dvm::DVSystemInterface *> systeminterfces);
    void setMscDocuments(QVector<msc::MscDocument *>);

protected:
    void closeEvent(QCloseEvent *e) override;

public Q_SLOTS:
    void callCheckMetrics(bool toggled);
    int showCheckingMessage();

Q_SIGNALS:
    void visibleChanged(bool visible);


private Q_SLOTS:
    void refreshView();
    void on_Export_clicked();
    void on_ColoursLegend_clicked();

private:
    QWidget *widgetLegend;

    struct MBSEQualityModelCheckerWindowPrivate;
    struct IVFunctionsLevel;
    struct DvModel;
    struct MscModel;
    struct SdlModel;
    struct SdlDecision;

    MBSEQualityModelCheckerWindowPrivate *d;
    IVFunctionsLevel *functionsClassified;
    DvModel          *dvModelClassified {nullptr};
    MscModel         *mscModelClassified;
    SdlModel         *sdlModels;
    ushort ivFunctionsNumber;

    QStringList sdlDiagramsPaths;
    QString projectDir;
    QDate date;
    QTime time;

    void addMetricResultToTable(QStringList metricInfo);
    void addMetricResultToSummaryTable(QStringList summaryInfo);

    void findSdlPaths(QFileInfo &parentFileInfo, int recLevel);
    void readSdlFile(QString pathFile,int modelNum);
    void iVFunctionsClassification();
    ushort numberChildFunctions(QTreeWidgetItem *parentWidgetItem, QStringList preSelection);
    ushort numberParentFunctions();
    ushort numberFunctions();
    ushort numberProvidedInterfaces();
    ushort numberRequiredInterfaces();
    ushort numberFunctionType();
    ushort numberConnections();
    ushort numberInterfaceGroups();
    ushort numberComments();
    ushort numberConnectionLayers();
    ushort numberFunctionInterfaces(ivm::IVFunction *function );

    void FunctionalAllocation();
    void FunctionalSize();
    void PhysicalSize();
    float MeanFunctionalDecomposition();
    float EffectiveFunctionalDecomposition();
    void FunctionalDecompositionHomogeneity(float metricA4_3_15, float metricA4_3_16);
    void CyclomaticComplexity();
    void DepthLevel();
    void ModularCoupling();
    void ModularCohesion();
    void ModelElementsRichness();
    void ModelDiagramsRichness();
    void ModelElementsUsageInDiagrams();
    void ModelElementsDescriptionCoverage();
    void ModelDiagramDescriptionCoverage();
    void ModelDiagramsUniqueness();
    void ModelDiagramsSize();
    void ModelElemntDiagramSize();
    void ActiveInterfaceSize(); //RID 3 New - SW tasks number metric
    void MutuallyExclusiveInterfaceSize();
    void StatesSize();
    void InterfaceDistributionHomogeneity();

    void metricationSummary();


}; //close MBSEQualityModelCheckingWindow class

}// close ive namespace
//#endif // MBSEQUALITYMODELCHECKINGWINDOW_H
