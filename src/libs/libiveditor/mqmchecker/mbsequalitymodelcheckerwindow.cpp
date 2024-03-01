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

#include "mbsequalitymodelcheckerwindow.h"
#include "dvcore/dvconnection.h"
#include "dvcore/dvfunction.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "qmessagebox.h"
#include "testgenerator/resultexporter/htmlresultexporter.h"
#include "ui_legend.h"
#include "ui_mbsequalitymodelcheckerwindow.h"

#include "ivinterface.h"
#include "ivmodel.h"
#include "ivfunctiontype.h"

#include <typeinfo>
#include "dvcore/dvfunction.h"
#include "interfacedocument.h"

#include <iostream>
#include <stdio.h>

#include <QDir>
#include <QSettings>
#include <tmc/TmcConfig/constants.h>
#include <QDesktopServices>
#include <QCheckBox>

#include "dvcore/dvbus.h"
#include "dvcore/dvconnection.h"
#include "dvcore/dvdevice.h"
#include "dvcore/dvfunction.h"
#include "dvcore/dvnode.h"
#include "dvcore/dvpartition.h"

#include <algorithm>

#include <iostream>

using namespace std;
using namespace dvm;

namespace dvm {
class DVFunction;
class DVNode;
class DVSystemFunction;
class DVModel;
}

namespace dve{
class DVAppModel;
}

namespace {
using Qt::endl;
}

namespace ive {
using namespace dvm;
class DVNode;
class DVSystemFunction;
class DVModel;

struct MBSEQualityModelCheckerWindow::MBSEQualityModelCheckerWindowPrivate {
    Ui::MBSEQualityModelCheckerWindow ui;
    InterfaceDocument *document { nullptr };
    QStringList dvModelsPaths;
    QStringList MSCDocumentsList;
};

struct MBSEQualityModelCheckerWindow::IVFunctionsLevel
{
    ushort  level;
    QString functionName;
    bool    checked;
    bool    decomposed;
    ushort pi_activeInterfaces;
    ushort pi_protectedInterfaces;
    int allInterfaces;
};

 struct MBSEQualityModelCheckerWindow::DvModel // It is assumed that only one model is possible
{
    QVector<dvm::DVPartition*> partitions { nullptr };
    QVector<dvm::DVFunction*> functions { nullptr };
    QVector<dvm::DVNode*> nodes { nullptr };
    QVector<dvm::DVMessage*> messages { nullptr };
    QVector<dvm::DVBoard*> boards { nullptr };
    QVector<dvm::DVBus*> buses { nullptr };
    QVector<dvm::DVConnection*> connections { nullptr };
    QVector<dvm::DVDevice*> devices { nullptr };
    QVector<dvm::DVPort*> ports { nullptr };
    QVector<dvm::DVSystemFunction*> systemFunctions { nullptr };
    QVector<dvm::DVSystemInterface*> systemInterfaces { nullptr };
};

struct MBSEQualityModelCheckerWindow::MscModel
{};

struct MBSEQualityModelCheckerWindow::SdlDecision
{
  QString title;
  QStringList answers;
};

struct MBSEQualityModelCheckerWindow::SdlModel
{
  SdlDecision* decisions { nullptr };
  QString title;
  int numbeDecisions;
  int numberStates;
};

MBSEQualityModelCheckerWindow::MBSEQualityModelCheckerWindow(InterfaceDocument *document, const QString projectDir, QWidget *parent)
    : QMainWindow(parent)
    , d(new MBSEQualityModelCheckerWindowPrivate)
{
   // Set UI and interface document
   d->ui.setupUi(this);
   d->document = document;

   // Define the icon of the windows
   setWindowIcon(QIcon(":/toolbar/icns/mqm_short.png"));

    // Set paths for project, properties, subtypes and results/outputs
   this->projectDir = projectDir;

    // Adjust to content size
    d->ui.centralWidget->adjustSize();
    d->ui.tableWidget_metrics_result->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QStringList headers_tables;
    headers_tables=QStringList()<<"Characteristic"<<"Sub characteristic"<<"Metric"<<"Measured Element"<<"Metric Value"<<"Target Value"<<"Minimum Target Value"<<"Maximum Target Value";
    d->ui.tableWidget_metrics_result->setColumnCount(8);
    d->ui.tableWidget_metrics_result->setHorizontalHeaderLabels(headers_tables);
    d->ui.tableWidget_metrics_result->setEditTriggers(QAbstractItemView::NoEditTriggers);

    headers_tables=QStringList()<<"Metrication summary"<<" ";
    d->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui.tableWidget->setColumnCount(2);
    d->ui.tableWidget->setHorizontalHeaderLabels(headers_tables);
    d->ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
   
    QFileInfo propertiesFileInfo(this->projectDir);
    findSdlPaths(propertiesFileInfo,0);

    int aux=0;
    sdlModels = new struct SdlModel [sdlDiagramsPaths.size()];

    for (QString str : sdlDiagramsPaths){
        readSdlFile(str,aux);
        aux++;
     }

    // Set status bar text color
    statusBar()->setStyleSheet("color: white");
    statusBar()->showMessage("Window started.", 6000);
}

void MBSEQualityModelCheckerWindow::on_ColoursLegend_clicked()
{
    widgetLegend = new QWidget(this,Qt::Window);

    Ui::Legend uiLegend;
    uiLegend.setupUi(widgetLegend);

    uiLegend.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    uiLegend.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    uiLegend.tableWidget->setItem(0,0, new QTableWidgetItem("    "));
    uiLegend.tableWidget->item(0,0)->setBackground(QBrush( QColor(234, 241, 221)));
    uiLegend.tableWidget->setItem(1,0, new QTableWidgetItem("    "));
    uiLegend.tableWidget->item(1,0)->setBackground(QBrush( QColor(229, 184, 183)));

    uiLegend.tableWidget->setItem(2,0, new QTableWidgetItem("    "));
    uiLegend.tableWidget->item(2,0)->setBackground(QBrush( QColor(219, 229, 241)));

    uiLegend.tableWidget->setItem(0,1, new QTableWidgetItem("Measured value equal to target value - CORRECT"));
    uiLegend.tableWidget->setItem(1,1, new QTableWidgetItem("Measured value different from target value - INCORRECT"));
    uiLegend.tableWidget->setItem(2,1, new QTableWidgetItem("Target value not defined"));

    widgetLegend->show();
}

void MBSEQualityModelCheckerWindow::on_Export_clicked()
{
    QStringList tableText;
    QFileInfo propertiesFileInfo(this->projectDir);

    int rows    = d->ui.tableWidget_metrics_result->rowCount();
    int columns = d->ui.tableWidget_metrics_result->columnCount();

    QDir dir;
    dir.setPath(propertiesFileInfo.filePath());

    for (int n=0; n<columns; n++){
        tableText<<d->ui.tableWidget_metrics_result->horizontalHeaderItem(n)->text();
    }
    for (int i=0; i<rows; i++){
        for (int n=0; n<columns; n++){
            tableText<<d->ui.tableWidget_metrics_result->item(i,n)->text();
        }
     }

    rows++; // To inlude the headers

    QString scriptFile = "mqm_export_metrics_to_xlsx.py";
    QStringList pythonCommandArguments = QStringList()<<scriptFile<<dir.path()<<time.toString()<<date.toString(Qt::ISODate)<<QString::number(rows)<<QString::number(columns)<<tableText;
    QProcess p;

    p.start("python3", pythonCommandArguments);


    p.waitForFinished(-1);
    QByteArray p_stdout = p.readAll();
    QString str_aux=QString(p_stdout);

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("MQM Results have been exported to Excel in:" + str_aux);
    msgBox.exec();
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberLeafFunctions 
 * Function creating a tree of QTreeWidgetItem reflecting the model functions
 */
ushort MBSEQualityModelCheckerWindow::numberChildFunctions(QTreeWidgetItem *parentWidgetItem, QStringList preSelection)
{
    ushort counter = 0;
    for (ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>()) {
            if(function->isInstanceDescendant()){
                counter++; // to count the number of functions
            }
    }
  return counter;
}
/*!
 * \brief MBSEQualityModelCheckerWindow::numberFunctionInterfaces 
 * It returns all the interfaces(provided and required) defined for a specific function
 */
ushort MBSEQualityModelCheckerWindow::numberFunctionInterfaces(ivm::IVFunction *function)
{ 
    ushort counter = 0;
    QVector<ivm::IVInterface*> interfaces = function->allInterfaces(); // It stores all interfaces in a vector
    counter = interfaces.size(); //Number of function's 'interfaces 
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberProvidedInterfaces 
 * It returns all the provided interfaces defined in the model
 */
ushort MBSEQualityModelCheckerWindow::numberProvidedInterfaces()
{
    ushort counter = 0;
    for (ivm::IVInterface *interfaces : d->document->objectsModel()->allObjectsByType<ivm::IVInterface>()){
        if(interfaces->isProvided()){
            counter++;
        }
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberProvidedInterfaces 
 * It returns all the required interfaces defined in the model
 */
ushort MBSEQualityModelCheckerWindow::numberRequiredInterfaces()
{
    ushort counter = 0;
    for (ivm::IVInterface *interfaces : d->document->objectsModel()->allObjectsByType<ivm::IVInterface>()){
        if(interfaces->isRequired()){
            counter++;
        }
    }
     return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberParentFunctions 
 * Function creating a tree of QTreeWidgetItem reflecting the model Functions and returns the number of parent functions
 */
ushort MBSEQualityModelCheckerWindow::numberParentFunctions()
{
    ushort counter = 0;
    for (ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>()) {
         if (function->hasNestedChildren()) {
            counter++;
         }
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberFunctions 
 * Function creating a tree of QTreeWidgetItem reflecting the model Functions and returns the number of functions whitout distintion of parent/child
 */
ushort MBSEQualityModelCheckerWindow::numberFunctions()
{
    ushort counter = 0;
    for (ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>()) {
         counter++; 
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberFunctionsType 
 * This function returs the number of functions Type in the model
 */
ushort MBSEQualityModelCheckerWindow::numberFunctionType()
{
    ushort counter = 0;
    for (ivm::IVObject *functiontype : d->document->objectsModel()->allObjectsByType<ivm::IVObject>()) {
        if(functiontype->isFunctionType()){ 
            counter++;// to access the number of functions
        }
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerrWindow::numberFunctionsType 
 * This function returs the number of connections in the model
 */
ushort MBSEQualityModelCheckerWindow::numberConnections()
{
    ushort counter = 0;
    for (ivm::IVObject *Object : d->document->objectsModel()->allObjectsByType<ivm::IVObject>()) {
        if(Object->isConnection())
       { counter++;// to access the number of  connection groups
        }

    }

return counter;
}
/*!
 * \brief MBSEQualityModelCheckerWindow::numberInterfaceGroups
 * This function returns the number of Interface Groups in the model
 */
ushort MBSEQualityModelCheckerWindow::numberInterfaceGroups()
{
    ushort counter = 0;
    for (ivm::IVObject *interfaceGrup : d->document->objectsModel()->allObjectsByType<ivm::IVObject>())
    {
        if(interfaceGrup->isInterfaceGroup()){ 
            counter++; 
         }
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberComments
 * This function returs the number of Comments in the model
 */
ushort MBSEQualityModelCheckerWindow::numberComments()
{
    ushort counter = 0;
    for (ivm::IVObject *comments : d->document->objectsModel()->allObjectsByType<ivm::IVObject>()) {
        if(comments->isComment()){ 
            counter++;
        }
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::numberConnectionLayers
 * This function returns the number of Connection Layers in the model
 */
ushort MBSEQualityModelCheckerWindow::numberConnectionLayers()
{
    ushort counter = 0;
    for (ivm::IVObject *object : d->document->objectsModel()->allObjectsByType<ivm::IVObject>()) {
        if(object->isConnectionLayer()){ 
            counter++;
        }
    }
    return counter;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::iVFunctionsClassification
 * This function classifies and stores the level of decomposition of all ivfunction
 */
void MBSEQualityModelCheckerWindow::iVFunctionsClassification()
{ 
    ushort count = 0;
    ushort activeInterfaces=0;
    ushort protectedInterfaces=0;
    bool found = false;
    QVector<ivm::IVFunction*> functionAll = d->document->objectsModel()->allObjectsByType<ivm::IVFunction>();
    ivFunctionsNumber = functionAll.size();
    if(ivFunctionsNumber>0)
    {
        functionsClassified = new struct IVFunctionsLevel [ivFunctionsNumber];

        while(count < functionAll.size())
        {
            for(ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>())
            {
                found = false;

                for(int i=0; i < count; i++)
                {
                    if(function->title() == functionsClassified[i].functionName)
                    {
                        found=true;
                        if(functionsClassified[i].checked == false)
                        {
                            functionsClassified[i].checked = true;
                            if (function->hasNestedChildren())
                            {
                                for (ivm::IVFunction *childFunction: function->functions())
                                {
                                    functionsClassified[count].functionName = childFunction->title();
                                    functionsClassified[count].level = functionsClassified[i].level + 1;
                                    functionsClassified[count].checked = false;
                                    // register of interfaces types

                                    for (IVInterface *interface :childFunction->pis())
                                    {
                                       if( interface->kind()==ivm::IVInterfaceProvided::OperationKind::Sporadic|| interface->kind()==ivm::IVInterfaceProvided::OperationKind::Cyclic) // counting name of active interfaces
                                                    {activeInterfaces++;}
                                                    else if  (interface->kind()==ivm::IVInterfaceProvided::OperationKind::Protected|| interface->kind()==ivm::IVInterfaceProvided::OperationKind::Unprotected)
                                                    {protectedInterfaces++;}// counting name of protected interfaces
                                    }
                                    functionsClassified[count].pi_activeInterfaces =activeInterfaces;
                                    functionsClassified[count].pi_protectedInterfaces =protectedInterfaces;
                                    functionsClassified[count].allInterfaces=childFunction->interfaces().size();
                                    activeInterfaces=0;
                                    protectedInterfaces=0;
                                    if(childFunction->hasNestedChildren())
                                    {
                                        functionsClassified[count].decomposed = true;
                                    }
                                    else
                                    {
                                        functionsClassified[count].decomposed = false;
                                    }
                                    count++;
                                }
                            }
                        }
                    }
                }

                if(!found) // function not register in the array of classified functions
                {
                    if (!function->isNested())
                    {
                        functionsClassified[count].functionName = function->title();
                        functionsClassified[count].level = 1; //if is the top level funciton level assigned is 1
                        functionsClassified[count].checked = true;
                        // register of interfaces types
                        for (IVInterface *interface :function->pis())
                        {
                           if( interface->kind()==ivm::IVInterfaceProvided::OperationKind::Sporadic|| interface->kind()==ivm::IVInterfaceProvided::OperationKind::Cyclic) // counting name of active interfaces
                                        {activeInterfaces++;}
                                        else if  (interface->kind()==ivm::IVInterfaceProvided::OperationKind::Protected|| interface->kind()==ivm::IVInterfaceProvided::OperationKind::Unprotected)
                                        {protectedInterfaces++;}// counting name of protected interfaces
                        }
                        functionsClassified[count].pi_activeInterfaces =activeInterfaces;
                        functionsClassified[count].pi_protectedInterfaces =protectedInterfaces;
                        functionsClassified[count].allInterfaces=function->interfaces().size();
                        activeInterfaces=0;
                        protectedInterfaces=0;
                        count++;
                        if (function->hasNestedChildren())
                        {
                            functionsClassified[count-1].decomposed = true;
                            for (ivm::IVFunction *childFunction: function->functions())
                            {
                                if(childFunction->isFunction())
                                {
                                    functionsClassified[count].functionName = childFunction->title();
                                    functionsClassified[count].level = 2; //  if the top level function is decoposed the child is assigned to level 2
                                    functionsClassified[count].checked = false;
                                    // register of interfaces types
                                    for (IVInterface *interface :childFunction->pis())
                                    {
                                       if( interface->kind()==ivm::IVInterfaceProvided::OperationKind::Sporadic|| interface->kind()==ivm::IVInterfaceProvided::OperationKind::Cyclic) // counting name of active interfaces
                                                    {activeInterfaces++;}
                                                    else if  (interface->kind()==ivm::IVInterfaceProvided::OperationKind::Protected|| interface->kind()==ivm::IVInterfaceProvided::OperationKind::Unprotected)
                                                    {protectedInterfaces++;}// counting name of protected interfaces
                                    }
                                    functionsClassified[count].pi_activeInterfaces =activeInterfaces;
                                    functionsClassified[count].pi_protectedInterfaces =protectedInterfaces;
                                    functionsClassified[count].allInterfaces=childFunction->interfaces().size();
                                    activeInterfaces=0;
                                    protectedInterfaces=0;

                                    if(childFunction->hasNestedChildren())
                                    {
                                        functionsClassified[count].decomposed=true;
                                    }
                                    else{
                                        functionsClassified[count].decomposed=false;
                                    }
                                    count++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*!
 * \brief MBSEQualityModelCheckerWindow::addMetricResultToTable
 * Function to add all the information of metric results in the MQM windows. 7 strings shall be always received
 */

void MBSEQualityModelCheckerWindow::addMetricResultToTable(QStringList metricInfo)
{
    int i = 0;
    int numberRows = d->ui.tableWidget_metrics_result->rowCount(); // add a row to each metric result

    d->ui.tableWidget_metrics_result->setRowCount(numberRows+1);

    for(QString text:metricInfo){
        d->ui.tableWidget_metrics_result->setItem(numberRows,i, new QTableWidgetItem(text));
        i++;
        if (i > 7){ i = 7; } // To ensure info is not written out of the table
    }

    if(metricInfo[4]!="[METRIC COULD NOT BE COMPUTED]")
    {   
        QColor background2;
        if (metricInfo[5] != "-")
        {
            float result = metricInfo[4].toFloat();
            float target = metricInfo[5].toFloat();

            if (result == target){ background2 = QColor(234, 241, 221);}
            else { background2 = QColor(229, 184, 183);}
        }
        else
        {
            if(metricInfo[6]!="-")
            {
                float result  = metricInfo[4].toFloat();
                float targetMin = metricInfo[6].toFloat();
                float targetMax = metricInfo[7].toFloat();

                if(result >= targetMin && result <= targetMax) { background2 = QColor(234, 241, 221);} // green background
                else { background2 = QColor(229, 184, 183);} // red background
               }
            else  
            { 
                background2=QColor(219, 229, 241); // blue background
            } 
        }

        for(int i =0; i<8;i++)
        {
            d->ui.tableWidget_metrics_result->item(numberRows,i)->setBackground(QBrush(background2));
         }
    }
}

/*!
 * \brief MBSEQualityModelCheckerWindow::addMetricResultToSummaryTable
 * Function to show the summary table in the MQM windows. 2 strings shall be always received
 */
void MBSEQualityModelCheckerWindow::addMetricResultToSummaryTable(QStringList summaryInfo)
{
    int i =0;
    int numberRows = d->ui.tableWidget->rowCount(); // add a row to each metric result

    d->ui.tableWidget->setRowCount(numberRows + 1);

    for(QString text:summaryInfo)
    {
        d->ui.tableWidget->setItem(numberRows, i, new QTableWidgetItem(text));
        i++;
        if (i>2){ i = 2;}// To ensure info is not writen out of the table
    }
}

/*!
 * \brief MBSEQualityModelCheckerWindow::setMscFiles
 * Function to store all the paths of the MSC files to access later if needed.
 */
void MBSEQualityModelCheckerWindow::setMscDocuments(QVector<msc::MscDocument *>documents)
{
    d->MSCDocumentsList =QStringList(); // delete all previous contained

   for(msc::MscDocument* document: documents){ // for created to avoid counting parent MSC elements as MSC diagrams. Only leaf elements are computed as diagrams
     if(document->hierarchyType() == msc::MscDocument::HierarchyLeaf)
     {
        d->MSCDocumentsList<<document->name();
      }
   }
}

void MBSEQualityModelCheckerWindow::DvModelsPaths(QStringList dvModelPaths)
{
    d->dvModelsPaths = dvModelPaths;
}

void MBSEQualityModelCheckerWindow::DvModelStore(QVector<dvm::DVPartition*> partitions,QVector<dvm::DVFunction*> functions, 
                                                  QVector<dvm::DVNode*> nodes, 
                                                  QVector<dvm::DVMessage* > messages,
                                                  QVector<dvm::DVConnection*> connections,
                                                  QVector<dvm::DVDevice*> devices,
                                                  QVector<dvm::DVPort*> ports,
                                                  QVector<dvm::DVSystemFunction*> systemFunctions,
                                                  QVector<dvm::DVSystemInterface*> systemInterfaces)

{
    dvModelClassified = new struct DvModel;
    dvModelClassified->partitions = partitions;
    dvModelClassified->functions = functions;
    dvModelClassified->nodes = nodes;
    dvModelClassified->messages = messages;
    dvModelClassified->connections = connections;
    dvModelClassified->devices = devices;
    dvModelClassified->ports = ports;
    dvModelClassified->systemFunctions = systemFunctions;
    dvModelClassified->systemInterfaces = systemInterfaces;
 }

void MBSEQualityModelCheckerWindow::findSdlPaths(QFileInfo &parentFileInfo, int recLevel)
{
    QDir dir;
    dir.setPath(parentFileInfo.filePath());
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst | QDir::Name);
    Qt::CheckState checkState;
    bool parentIsFullyChecked = true;
    bool parentIsUnchecked = true;

    const QFileInfoList fileList = dir.entryInfoList();
    for (int i = 0; i < fileList.size(); i++) 
    {
        QFileInfo childFileInfo = fileList.at(i);
        if (childFileInfo.fileName() != "." && childFileInfo.fileName() != ".."&&childFileInfo.isDir()) { // is directory
        { // constrain to 1 level hierarchy
            if(childFileInfo.fileName()!="modelchecking"){
                findSdlPaths( childFileInfo, recLevel + 1);} // constrain to 1 level hierarchy
            }
        } 
        else { // is file
            const QString childFileSuffix = childFileInfo.suffix();
            if (childFileSuffix == "pr") {
                 QString filepath=dir.path()+"/"+childFileInfo.fileName();
                 sdlDiagramsPaths<<filepath;
            }
        }
    }
}

void MBSEQualityModelCheckerWindow:: readSdlFile(QString pathFile,int modelNum)
{
    QFileInfo f(pathFile);
    sdlModels[modelNum].title = f.fileName();

    QString scriptFile = "readSDL.py";
    QStringList pythonCommandArguments = QStringList()<<scriptFile<<pathFile;
    QProcess p;


    p.start("python3", pythonCommandArguments);

    p.waitForFinished(-1);
    QByteArray p_stdout = p.readAll();
    QString str_aux=QString(p_stdout);
    QStringList strings;
    QList<QByteArray>list= p_stdout.split(',');

    // remove special characters received ib
   for ( QByteArray &item : list) {
        QString str_aux = QString(item);
        str_aux.remove("\\n'");
        str_aux.remove("\n'");
        str_aux.remove(QRegularExpression("[' ]"));
        str_aux.remove("[");
        str_aux.remove("]");
        strings<<str_aux;
   }
    ushort count = 0;
    if (!strings.empty())
       {
       sdlModels[modelNum].numberStates=strings[0].toInt();
       strings.removeFirst();
       }
    for (QString str_aux : strings)
    {
        if (str_aux == "decision"){ count++; }
    }
    
    sdlModels[modelNum].numbeDecisions = count;
    
    if (count != 0)
    {
        sdlModels[modelNum].decisions = new struct SdlDecision[count];
        int i = -1;
        bool nextDecision = false;
        for (QString str_aux:strings)
        {
            if(nextDecision == true)
            { 
                sdlModels[modelNum].decisions[i].title = str_aux;
                nextDecision=false;
            }
            else
            {
                 if (str_aux=="decision"){
                    nextDecision=true; 
                    i++;
                 }
                 else{ 
                    str_aux.remove(QRegularExpression("[:() ]"));
                    sdlModels[modelNum].decisions[i].answers<<str_aux;
                 }
            }
         }
     }

}

MBSEQualityModelCheckerWindow::~MBSEQualityModelCheckerWindow()
{
    delete d;
    d = nullptr;
}

//! This emits visibleChanged after calling the standard setVisible method.
//! And it refreshes the view if this is now visible.
//! This is used to keep the "show model checking" menu action updated.
void MBSEQualityModelCheckerWindow::setVisible(bool visible)
{
    const bool wasVisible = isVisible();
    QWidget::setVisible(visible);
    if (isVisible() != wasVisible) {
        Q_EMIT visibleChanged(visible);
        if (isVisible()) {
            refreshView();
        }
    }
}

//! Needed by MBSEQualityModelCheckerWindow::setVisible
void MBSEQualityModelCheckerWindow::refreshView()
{
    return;
}

void MBSEQualityModelCheckerWindow::closeEvent(QCloseEvent *e)
{
    QMainWindow::closeEvent(e);
     QWidget::closeEvent(e);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::callCheckMetrics 
 * This function launches a mensage box and check all metrics if yes is pressed
 */
void MBSEQualityModelCheckerWindow::callCheckMetrics(bool toggled)
{
    if (!toggled) // this avoids launching the sequence when MQM IV interface widget is toggled by unclick
    {
        return;
    }

    iVFunctionsClassification();// classify functions in every check to ensure iv classification is updated with the last version;


    if(dvModelClassified != nullptr){FunctionalAllocation();}

    else {
            QStringList metric = QStringList()<<"Functionality"<<"Completeness"<<"Functional allocation" <<"DV not defined" <<"0"<<"1"<<"0"<<"1";
            addMetricResultToTable(metric);
        }
    FunctionalSize();
    if(dvModelClassified != nullptr)
    {  PhysicalSize();    }
    else
        {
        QStringList metric = QStringList()<<"Functionality"<<"Size"<<"Physical size"<<"DV not defined"  <<"0"<<"0"<<"0"<<"-";
        addMetricResultToTable(metric);
         }
    ActiveInterfaceSize();
    MutuallyExclusiveInterfaceSize();
    if(!sdlDiagramsPaths.empty())    {  StatesSize(); }

    else
    {
        QStringList metric = QStringList()<<"Functionality"<<"Size"<<"States size"<<"Behavioural Diagrams not defined"  <<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
        addMetricResultToTable(metric);
    }


    float metricMeanFunctionalDecomposition = MeanFunctionalDecomposition();
    float metricEffectiveFunctionalDecomposition = EffectiveFunctionalDecomposition();
    FunctionalDecompositionHomogeneity(metricMeanFunctionalDecomposition,metricEffectiveFunctionalDecomposition);

    if(!sdlDiagramsPaths.empty())    {CyclomaticComplexity();    }
    else
    {
        QStringList metric = QStringList()<<"Maintainability"<<"Complexity"<<"Cyclomatic complexity"<<"Behavioural Diagrams not defined"  <<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
        addMetricResultToTable(metric);

    }
    DepthLevel();
    InterfaceDistributionHomogeneity();
    ModularCoupling();
    ModularCohesion();
    ModelElementsRichness();
    ModelDiagramsRichness();
    ModelElementsUsageInDiagrams();
    ModelElementsDescriptionCoverage();
    ModelDiagramDescriptionCoverage();
    ModelDiagramsUniqueness();
    ModelDiagramsSize();
    ModelElemntDiagramSize();






    metricationSummary();
    date= QDate::currentDate();
    time=QTime::currentTime();

}

void MBSEQualityModelCheckerWindow:: metricationSummary()
{
    float percentageComputed = 0;
    float numTargetDef=0;
    float computed = 0;
    float computedType = 0;
    QStringList nameComputedMetrics;
    int rows = d->ui.tableWidget_metrics_result->rowCount();

    for(int i=0; i<rows; i++)
    {
        QBrush backgound = d->ui.tableWidget_metrics_result->item(i,0)->background();
        QColor color = backgound.color();
        QColor red = QColor(229, 184, 183);
        QColor green = QColor(234, 241, 221);
        QColor blue = QColor(219, 229, 241);
        if (color == red || color == green || color == blue)
        {
            computed++;
            nameComputedMetrics<<d->ui.tableWidget_metrics_result->item(i,2)->text();
        }
        if (color == red || color == green ){numTargetDef++;}
    }

    nameComputedMetrics.removeDuplicates();
    computedType=nameComputedMetrics.size();

    QStringList summaryCategory;
    summaryCategory = QStringList()<<"Number of types of metrics"<<QString::number(22);
    addMetricResultToSummaryTable(summaryCategory);
    summaryCategory = QStringList()<<"Number of types of metrics computed"<<QString::number(computedType);
    addMetricResultToSummaryTable(summaryCategory);

        percentageComputed = (computedType/22)*100;

    summaryCategory = QStringList()<<"Percentage of types of metrics computed"<<QString::number(percentageComputed,'f',2)+"%";
    addMetricResultToSummaryTable(summaryCategory);

    if (computed > 0)
    {
        percentageComputed=(numTargetDef/computed)*100;
    }
    summaryCategory = QStringList()<<"Percentage of metrics with target value computed"<<QString::number(percentageComputed,'f',2)+"%";
    addMetricResultToSummaryTable(summaryCategory);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::showCheckingMessage 
 * This function sets the mensage box requested by callCheckMetrics Function
 */
int MBSEQualityModelCheckerWindow::showCheckingMessage()
{
    QMessageBox msgBox;
    msgBox.setText("MQM Checker is going to be launched");
    msgBox.setInformativeText("Do you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    return ret;
}

// LIST OF METRICS-------------------------------------------------------
/*!
 * \brief MBSEQualityModelCheckerWindow::FunctionalAllocation 
 * This function computes the Functional Allocation
 */
void MBSEQualityModelCheckerWindow::FunctionalAllocation()
{
    float metricResult = 0.0;
    QStringList metric;

    QStringList IVFunctionsLeafs;

    for(int i=0; i<ivFunctionsNumber; i++) // if any function is defined the for is not produced
    {
        if(functionsClassified[i].decomposed == false)
        {
            IVFunctionsLeafs<<functionsClassified[i].functionName;
        }
    }

    for(DVFunction *function : dvModelClassified->functions)
    {
        if(IVFunctionsLeafs.contains(function->title()))
        {
            metricResult++;
        }
    }
    if (IVFunctionsLeafs.size() > 0)
    {
        metricResult = metricResult/IVFunctionsLeafs.size();
    }

    if(metricResult > 0) //At least a iv diagram exists always if any function is defined metric could not be computed
    { 
        metric =QStringList()<<"Functionality"<<"Completeness"<<"Functional allocation" <<"Allocation of Function to Partition"<<QString::number(metricResult,'f',2)<<"1"<<"0"<<"1";
    }
    else
    {
        metric =QStringList()<<"Functionality"<<"Completeness"<<"Functional allocation"<<"Allocation of Function to Partition"<<"[METRIC COULD NOT BE COMPUTED]"<<"1"<<"0"<<"1";
    }
    addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::FunctionalSize 
 * This function computes the Functional Size
 */
void MBSEQualityModelCheckerWindow::FunctionalSize()
{
    ushort metricResult = numberFunctions();
    QStringList metric;

    if(metricResult >= 0){// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error

        metric = QStringList()<<"Functionality"<<"Size"<<"Functional size" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    { 
        metric = QStringList()<<"Functionality"<<"Size"<<"Functional size" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
    }
    addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::PhysicalSize 
 * This function computes the Physical Size
 */
void MBSEQualityModelCheckerWindow::PhysicalSize()
{   
    float metricResult = 0.0;
    QStringList metric;

    metricResult= dvModelClassified->partitions.size()
                 +dvModelClassified->nodes.size()
                 +dvModelClassified->connections.size()
                 +dvModelClassified->devices.size();


    if(metricResult >=0 )// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error
    {
        metric = QStringList()<<"Functionality"<<"Size"<<"Physical size" <<"Allocation "<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";;
    }
    else
    {
        metric = QStringList()<<"Functionality"<<"Size"<<"Physical size" <<"Allocation"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
    }
    addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::MeanFunctionalDecomposition 
 * This function computes the Mean functional decomposition
 */
float MBSEQualityModelCheckerWindow::MeanFunctionalDecomposition()
{
    float metricResult = 0.0;
    float maxLevelDecomp = 0;
    QStringList metric;

    for (int i=0; i<ivFunctionsNumber; i++)// if any Function is defined maxLevelDecom is 0, and for is not produced
    {
        if (maxLevelDecomp < functionsClassified[i].level)
        {
            maxLevelDecomp = functionsClassified[i].level;
        }
    }

    for(int i=1; i<maxLevelDecomp; i++)  //if any Function is defined maxLevelDecom is 0, and foris not produced
    {
        float levelA = 0;
        float levelB = 0;
        for(int n=0; n<ivFunctionsNumber; n++)
        {
            if (functionsClassified[n].level == i){levelA++;}
            else{
                if (functionsClassified[n].level == (i+1)){levelB++;}
            }
        }

        metricResult = metricResult + (levelB/levelA); // if any funcion exist the top level fuction is 1= levelA,
    }
    
    if(maxLevelDecomp > 1)
    { 
       metricResult= metricResult/((maxLevelDecomp)-1.0);
    }


    if(maxLevelDecomp > 0)
    {
        metric =QStringList()<<"Maintainability"<<"Complexity"<<"Mean functional decomposition" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"5"<<"9";
    }
    else
    {
        metric =QStringList()<<"Maintainability"<<"Complexity"<<"Mean functional decomposition" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"5"<<"9";
    }
    addMetricResultToTable(metric);
    return metricResult;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::EffectiveFunctionalDecomposition
 * This function computes Effective functional decomposition
 */
float MBSEQualityModelCheckerWindow::EffectiveFunctionalDecomposition()
{
    ushort maxLevelDecomp = 0;
    QStringList metric;
    float metricResult = 0.0;
    float aux = 0;

    for (int i=0; i<ivFunctionsNumber; i++) //if any Function for is not produced
    {
        if (maxLevelDecomp<functionsClassified[i].level)
        {
            maxLevelDecomp = functionsClassified[i].level;}
        }

    for (int i=1; i<maxLevelDecomp; i++)
    {
        float levelA = 0;
        float levelB = 0;
        for (int n=0; n<ivFunctionsNumber; n++)
        {
            if (functionsClassified[n].level == i&&functionsClassified[n].decomposed)
            {   
                levelA++;
            }
            else
            {
                if (functionsClassified[n].level == (i+1)){levelB++;}
            }
        }
        metricResult = metricResult + (levelB/levelA); // If any fuction exist levelA is 1
    }

    if (maxLevelDecomp>1)
    {
        aux=1.0/(static_cast<float>(maxLevelDecomp)-1.0);
    }

    metricResult = metricResult*aux;

    if(maxLevelDecomp>=0) // if there isn't any function decomposed then decomposition is 0
    {
        metric = QStringList()<<"Maintainability"<<"Complexity"<<"Effective functional decomposition" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"5"<<"9";
    }
    else
    {
        metric =QStringList()<<"Maintainability"<<"Complexity"<<"Effective functional decomposition" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"5"<<"9";
    }
    addMetricResultToTable(metric);
    return metricResult;
}

/*!
 * \brief MBSEQualityModelCheckerWindow::FunctionalDecompositionHomogeneity
 * This metric computes the Functional decomposition homogeneity
 */
void MBSEQualityModelCheckerWindow::FunctionalDecompositionHomogeneity(float metricMeanFunctionalDecomposition, float metricEffectiveFunctionalDecomposition )
{
     QStringList metric;
     float metricResult;
     if(metricMeanFunctionalDecomposition > 0)
     {
         metricResult = metricEffectiveFunctionalDecomposition/metricMeanFunctionalDecomposition;
         metric = QStringList()<<"Maintainability"<<"Complexity"<<"Functional decomposition homogeneity" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"1"<<"1.2";
     }
     else
     { 
        metric =QStringList()<<"Maintainability"<<"Complexity"<<"Functional decomposition homogeneity" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"1"<<"1.2";
     }
     addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::CyclomaticComplexity 
 * This function computes the Cyclomatic Complexity
 */
void MBSEQualityModelCheckerWindow::CyclomaticComplexity()
{
    float metricResult;
    for(int i=0; i<sdlDiagramsPaths.size(); i++)
    {
         metricResult = 0.0;
         QStringList metric;
         QString aux= sdlModels[i].title;
         aux.remove(".pr",Qt::CaseSensitive);
         for (int j=0;j<ivFunctionsNumber;j++)
         {
             if(aux.contains(functionsClassified[j].functionName, Qt::CaseInsensitive))
             {
                 j=ivFunctionsNumber;
                 for (int a=0; a<sdlModels[i].numbeDecisions; a++)
                 {
                    metricResult = metricResult + (sdlModels[i].decisions[a].answers.size()-1);
                 }
                 metricResult = metricResult + 1;

                 if(metricResult > 0)//if a state machine is defined at least cyclomatic complexity is 1
                 {
                    metric =QStringList()<<"Maintainability"<<"Complexity"<<"Cyclomatic complexity" <<"Behavioural Diagram: "+ sdlModels[i].title<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
                 }
                 else
                 {
                    metric =QStringList()<<"Maintainability"<<"Complexity"<<"Cyclomatic complexity" <<"Behavioural Diagram: "+sdlModels[i].title<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
                 }
                 addMetricResultToTable(metric);
             }
        }
     }
 }

 /*!
  * \brief MBSEQualityModelCheckerWindow::DepthLevel 
  * This function computes the Depth level
  */
 void MBSEQualityModelCheckerWindow::DepthLevel()
 {    float metricResult=0;
      QStringList metric;

      for (int i=0; i<ivFunctionsNumber; i++)// if any Function is and for is not produced
      {
        if (metricResult<functionsClassified[i].level)
        {
            metricResult = functionsClassified[i].level;
        }
      }
      if(metricResult >= 0) // asuming that depth level could be 0
      {
        metric =QStringList()<<"Maintainability"<<"Complexity"<<"Depth level" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
      }
      else
      { 
        metric =QStringList()<<"Maintainability"<<"Complexity"<<"Depth level" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
      }
      addMetricResultToTable(metric);
}

 /*!
  * \brief MBSEQualityModelCheckerWindow::ModularCoupling
  * This function computes the Modular coupling
  */
 void MBSEQualityModelCheckerWindow::ModularCoupling()
 {
     float metricResult = 0.0;
     float numberDecomposedFunctions = numberParentFunctions();
     QStringList metric;

     for(int i=0; i<ivFunctionsNumber; i++)//if any Function is and for is not produced
     {   
        float coupling = 0.0;
        float numberConnections = 0.0;
        if(functionsClassified[i].decomposed)
        {
             ivm::IVFunction *ivFunction = d->document->objectsModel()->getFunction(functionsClassified[i].functionName, Qt::CaseInsensitive);
             for(ivm::IVConnection *connections: ivFunction->connections())
             {
                numberConnections++;
                if (connections->targetName() == functionsClassified[i].functionName || connections->sourceName() == functionsClassified[i].functionName)
                {
                    coupling++;
                }
             }
             if(numberConnections != 0)
             {
                metricResult = metricResult + (coupling/numberConnections);
             }
         }
     }

     if(numberDecomposedFunctions!=0)
     {
        metricResult = metricResult/numberDecomposedFunctions;
     }
     else
     {
        metricResult = 0.0;
     }

     if(metricResult >= 0)// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error
     {
        metric =QStringList()<<"Maintainability"<<"Modularity"<<"Modular coupling" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
     }
     else
     {
        metric =QStringList()<<"Maintainability"<<"Modularity"<<"Modular coupling" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
     }
     addMetricResultToTable(metric);
 }

 /*!
  * \brief MBSEQualityModelCheckerWindow::ModularCohesion 
  * This function computes the Modular cohesion
  */
 void MBSEQualityModelCheckerWindow::ModularCohesion()
 {
     float metricResult = 0.0;
     float numberDecomposedFunctions = numberParentFunctions();
     QStringList metric;

     for(int i=0; i<ivFunctionsNumber; i++)//if any Function is and for is not produced
     {   
        float cohesedConectors = 0.0;
        float numberConnections = 0.0;
        if(functionsClassified[i].decomposed)
        {
             ivm::IVFunction *ivFunction = d->document->objectsModel()->getFunction(functionsClassified[i].functionName, Qt::CaseInsensitive);
             for (ivm::IVConnection *connections: ivFunction->connections())
             {
                numberConnections++;
                if (connections->targetName() != functionsClassified[i].functionName && connections->sourceName() != functionsClassified[i].functionName)
                {   
                    cohesedConectors++;
                }
             }
             if(numberConnections!=0)
             {
                metricResult = metricResult + (cohesedConectors/numberConnections);
             }
        }
     }

     if (numberDecomposedFunctions!=0)
     {
        metricResult = metricResult/numberDecomposedFunctions;
     }
     else 
     {
        metricResult = 0.0;
     }

     if(metricResult >= 0)// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error
     {
        metric =QStringList()<<"Maintainability"<<"Modularity"<<"Modular cohesion" <<"Function"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
     }
     else
     {
        metric =QStringList()<<"Maintainability"<<"Modularity"<<"Modular cohesion"<<"Function" <<"[METRIC COULD NOT BE COMPUTED]";
     }
     addMetricResultToTable(metric);
 }

/*!
 * \brief MBSEQualityModelCheckerWindow::ModelElementsRichness
 * This function computes the Model elements richness
 */
void MBSEQualityModelCheckerWindow::ModelElementsRichness()
{
    ushort metricResult = 0;
    ushort *numberElemens = new ushort[9];
    QStringList metric;
    // Compute metric for IV diagram
     numberElemens[0] = numberFunctions();
     numberElemens[1] = numberProvidedInterfaces();
     numberElemens[2] = numberRequiredInterfaces();
     numberElemens[3] = numberFunctionType();
     numberElemens[4] = numberInterfaceGroups();
     numberElemens[5] = numberComments();
     numberElemens[6] = numberConnections();
     numberElemens[7] = numberConnectionLayers();

    for(ushort i=0; i<8; i++)
    {
        if (numberElemens[i]>0)
        {
            metricResult++;
        }
    }

    if(metricResult >= 0){// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error
        metric = QStringList()<<"Functionality"<<"Model coverage "<<"Model elements richness" <<"IV Diagram"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    {
        metric = QStringList()<<"Functionality"<<"Model coverage "<<"Model elements richness"<<"IV Diagram" <<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
    }
     addMetricResultToTable(metric);
// Compute metric for DV diagram
     for(ushort i=0; i<9; i++)
     {
        numberElemens[i]=0;
     }
     if(dvModelClassified != nullptr)
    {
         numberElemens[0] = dvModelClassified->partitions.size();
         numberElemens[1] = dvModelClassified->functions.size();
         numberElemens[2] = dvModelClassified->nodes.size();
         numberElemens[3] = dvModelClassified->messages.size();
         numberElemens[4] = dvModelClassified->connections.size();
         numberElemens[5] = dvModelClassified->devices.size();
         numberElemens[6] = dvModelClassified->ports.size();
         numberElemens[7] = dvModelClassified->systemFunctions.size();
         numberElemens[8] = dvModelClassified->systemInterfaces.size();
    }


    metricResult = 0;

    for(ushort i=0; i<9; i++)
    {
        if (numberElemens[i] > 0)
        {metricResult++;}
    }

    if(metricResult >= 0)// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error
    {
        metric =QStringList()<<"Functionality"<<"Model coverage "<<"Model elements richness" <<"DV Diagram"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    {
        metric =QStringList()<<"Functionality"<<"Model coverage "<<"Model elements richness"<<"DV Diagram" <<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
    }
     addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::ModelDiagramsRichness
 * This function computes the Model diagrams richness
 */
void MBSEQualityModelCheckerWindow::ModelDiagramsRichness()
{
    ushort metricResult = 1;// is initialized as 1 because IV diagram must exixt even if it's empty
    QStringList metric;

    if (dvModelClassified != nullptr){metricResult++;}
    if (!sdlDiagramsPaths.isEmpty()){metricResult++;}
    if (!d->MSCDocumentsList.isEmpty()){metricResult++;}

    if(metricResult >= 0)// if any function is defined the metric still being correct but any (hipotetical) case that the result is negative is an error
    {
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model diagram richness"<<"All Model Diagrams" <<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    {
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model diagram richness" <<"All Model Diagrams"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
    }

    addMetricResultToTable(metric);
}
/*!
 * \brief MBSEQualityModelCheckerWindow::ModelElementsUsageInDiagrams
 * This function computes the Model elements usage in diagrams
 */
void MBSEQualityModelCheckerWindow::ModelElementsUsageInDiagrams()
{
      float metricResult = 0.0;
      QStringList metric;

float visibleElements=0.0;
float definedElements=0.;
QStringList topLevelNamesFucntions;



      for(ivm::IVObject *element : d->document->objectsModel()->allObjectsByType<ivm::IVObject>())
      { if(element->isFunctionType()||element->isFunction())
          {definedElements++;
              if(element->isVisible())
                {visibleElements++;}
          }

      }

      if(definedElements!=0)
      {
        metricResult = visibleElements/definedElements;
      }

      if(metricResult >= 0){//at least iv diafram exist always
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model elements usage in diagrams coverage"<<"Model elements" <<QString::number(metricResult,'f',2)<<"-"<<"0"<<"1";
      }
      else
      {
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model elements usage in diagrams coverage" <<"Model elements"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"0"<<"1";
      }
      addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::ModelElementsDescriptionCoverage
 * This function computes the Model elements description coverage
 */
void MBSEQualityModelCheckerWindow::ModelElementsDescriptionCoverage()
{     
      float metricResult = 0.0;
      QStringList metric;
      float functionsWithComments = 0; //if i am here at least iv diagram exists.
      float numberIVFunctions = ivFunctionsNumber;

      for(ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>())
      {
           for(EntityAttribute atr :function->entityAttributes()){
               if("Comment"==atr.name())
               { QString valueComment=atr.value().toString();

                  if (!valueComment.isEmpty())
                     { functionsWithComments++;}
               }
           }
      }
      if(numberIVFunctions!=0)
      {

        metricResult = functionsWithComments/numberIVFunctions;
      }

      if(metricResult >= 0){//at least iv diafram exist always
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model elements description coverage"<<"Function" <<QString::number(metricResult,'f',2)<<"-"<<"0"<<"1";
      }
      else
      {
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model elements description coverage" <<"Function"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"0"<<"1";
      }
      addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::ModelDiagramDescriptionCoverage
 * This function computes the Model diagram description coverage
 */
void MBSEQualityModelCheckerWindow::ModelDiagramDescriptionCoverage()
{
    float metricResult = 0.0;
    QStringList metric;
    float numberIVComments = numberComments();

    if(numberIVComments != 0)
    { //number diagrams at least is 1
        metricResult = 1;
    }

    if(metricResult >= 0)
    {//at least iv diafram exist always
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model diagrams description coverage" <<"IV Diagram"<<QString::number(metricResult,'f',2)<<"1"<<"-"<<"-";
    }
    else
    {
        metric =QStringList()<<"Model coverage"<<"Completeness"<<"Model diagrams description coverage" <<"IV Diagram"<<"[METRIC COULD NOT BE COMPUTED]"<<"1"<<"-"<<"-";
    }
      addMetricResultToTable(metric);
}
/*!
 * \brief MBSEQualityModelCheckerWindow::ModelDiagramsUniqueness Metric
 * This function computes the Model Diagrams Uniqueness
 */
void MBSEQualityModelCheckerWindow::ModelDiagramsUniqueness()
{
    float metricResult = 0.0;
    QStringList metric;
    float numTotalDiagrams = 1; 
    float duplicates = 0;

    // computing dv diagrams
    if(!d->dvModelsPaths.isEmpty())
    {
        QStringList dvNames;
        for(QString singleDiagrams:d->dvModelsPaths)
        {
            QDir dir(singleDiagrams);
            dvNames<< dir.dirName();
        }

        duplicates = dvNames.removeDuplicates();
        numTotalDiagrams=numTotalDiagrams+d->dvModelsPaths.size();
    }

    // computing MSC diagrams
    if (!d->MSCDocumentsList.isEmpty())
    {
        QStringList mscNames = d->MSCDocumentsList;

        numTotalDiagrams = numTotalDiagrams+d->MSCDocumentsList.size();
        duplicates = duplicates + mscNames.removeDuplicates();
    }
    // computing SDL diagrams
    QStringList sdlDiagramsNames;
    if(sdlDiagramsPaths.size()!=0)
    { 
        for(int i=0;i<sdlDiagramsPaths.size();i++)
        { if (sdlModels[i].title!="system_structure.pr")
            { sdlDiagramsNames<<sdlModels[i].title;}
        }
        duplicates = duplicates + sdlDiagramsNames.removeDuplicates();
        numTotalDiagrams=numTotalDiagrams+sdlDiagramsPaths.size();
    }

    metricResult = 1 - (duplicates/numTotalDiagrams);
    if(metricResult > 0)
    {//at least iv diafram exist always
        metric =QStringList()<<"Model coverage"<<"Correctness"<<"Model diagrams uniqueness" <<"All Model Diagrams"<<QString::number(metricResult,'f',2)<<"1"<<"-"<<"-";
       }
    else
    {
        metric =QStringList()<<"Model coverage"<<"Correctness"<<"Model diagrams uniqueness" <<"All Model Diagrams"<<"[METRIC COULD NOT BE COMPUTED]"<<"1"<<"-"<<"-";
    }
    addMetricResultToTable(metric);
}

/*!
 * \brief MBSEQualityModelCheckerWindow::ModelDiagramsSize 
 * This function computes the Model Diagrams size
 */
void MBSEQualityModelCheckerWindow::ModelDiagramsSize()
{
    float metricResult = 1.0;
    QStringList metric;

    // computing dv diagrams
    if(!d->dvModelsPaths.isEmpty())
    {
        metricResult = metricResult + d->dvModelsPaths.size();
    }

    // computing MSC diagrams
    if (!d->MSCDocumentsList.isEmpty())
    {
        metricResult = metricResult + d->MSCDocumentsList.size();
    }

    // computing SDL diagrams
    if(sdlDiagramsPaths.size() != 0)
    {
        metricResult = metricResult + sdlDiagramsPaths.size();
    }

    if(metricResult > 0)
    {//at least iv diafram exist always
        metric =QStringList()<<"Model coverage"<<"Size"<<"Model diagrams size"<<"All Model Diagrams" <<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    {
        metric =QStringList()<<"Model coverage"<<"Size"<<"Model diagrams size"<<"All Model Diagrams"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
     }
    addMetricResultToTable(metric);
}
/*!
 * \brief MBSEQualityModelCheckerWindow::ModelElementDiagramSize
 * This function computes the Model Elements Diagrams size
 */
void MBSEQualityModelCheckerWindow::ModelElemntDiagramSize()
{
    float metricResult = 1.0;
    QStringList metric;


    if(0<numberFunctions())         {metricResult++;}
    if(0<numberProvidedInterfaces()){metricResult++;}
    if(0<numberRequiredInterfaces()){metricResult++;}
    if(0<numberFunctionType())      {metricResult++;}
    if(0<numberInterfaceGroups())   {metricResult++;}
    if(0<numberComments())          {metricResult++;}
    if(0<numberConnections())       {metricResult++;}
    if(0<numberConnectionLayers())  {metricResult++;}

    if( dvModelClassified!= nullptr)
    {
       if( 0!=dvModelClassified->partitions.size()){metricResult++;}
       if( 0!= dvModelClassified->functions.size()){metricResult++;}
       if( 0!= dvModelClassified->nodes.size()){metricResult++;}
       if( 0!= dvModelClassified->messages.size()){metricResult++;}
       if( 0!= dvModelClassified->connections.size()){metricResult++;}
       if( 0!= dvModelClassified->devices.size()){metricResult++;}
       if( 0!= dvModelClassified->ports.size()){metricResult++;}
       if( 0!= dvModelClassified->systemFunctions .size()){metricResult++;}
       if( 0!= dvModelClassified->systemInterfaces.size()){metricResult++;}
    }

    if(metricResult > 0)
    {//at least iv diafram exist always
        metric =QStringList()<<"Model coverage"<<"Size"<<"Model diagrams elements size"<<"IV and DV Diagrams" <<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    {
        metric =QStringList()<<"Model coverage"<<"Size"<<"Model diagrams elements size"<<"IV and DV Diagrams"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
     }
    addMetricResultToTable(metric);
}
/*!
 * \brief MBSEQualityModelCheckerWindow::ActiveInterfaceSize
 * This function computes the Number of Active Interfaces
 */
void MBSEQualityModelCheckerWindow::ActiveInterfaceSize()
{
    float metricResult = 0.0;
    QStringList metric;
    for (int i=0; i<ivFunctionsNumber; i++)
    {
        if(functionsClassified[i].decomposed==false)
        {metricResult=metricResult+ functionsClassified[i].pi_activeInterfaces;}
    }

    if(metricResult >= 0)
    {
        metric =QStringList()<<"Functionality"<<"Size"<<"Active interface size" <<"IV Diagrams"<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
    }
    else
    {
        metric =QStringList()<<"Functionality"<<"Size"<<"Active interface size"<<"IV Diagrams"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
     }
    addMetricResultToTable(metric);

}
/*!
 * \brief MBSEQualityModelCheckerWindow::MutuallyExclusiveInterfaceSize
 * This function computes the Mutually Exclusive Interface Size of each function
 */
void MBSEQualityModelCheckerWindow::MutuallyExclusiveInterfaceSize()
{
    QStringList metric ;
    for (int i=0; i<ivFunctionsNumber; i++)
    {
        if (functionsClassified[i].decomposed==false){
            metric =QStringList()<<"Functionality"<<"Size"<<"Mutually exclusive interface size" <<"Function: "+functionsClassified[i].functionName  <<"Active:"+ QString::number(functionsClassified[i].pi_activeInterfaces)+" Non-Active:"+ QString::number(functionsClassified[i].pi_protectedInterfaces)<<"-"<<"-"<<"-";
            addMetricResultToTable(metric);
        }
    }

    if(ivFunctionsNumber<=0){metric =QStringList()<<"Functionality"<<"Size"<<"Mutually exclusive interface size" <<"Functions not defined"<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
        addMetricResultToTable(metric);}
}

void MBSEQualityModelCheckerWindow::StatesSize()
{    float metricResult;

     for(int i=0; i<sdlDiagramsPaths.size(); i++)
     {



            QString aux= sdlModels[i].title;
            aux.remove(".pr",Qt::CaseSensitive);
            for (int j=0;j<ivFunctionsNumber;j++)
            {if(aux.contains(functionsClassified[j].functionName, Qt::CaseInsensitive))
                {
                  j=ivFunctionsNumber;
                  metricResult = sdlModels[i].numberStates;
                  QStringList metric;
                  if(metricResult >= 0)
                  {
                     metric =QStringList()<<"Functionallity"<<"Size"<<"States size" <<"Behavioural Diagram: "+sdlModels[i].title<<QString::number(metricResult,'f',2)<<"-"<<"-"<<"-";
                   }
                  else
                  {
                     metric =QStringList()<<"Functionallity"<<"Size"<<"States size" <<"Behavioural Diagram: "+sdlModels[i].title<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-"<<"-";
                  }
                  addMetricResultToTable(metric);
                }
            }
      }

}
void MBSEQualityModelCheckerWindow::InterfaceDistributionHomogeneity()
{   float metricResult;
    QStringList metric;
    ushort interfaceNumberList [ivFunctionsNumber];
    ushort aux=0;
    int leafFucntionsNumber=0;
    float median=0;
// store the interface numbers in an array to classify and extract median value
    for (int i=0; i<ivFunctionsNumber; i++)
    { if(functionsClassified[i].decomposed==false)
        {interfaceNumberList[leafFucntionsNumber]=functionsClassified[i].allInterfaces;
         leafFucntionsNumber++;}
    }

    for(int i=0; i<leafFucntionsNumber; i++)
    {for(int j=0; j<leafFucntionsNumber; j++)
        {if (interfaceNumberList[j]>interfaceNumberList[j+1])
            {aux=interfaceNumberList[j+1];
                interfaceNumberList[j+1]=interfaceNumberList[j];
                interfaceNumberList[j]=aux;
            }
        }
    }
    if((leafFucntionsNumber%2)==0){median= (interfaceNumberList[leafFucntionsNumber/2]+interfaceNumberList[(leafFucntionsNumber/2)+1])/2;}
    else {
        median=interfaceNumberList[leafFucntionsNumber/2];}

    for (int i=0; i<ivFunctionsNumber; i++)
    { if (functionsClassified[i].decomposed==false)
        {
        if (median>=0)
        {   metricResult=(functionsClassified[i].allInterfaces-median)/median;
            metric =QStringList()<<"Maintainability"<<"Complexity"<<"Interface distribution homogeneity"<<"Function: "+ functionsClassified[i].functionName <<QString::number(metricResult,'f',2)<<"-"<<"-0.5"<<"0.5";
        }
        else
        {
            metric =QStringList()<<"Maintainability"<<"Complexity"<<"Interface distribution homogeneity"<<"Function: "+ functionsClassified[i].functionName<<"[METRIC COULD NOT BE COMPUTED]"<<"-"<<"-0.5"<<"0.5";

        }
        addMetricResultToTable(metric);
        }
    }
}
}// close IVE namespace
