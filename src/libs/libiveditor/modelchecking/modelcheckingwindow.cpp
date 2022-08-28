/*
   Copyright (C) 2021-2022 GMV - <tiago.jorge@gmv.com>

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

#include "modelcheckingwindow.h"
#include "ui_modelcheckingwindow.h"
#include "ivfunction.h"
#include "interfacedocument.h"
#include "ivmodel.h"
#include "xmelwriter.h"
#include "xmelreader.h"
#include "xmelreader.h"

#include <QDir>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QMenu>
#include <QDirIterator>
#include <QFileDialog>
#include <QDebug>

namespace ive {

struct ModelCheckingWindow::ModelCheckingWindowPrivate {
    Ui::ModelCheckingWindow *ui { new Ui::ModelCheckingWindow };
    InterfaceDocument *document { nullptr };   
};

/*!
 * \brief ModelCheckingWindow::ModelCheckingWindow Initializes the MC window, performing initial checks, window setup configuration and window population with the model elements.
 */
ModelCheckingWindow::ModelCheckingWindow(InterfaceDocument *document, const QString projectDir, QWidget *parent) :
    QMainWindow(parent)
    , d(new ModelCheckingWindowPrivate)
{
    // Set ui and interface document
    d->ui->setupUi(this);
    d->document = document;

    // Set paths for project, properties, subtypes and results/oputputs
    this->projectDir = projectDir;
    this->propertiesPath =  projectDir + "/work/modelchecking/properties";
    this->subtypesPath =  projectDir + "/work/modelchecking/subtypes";
    this->configurationsPath =  projectDir + "/work/modelchecking/configurations";
    this->outputPath = projectDir + "/work/build/modelchecking/output";

    // CHECK project dir structure, create directories where necessary
    // check if properties dir exists and create it otherwise
    if (!QDir(this->propertiesPath).exists()){
        QDir().mkdir(this->propertiesPath);
    }
    // check if subtypes dir exists and create it otherwise
    if (!QDir(this->subtypesPath).exists()){
        QDir().mkdir(this->subtypesPath);
    }
    // check if configurations dir exists
    Q_ASSERT(QDir(this->configurationsPath).exists());
    // check if output dir exists
    Q_ASSERT(QDir(this->outputPath).exists());

    // Define right-click menus and set menu policy
    this->contextMenuPropertiesTop = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuProperties = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuPropertiesMSCFile = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuPropertiesFile = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuSubtypes = new QMenu(d->ui->treeWidget_subtyping);
    this->contextMenuSubtypesFile = new QMenu(d->ui->treeWidget_subtyping);
    d->ui->treeWidget_properties->setContextMenuPolicy(Qt::CustomContextMenu);
    d->ui->treeWidget_subtyping->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *newProperty = this->contextMenuPropertiesTop->addAction("New property");
    QAction *deletePropertyDir = this->contextMenuProperties->addAction("Delete property folder");
    QAction *MSC2OBS = this->contextMenuPropertiesMSCFile->addAction("Convert MSC to Observer");
    QAction *deleteMSC = this->contextMenuPropertiesMSCFile->addAction("Delete property file");
    QAction *deletePropertyFile = this->contextMenuPropertiesFile->addAction("Delete property file");
    QAction *newSubtypes = this->contextMenuSubtypes->addAction("New subtypes");
    QAction *deleteSubtypes = this->contextMenuSubtypesFile->addAction("Delete subtypes");
    connect(newProperty, SIGNAL(triggered()), this, SLOT(addProperty()));
    connect(deletePropertyDir, SIGNAL(triggered()), this, SLOT(deleteProperty()));
    connect(MSC2OBS, SIGNAL(triggered()), this, SLOT(convertToObs()));
    connect(deleteMSC, SIGNAL(triggered()), this, SLOT(deleteProperty()));
    connect(deletePropertyFile, SIGNAL(triggered()), this, SLOT(deleteProperty()));
    connect(newSubtypes, SIGNAL(triggered()), this, SLOT(addSubtypes()));
    connect(deleteSubtypes, SIGNAL(triggered()), this, SLOT(deleteSubtypes()));

    // Make tree views show horizontal scroll bars
    d->ui->treeWidget_properties->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->treeWidget_subtyping->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->treeWidget_submodel->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->treeWidget_results->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Build properties tree view
    QFileInfo propertiesFileInfo(this->propertiesPath);
    QStringList fileColumnProps;
    fileColumnProps.append(propertiesFileInfo.fileName());
    this->propertiesTopDirWidgetItem = new QTreeWidgetItem(fileColumnProps);
    this->propertiesTopDirWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    d->ui->treeWidget_properties->addTopLevelItem(this->propertiesTopDirWidgetItem);
    this->propertiesTopDirWidgetItem->setCheckState(0, listProperties(this->propertiesTopDirWidgetItem, propertiesFileInfo, {}, {}, 0));

    // Build subtyping tree view
    QFileInfo subtypesFileInfo(this->subtypesPath);
    QStringList fileColumnSubtypes;
    fileColumnSubtypes.append(subtypesFileInfo.fileName());
    subtypesTopDirWidgetItem = new QTreeWidgetItem(fileColumnSubtypes);
    subtypesTopDirWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    d->ui->treeWidget_subtyping->addTopLevelItem(subtypesTopDirWidgetItem);
    listSubtypes(subtypesTopDirWidgetItem, subtypesFileInfo, {});

    // Build submodel tree view
    QStringList functionColumnSubmodel;
    functionColumnSubmodel.append("Model Functions");
    functionsTopNodeWidgetItem = new QTreeWidgetItem(functionColumnSubmodel);
    functionsTopNodeWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_FileDialogListView));
    d->ui->treeWidget_submodel->addTopLevelItem(functionsTopNodeWidgetItem);
    listModelFunctions(functionsTopNodeWidgetItem, {});

    // Build results tree view
    QFileInfo resultsFileInfo(this->outputPath);
    QStringList fileColumnResuls;
    fileColumnResuls.append(resultsFileInfo.fileName());
    resultsTopDirWidgetItem = new QTreeWidgetItem(fileColumnResuls);
    resultsTopDirWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    d->ui->treeWidget_results->addTopLevelItem(resultsTopDirWidgetItem);
    listResults(resultsTopDirWidgetItem, resultsFileInfo);

    // Set validators on MC options value fileds
    d->ui->lineEdit_maxNumEnvRICalls->setValidator( new QIntValidator(0, 1000000, this) );
    d->ui->lineEdit_maxNumScenarios->setValidator( new QIntValidator(0, 1000, this) );
    d->ui->lineEdit_maxNumStates->setValidator( new QIntValidator(0, 10000000, this) );
    d->ui->lineEdit_timeLimit->setValidator( new QIntValidator(0, 100000, this) );

    // set status bar text color
    statusBar()->setStyleSheet("color: white");

    statusBar()->showMessage("Window started.", 6000);
}

ModelCheckingWindow::~ModelCheckingWindow()
{
    delete d;
    d = nullptr;
}

/*!
 * \brief ModelCheckingWindow::callTasteGens Calls any needed TASTE generators.
 */
void ModelCheckingWindow::callTasteGens(bool toggled){
    if (!toggled) {return;}

    // CALL MAKE SKELETONS
    QString makeSkeletonsCall = "make skeletons";
    QProcess *makeSleletonsCallerProcess = new QProcess(this);
    // set path to project dir
    QString qDirAppPath = QDir::currentPath();
    QDir::setCurrent(this->projectDir+"/");
    if (makeSleletonsCallerProcess->execute(makeSkeletonsCall) != 0) {
        QMessageBox::warning(this, tr("Make skeletons call"),
                             "Error when making skeletons!");
    }
    // reset path
    QDir::setCurrent(qDirAppPath);

    // CALL MAKE DEPLOYMENT
    QString makeDeploymentCall = "make DeploymentView.aadl";
    QProcess *makeDeploymentCallerProcess = new QProcess(this);
    // set path to project dir
    qDirAppPath = QDir::currentPath();
    QDir::setCurrent(this->projectDir+"/");
    if (makeDeploymentCallerProcess->execute(makeDeploymentCall) != 0) {
        QMessageBox::warning(this, tr("Make deployment call"),
                             "Error when making deployment view!");
    }
    // reset path
    QDir::setCurrent(qDirAppPath);

    // CALL KAZOO
    QString kazooCall = "kazoo -gw --glue -t MOCHECK";
    QProcess *kazooCallerProcess = new QProcess(this);
    // set path to project dir
    qDirAppPath = QDir::currentPath();
    QDir::setCurrent(this->projectDir+"/");
    if (kazooCallerProcess->execute(kazooCall) != 0) {
        QMessageBox::warning(this, tr("Kazoo call"),
                             "Error when calling kazoo!");
    }
    // reset path
    QDir::setCurrent(qDirAppPath);
}

/*!
 * \brief ModelCheckingWindow::listProperties Recursive function creating a tree of QTreeWidgetItem reflecting the properties directory, with memory for node (property directory or property) expansion and selection states.
 * Up to 1 level hierarchy supported.
 * \return The recursion subtree top node selection state.
 */
Qt::CheckState ModelCheckingWindow::listProperties(QTreeWidgetItem *parentWidgetItem, QFileInfo &parentFileInfo, QStringList preSelection, QStringList expanded, int recLevel) {
    QDir dir;
    dir.setPath(parentFileInfo.filePath());
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst | QDir::Name);
    Qt::CheckState checkState;
    bool parentIsFullyChecked = true;
    bool parentIsUnchecked = true;

    const QFileInfoList fileList = dir.entryInfoList();

    for (int i = 0; i < fileList.size(); i++) {
        QFileInfo childFileInfo = fileList.at(i);
        QStringList childWidgetInfo;
        childWidgetInfo.append(childFileInfo.fileName());
        if (childFileInfo.fileName() == "." || childFileInfo.fileName() == ".." ); // nothing
        else if(childFileInfo.isDir()) { // is directory
            if(recLevel == 0){ // constrain to 1 level hierarchy
                QTreeWidgetItem *childWidgetItem = new QTreeWidgetItem(childWidgetInfo);
                childWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
                parentWidgetItem->addChild(childWidgetItem);
                checkState = listProperties(childWidgetItem, childFileInfo, preSelection, expanded, recLevel + 1); // constrain to 1 level hierarchy
                childWidgetItem->setCheckState(0, checkState);
                if (checkState == Qt::Unchecked){parentIsFullyChecked = false;}
                if (checkState == Qt::Checked || checkState == Qt::PartiallyChecked){parentIsUnchecked = false;}
            }
        }
        else { // is file
            if (childFileInfo.suffix() == "msc" || childFileInfo.suffix() == "pr"){
                childWidgetInfo.append(childFileInfo.filePath());
                QTreeWidgetItem *childWidgetItem = new QTreeWidgetItem(childWidgetInfo);
                if (childFileInfo.suffix() == "msc") {
                    childWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_MediaPlay));
                } else {
                    childWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_FileDialogContentsView));
                }
                checkState = getCheckState(preSelection, childFileInfo.filePath());
                childWidgetItem->setCheckState(0, checkState);
                parentWidgetItem->addChild(childWidgetItem);
                if (checkState == Qt::Unchecked){parentIsFullyChecked = false;}
                if (checkState == Qt::Checked || checkState == Qt::PartiallyChecked){parentIsUnchecked = false;}
            }
        }
    }

    parentWidgetItem->setExpanded(isExpanded(expanded, parentWidgetItem->text(0)));

    if (parentIsFullyChecked && !parentIsUnchecked){return Qt::Checked;}
    if (!parentIsFullyChecked && parentIsUnchecked){return Qt::Unchecked;}
    // empty dir
    if (parentIsFullyChecked && parentIsUnchecked){return Qt::Unchecked;}
    return Qt::PartiallyChecked;

}

/*!
 * \brief ModelCheckingWindow::isExpanded Checks if node (property directory) corresponding to 'dirName' is in the list 'expanded' of expanded (property directory) nodes.
 * \return 'true' if node is in the list (i.e. was expanded); 'false' otherwise.
 */
bool ModelCheckingWindow::isExpanded(QStringList expanded, QString dirName){
    for (QString expand : expanded){
        if (expand == dirName) {return true;}
    }
    return false;
}

/*!
 * \brief ModelCheckingWindow::getCheckState Checks if node corresponding to 'stringId' is in the list 'selections' of selected nodes.
 * \return 'Qt::Checked' if node is in the list (i.e. was selected); 'Qt::Unchecked' otherwise.
 */
Qt::CheckState ModelCheckingWindow::getCheckState(QStringList selections, QString stringId){
    for (QString selection : selections){
        if (stringId.contains(selection, Qt::CaseSensitive)) {return Qt::Checked;}
    }
    return Qt::Unchecked;
}

/*!
 * \brief ModelCheckingWindow::listSubtypes Function creating a tree of QTreeWidgetItem reflecting the subtypes directory, with memory for node (subtype file) selection state.
 */
void ModelCheckingWindow::listSubtypes(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent, QStringList preSelection) {
    QDir dir;
    dir.setPath(parent.filePath());
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst | QDir::Name);

    const QFileInfoList fileList = dir.entryInfoList();

    for (int i = 0; i < fileList.size(); i++) {
        QFileInfo childFileInfo = fileList.at(i);
        QStringList fileColumn;
        fileColumn.append(childFileInfo.fileName());
        if (childFileInfo.fileName() == "." || childFileInfo.fileName() == ".." ); // nothing
        else if(childFileInfo.isDir()) { // is directory
            continue;
        }
        else { // is file
                if (childFileInfo.suffix() == "asn"){
                    fileColumn.append(childFileInfo.filePath());
                    QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
                    child->setIcon(0, this->style()->standardIcon(QStyle::SP_TitleBarNormalButton));
                    child->setCheckState(0, getCheckState(preSelection, childFileInfo.filePath()));
                    parentWidgetItem->addChild(child);
                }
        }
    }
}

/*!
 * \brief ModelCheckingWindow::listModelFunctions Function creating a tree of QTreeWidgetItem reflecting the model Functions, with memory for node (Function) selection state.
 */
void ModelCheckingWindow::listModelFunctions(QTreeWidgetItem *parentWidgetItem, QStringList preSelection) {
    for (ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>()) {
        if(!function->hasNestedChildren()){
            QStringList functionColumn;
            functionColumn.append(function->title());
            QTreeWidgetItem *functionNode = new QTreeWidgetItem(functionColumn);
            functionNode->setIcon(0, this->style()->standardIcon(QStyle::SP_MediaStop));
            if (preSelection.size() == 0) { // cannot be empty, only if in first tree build so preselect all nodes in that case
                functionNode->setCheckState(0, Qt::Checked);
            } else {
                functionNode->setCheckState(0, getCheckState(preSelection, function->title()));
            }
            parentWidgetItem->addChild(functionNode);
        }
    }
}

/*!
 * \brief ModelCheckingWindow::listModelFunctions Function creating a tree of QTreeWidgetItem reflecting the results/output directory.
 */
void ModelCheckingWindow::listResults(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent) {
    QDir dir;
    dir.setPath(parent.filePath());
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst | QDir::Name);

    const QFileInfoList fileList = dir.entryInfoList();

    for (int i = 0; i < fileList.size(); i++) {
        QFileInfo fileInfo = fileList.at(i);
        QStringList fileColumn;
        fileColumn.append(fileInfo.fileName());
        if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." ); // nothing
        else if(fileInfo.isDir()) { // is directory
            continue;
        }
        else { // is file
                if (fileInfo.suffix() == "msc"){
                    fileColumn.append(fileInfo.filePath());
                    QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
                    child->setIcon(0, this->style()->standardIcon(QStyle::SP_MediaPlay));
                    parentWidgetItem->addChild(child);
                }
                if (fileInfo.suffix() == "scn"){
                    fileColumn.append(fileInfo.filePath());
                    QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
                    child->setIcon(0, this->style()->standardIcon(QStyle::SP_FileIcon));
                    parentWidgetItem->addChild(child);
                }
        }
    }
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_properties_itemChanged Checks or unchecks child tree nodes as per parent resulting check state upon change.
 */
void ModelCheckingWindow::on_treeWidget_properties_itemChanged(QTreeWidgetItem *item, int column)
{
    if (item ==nullptr)
    {
        return;
    }

    if (item->checkState(0) == Qt::Checked)
    {

        int cnt = item->childCount();
        if (cnt >0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Checked);
            }
        }
        updateParentItem(item);
    }
    else if (item->checkState(0) == Qt::Unchecked)
    {
        int cnt = item->childCount();
        if (cnt > 0)
        {
            for (int i = 0;i < cnt;i++)
            {
                item->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
        updateParentItem(item);
    }
}

/*!
 * \brief ModelCheckingWindow::updateParentItem Updates parent tree node check state as per children check states upon child 'item' check state change.
 */
void ModelCheckingWindow::updateParentItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (parent == nullptr)
    {
        return;
    }

    int selectedCount = 0;
    int childCount = parent->childCount();
    for (int i = 0; i < childCount; i++)
    {
        QTreeWidgetItem *childItem = parent->child(i);
        if (childItem->checkState(0) == Qt::Checked)
        {
            selectedCount++;
        }
    }
    if (selectedCount <= 0)
    {
        parent->setCheckState(0, Qt::Unchecked);
    }
    else if (selectedCount > 0 && selectedCount < childCount)
    {
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    else if (selectedCount == childCount)
    {
        parent->setCheckState(0, Qt::Checked);
    }
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_properties_itemDoubleClicked Opens property file in respective editor, upon file node double click.
 */
void ModelCheckingWindow::on_treeWidget_properties_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(1) == "") { // is directory node
        return;
    }
    // is property file
    QFileInfo fileInfo(item->text(1)); // text(1) of file hosts the file absolute path
    QString cmd;
    if (fileInfo.completeSuffix() == "msc"){
        cmd = "spacecreator.AppImage -client " + item->text(1);
    } else{ // then has to be a .pr file
        cmd = "opengeode " + item->text(1);
    }
    QProcess *p = new QProcess();
    p->start(cmd);
    if(!p->waitForStarted(10000)) {
        QMessageBox::warning(this, tr("Open property"),
                             tr("Error when calling '%1'.").arg(cmd));
        return;
    }
    statusBar()->showMessage("File open.", 6000);
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_subtyping_itemDoubleClicked Opens subtypes file in respective editor, upon file node double click.
 */
void ModelCheckingWindow::on_treeWidget_subtyping_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(1) == "") { // is dir node
        return;
    }
    // is subtyping file
    QString cmd = "spacecreator.AppImage -client " + item->text(1);
    QProcess *p = new QProcess();
    p->start(cmd);
    if(!p->waitForStarted(10000)) {
        QMessageBox::warning(this, tr("Open subtyping"),
                             tr("Error when calling '%1'.").arg(cmd));
        return;
    }
    statusBar()->showMessage("File open.", 6000);
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_subtyping_itemChanged Ensures only one subtyping file if selected, upon new selection.
 */
void ModelCheckingWindow::on_treeWidget_subtyping_itemChanged(QTreeWidgetItem *item, int column)
{
    if (item ==nullptr)
    {
        return;
    }

    QTreeWidgetItem *parent = item->parent();
    if (parent == nullptr)
    {
        return;
    }

    int childCount = parent->childCount();
    QTreeWidgetItem *childItem;

    if (item->checkState(0) == Qt::Checked)
    {
        for (int i = 0; i < childCount; i++)
        {
            childItem = parent->child(i);
            if (childItem != item)
            {
                childItem->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}

/*!
 * \brief ModelCheckingWindow::on_pushButton_interactiveSim_clicked Calls the interactive simulator
 */
void ModelCheckingWindow::on_pushButton_interactiveSim_clicked()
{
    // First save current configuration to hidden file .mcconfig.xml
    if (!saveConfiguration()) {
        return;
    }

    // build and run the interactive simulator
    QString callSimCmd = "xterm -e '/usr/bin/make simu; read -p 'Press Enter to close''";
    if (QProcess::execute(callSimCmd) != 0) {
        QMessageBox::warning(this, tr("Interactive Simulator"),
                             "Error executing: " + callSimCmd);
    }
}

/*!
 * \brief ModelCheckingWindow::on_pushButton_exhaustiveSim_clicked Calls the exhaustive simulator
 */
void ModelCheckingWindow::on_pushButton_exhaustiveSim_clicked()
{
    // First save current configuration to hidden file .mcconfig.xml
    if (!saveConfiguration()) {
        return;
    }

    // build and run the exhaustive simulator
    QString callSimCmd = "xterm -e '/usr/bin/make native_modelchecker; read -p 'Press Enter to close''";
    if (QProcess::execute(callSimCmd) != 0) {
        QMessageBox::warning(this, tr("Exhaustive Simulator"),
                             "Error executing: " + callSimCmd);
    }
}

/*!
 * \brief ModelCheckingWindow::on_pushButton_callIF_clicked Calls the model checker engine (IF), performing necessary operations before (e.g. save current configuration) and after (e.g. results translation) the call.
 */
void ModelCheckingWindow::on_pushButton_callIF_clicked()
{
    // CONFIRM MC call with user
    int ret = QMessageBox::warning(this, tr("Call IF"),
                                   "Do you confirm you want to call IF? Default output folder will be rebuilt!",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    // First save current configuration to hidden file .mcconfig.xml
    if (!saveConfiguration()) {
        return;
    }

    // JUMP to project directory
    QString qDirAppPath = QDir::currentPath();
    QDir::setCurrent(this->projectDir+"/");

    // REMOVE statusfile, callif.sh
    QString rmFilesCmd = "rm -f statusfile callif.sh";
    if (QProcess::execute(rmFilesCmd) != 0) {
        QMessageBox::warning(this, tr("Call IF"),
                             "Error executing: " + rmFilesCmd);
    }

    // REMOVE output dir, if existing
    if(QDir(this->outputPath).exists()){
        QDir(this->outputPath).removeRecursively();
    }

    // DESTROY tree except root
    QTreeWidgetItem *treeRoot = this->resultsTopDirWidgetItem;
    for (int i = treeRoot->childCount(); i > 0; i--){
        treeRoot->removeChild(treeRoot->child(i-1));
    }

    // CREATE callif.sh
    QFile callIfFile("callif.sh");
    if(callIfFile.open(QIODevice::ReadWrite)){
        QTextStream stream(&callIfFile);
        stream << "#!/bin/bash" << endl;
        stream << "make clean" << endl;
        stream << "make model-check" << endl;
        stream << "echo $? > statusfile" << endl;
    } else {
        QMessageBox::warning(this, tr("Call IF"),
                             "Error opening callif.sh!");
    }

    // CALL IF make rule via terminal, saving make return in statusfile
    QString callIFCmd = "xterm -hold -e sh callif.sh";
    if (QProcess::execute(callIFCmd) != 0) {
        QMessageBox::warning(this, tr("Call IF"),
                             "Error executing: " + callIFCmd);
    }

    // READ statusfile with make return value
    QByteArray makeStatus;
    int makeReturn;
    QFile statusFile("statusfile");
    if (statusFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        makeStatus = statusFile.readAll();
        makeReturn = QString::compare(QString(makeStatus.at(0)), "0");
        if (makeReturn != 0) {
            QMessageBox::warning(this, tr("Call IF"),
                                 "Error executing: make model-check - make return: " + QString::number(makeReturn));
        }
    } else {
        QMessageBox::warning(this, tr("Call IF"),
                             "Error opening status file!");
    }

    // CHECK output dir exists
    if(!QDir(this->outputPath).exists()){
        QMessageBox::warning(this, tr("Call IF"),
                             "Default output folder was not generated!");
        // reset path and return
        QDir::setCurrent(qDirAppPath);
        return;
    }

    // CONVERT IF scenarios (scn files) into msc files
    if (QDir(this->outputPath).isEmpty()){
        QMessageBox::information(this, tr("Call IF"),
                             "No scenarios present in default output folder! ");
        // reset path and return
        QDir::setCurrent(qDirAppPath);
        return;
    }

    // Output folder was generated and it is not empty
    statusBar()->showMessage("Scenarios were found!", 6000);

    // ask user to save scenarios
    ret = QMessageBox::warning(this, tr("Call IF"),
                                   "Do you want to copy somewhere the generated scenarios?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QString saveDirectoryName =
                QFileDialog::getExistingDirectory(this, tr("Select Directory (no whitespaces)"),
                                             this->configurationsPath,
                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        while(!saveDirectoryName.isEmpty() && saveDirectoryName.contains(" ")){
            QMessageBox::warning(this, tr("Call IF"),
                                 "Please pick a directory name with no whitespaces.");
            saveDirectoryName =
                                QFileDialog::getExistingDirectory(this, tr("Select Directory (no whitespaces)"),
                                                             this->configurationsPath,
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        }
        if(!saveDirectoryName.isEmpty()){
            // copy output directory
            if (QProcess::execute("cp -r . " + saveDirectoryName) != 0) {
                QMessageBox::warning(this, tr("Call IF"),
                                     "Error copying output folder to: " + saveDirectoryName);
            }
        }
    }

    // reset path
    QDir::setCurrent(qDirAppPath);

    //Build results/output tree view
    QFileInfo resultsFileInfo(this->outputPath);
    listResults(resultsTopDirWidgetItem, resultsFileInfo);
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_results_itemDoubleClicked Opens output files in respective editor, upon file node double click.
 */
void ModelCheckingWindow::on_treeWidget_results_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(1) == "") {return;}
    QFileInfo fileInfo(item->text(1));
    QString cmd;
    if (fileInfo.completeSuffix() == "msc"){
        cmd = "spacecreator.AppImage -client " + item->text(1);
    } else{
        cmd = "kate " + item->text(1);
    }
    QProcess *p = new QProcess();
    p->start(cmd);
    if(!p->waitForStarted(10000)) {
        QMessageBox::warning(this, tr("Open scenario"),
                             tr("Error when calling '%1'.").arg(cmd));
        return;
    }
    statusBar()->showMessage("File open.", 6000);
}

/*!
 * \brief ModelCheckingWindow::convertToObs Converts an .msc property file into a semantically equivalent observer property file, and then rebuilds the properties tree keeping node selection and expansion.
 */
void ModelCheckingWindow::convertToObs()
{
    // Get msc property name
    QFileInfo fileInfo(d->ui->treeWidget_properties->currentItem()->text(0));
    QString fileName = fileInfo.baseName()+".pr";
    QString propDirName = d->ui->treeWidget_properties->currentItem()->parent()->text(0);

    // CALL MAKE RULE
    // set path to property dir
    QString qDirAppPath = QDir::currentPath();
    QDir::setCurrent(this->projectDir+"/work/modelchecking/properties/" + propDirName);
    if (QProcess::execute("make " + fileName) != 0) {
        QMessageBox::warning(this, tr("Convert to Observer"),
                             "Error when calling make rule!");
        // reset path
        QDir::setCurrent(qDirAppPath);
        return;
    }
    // reset path
    QDir::setCurrent(qDirAppPath);

    // REFRESH TREEVIEW with preselection
    QFileInfo propertiesFileInfo(this->propertiesPath);
    // save user selection
    QStringList preSelection = getPropertiesSelection(this->propertiesTopDirWidgetItem, {});
    // save user expanded nodes
    QStringList expandedNodes = getExpandedNodes(this->propertiesTopDirWidgetItem, {this->propertiesTopDirWidgetItem->text(0)});
    // destroy tree except root
    QTreeWidgetItem *treeRoot = this->propertiesTopDirWidgetItem;
    for (int i = treeRoot->childCount(); i > 0; i--){
        treeRoot->removeChild(treeRoot->child(i-1));
    }
    // rebuild tree with saved selection
    this->propertiesTopDirWidgetItem->setCheckState(0, listProperties(this->propertiesTopDirWidgetItem, propertiesFileInfo, preSelection, expandedNodes, 0));

    statusBar()->showMessage("Observer generated.", 6000);
}

/*!
 * \brief ModelCheckingWindow::addProperty Presents the user with dialogs to select a property type and provide its name, creates the property file and then rebuilds the properties tree keeping node selection and expansion.
 */
void ModelCheckingWindow::addProperty()
{
    bool ok1;
    QString propertyType = "";
    QStringList propertyTypes = { "Boolean Stop Condition - LTL", "Boolean Stop Condition - Observer", "Message Sequence Chart Search/Verify", "Observer" };
    propertyType = QInputDialog::getItem(this, tr("New Property"), tr("Property type:"), propertyTypes, 0, false, &ok1);

    if (ok1 && !propertyType.isEmpty()){
        QString makeRule = "";
        if(propertyType == "Observer") {makeRule = "create-obs";}
        if(propertyType == "Message Sequence Chart Search/Verify") {makeRule = "create-msc";}
        if(propertyType == "Boolean Stop Condition - Observer") {makeRule = "create-bsc";}
        if(propertyType == "Boolean Stop Condition - LTL") {
            QMessageBox::warning(this, tr("Add property"),
                                 "LTL not yet supported.");
            return;
        }
        bool ok2;
        QString label = "Property name: (no whitespace or '-')                                                                             ";
        QString propertyName = QInputDialog::getText(this, "New " + propertyType,
                                                label, QLineEdit::Normal,
                                                "new_property", &ok2);

        if (ok2 && !propertyName.isEmpty()){
            // CHECK FILE NAME
            // use "_" instead of whitespace or '-', if existing
            propertyName.replace(" ", "_");
            propertyName.replace("-", "_");
            // check if file with same name exists already, append suffix in that case
            QFile file;
            QString filePath = this->propertiesPath + "/" + propertyName;
            file.setFileName(filePath);
            while(file.exists())
            {
                filePath = filePath + "_1";
                propertyName = propertyName + "_1";
                file.setFileName(filePath);
            }
            // CALL MAKE RULE
            QString makeCall = "make " + makeRule + " NAME=" + propertyName;
            QProcess *makeCallerProcess = new QProcess(this);
            // set path to project dir
            QString qDirAppPath = QDir::currentPath();
            QDir::setCurrent(this->projectDir+"/");
            if (makeCallerProcess->execute(makeCall)) {
                QMessageBox::warning(this, tr("Add property"),
                                     tr("Error executing '%1'").arg(makeCall));
            }

            // REFRESH TREEVIEW with preselection
            QFileInfo propertiesFileInfo(this->propertiesPath);
            // save user selection
            QStringList preSelection = getPropertiesSelection(this->propertiesTopDirWidgetItem, {});
            // save user expanded nodes
            QStringList expandedNodes = getExpandedNodes(this->propertiesTopDirWidgetItem, {this->propertiesTopDirWidgetItem->text(0)});
            // destroy tree except root
            QTreeWidgetItem *treeRoot = this->propertiesTopDirWidgetItem;
            for (int i = treeRoot->childCount(); i > 0; i--){
                treeRoot->removeChild(treeRoot->child(i-1));
            }
            // rebuild tree with saved selection
            this->propertiesTopDirWidgetItem->setCheckState(0, listProperties(this->propertiesTopDirWidgetItem, propertiesFileInfo, preSelection, expandedNodes, 0));

            if (QDir(filePath).exists() && !QDir(filePath).isEmpty()){
                statusBar()->showMessage("Property " + propertyName + " added.", 6000);
            }

            // reset path
            QDir::setCurrent(qDirAppPath);
        }
    }
}

/*!
 * \brief ModelCheckingWindow::getPropertiesSelection Saves the properties that are selected ('Qt::Checked') under a tree node.
 * \return The list 'selections' with the properties selected, each including its parent name.
*/
QStringList ModelCheckingWindow::getPropertiesSelection(QTreeWidgetItem *propertyWidgetItem, QStringList selections){
    for (int i = 0; i < propertyWidgetItem->childCount(); i++)
    {
        QTreeWidgetItem *child = propertyWidgetItem->child(i);
        if (child->childCount() == 0 && child->text(1) != ""){ // is property file
            if (child->checkState(0) == Qt::Checked){ // and is checked
                selections.append(child->parent()->text(0) + "/" + child->text(0));
            }
        } else if (child->childCount() > 0) { // is non empty dir
            selections = selections + getPropertiesSelection(child, {});
        }
    }

    return selections;
}

/*!
 * \brief ModelCheckingWindow::getExpandedNodes Saves the property directories that are expanded under a tree node.
 * \return The list 'expanded' with the expanded directory names.
*/
QStringList ModelCheckingWindow::getExpandedNodes(QTreeWidgetItem *propertyWidgetItem, QStringList expanded){

    for (int i = 0; i < propertyWidgetItem->childCount(); i++)
    {
        QTreeWidgetItem *child = propertyWidgetItem->child(i);
        if (child->childCount() > 0){ // is dir with children
            if (child->isExpanded()){ // and is expanded
                expanded.append(child->text(0));
            }
            expanded = expanded + getExpandedNodes(child, {});
        }
    }

    return expanded;
}

/*!
 * \brief ModelCheckingWindow::getSubtypesSelection Saves the subtyping file selected ('Qt::Checked').
 * \return The one element list 'selections' with the selected subtyping file name.
*/
QStringList ModelCheckingWindow::getSubtypesSelection(){
    QStringList selections = {};
    for (int i = 0; i < this->subtypesTopDirWidgetItem->childCount(); i++)
    {
        QTreeWidgetItem *child = this->subtypesTopDirWidgetItem->child(i);
        if (child->checkState(0) == Qt::Checked){ // subtype file is checked
            selections.append(child->text(0));
        }
    }
    if (selections.size() > 1) {
        QMessageBox::warning(this, tr("Selected subtypes"),
                             "Select up to one subtyping!");
        return {selections.at(0)};
    }
    return selections;
}

/*!
 * \brief ModelCheckingWindow::getFunctionsSelection Saves the model Functions selected ('Qt::Checked').
 * \return The list 'selections' with the names of the selected Functions.
*/
QStringList ModelCheckingWindow::getFunctionsSelection(){
    QStringList selections = {};
    for (int i = 0; i < this->functionsTopNodeWidgetItem->childCount(); i++)
    {
        QTreeWidgetItem *child = this->functionsTopNodeWidgetItem->child(i);
        if (child->checkState(0) == Qt::Checked){ // function node is checked
            selections.append(child->text(0));
        }
    }
    return selections;
}

/*!
 * \brief ModelCheckingWindow::addSubtypes Presents the user with a dialog to provide a new subtyping file name, generates the subtyping file and then adds its node to the subtypes tree.
 */
void ModelCheckingWindow::addSubtypes()
{
    bool ok;
    QString subtypingFileName = QInputDialog::getText(this, tr("New subtypes"),
                                            tr("Subtyping file name (no whitespace):"), QLineEdit::Normal,
                                            "new-subtypes", &ok);
    if (ok && !subtypingFileName.isEmpty()){
        // CHECK FILE NAME
        // use "-" instead of whitespace, if existing
        subtypingFileName.replace(" ", "-");
        // check if file with same name exists already, append suffix in that case
        QFile file;
        QString filePath = this->subtypesPath + "/" + subtypingFileName + ".asn";
        file.setFileName(filePath);
        while(file.exists())
        {
            filePath = filePath.left(filePath.size()-4) + "-1.asn";
            subtypingFileName = subtypingFileName + "-1";
            file.setFileName(filePath);
        }

        // CALL MAKE RULE
        // first set path to project dir
        QString qDirAppPath = QDir::currentPath();
        QDir::setCurrent(this->projectDir+"/");
        if (QProcess::execute("make create-subtype NAME="+subtypingFileName) != 0) {
            QMessageBox::warning(this, tr("Add subtypes"),
                                 tr("Error executing 'make create-subtype NAME=%1'").arg(subtypingFileName));
            //reset path
            QDir::setCurrent(qDirAppPath);
            return;
        }
        //reset path
        QDir::setCurrent(qDirAppPath);

        // ADD NEW TREE NODE
        QStringList fileColumn;
        QFileInfo fileInfo = filePath;
        fileColumn.append(fileInfo.fileName());
        fileColumn.append(fileInfo.filePath());
        QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
        child->setIcon(0, this->style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        child->setCheckState(0, Qt::Unchecked);
        d->ui->treeWidget_subtyping->topLevelItem(0)->addChild(child);

        statusBar()->showMessage("Subtyping " + subtypingFileName + " added.", 6000);
    }
}

/*!
 * \brief ModelCheckingWindow::deleteSubtypes Deletes subtyping file and respective node, asking user confirmation.
 */
void ModelCheckingWindow::deleteSubtypes()
{
    QFileInfo fileInfo(d->ui->treeWidget_subtyping->currentItem()->text(0));

    int ret = QMessageBox::warning(this, tr("Delete subtypes"),
                                   "Do you confirm you want to delete subtypes file " + fileInfo.fileName() + "?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes){
        // delete file
        QString rmSubtypesCmd;
        rmSubtypesCmd = "rm " + this->subtypesPath + "/" + fileInfo.fileName();
        if (QProcess::execute(rmSubtypesCmd) != 0) {
            QMessageBox::warning(this, tr("Delete subtypes"),
                                 "Error rm command");
            return;
        }

        // delete tree node
        d->ui->treeWidget_subtyping->topLevelItem(0)->removeChild(d->ui->treeWidget_subtyping->currentItem());

        statusBar()->showMessage(tr("Subtyping deleted."), 6000);
    }
}

/*!
 * \brief ModelCheckingWindow::deleteProperty Deletes a property file or property directory (and respective node), asking user confirmation.
 */
void ModelCheckingWindow::deleteProperty()
{
    QFileInfo fileInfo(d->ui->treeWidget_properties->currentItem()->text(0));
    QString fileOrDir, parent;
    // check if what is to be deleted is a file or a directory/folder
    if (d->ui->treeWidget_properties->currentItem()->text(1) == ""){ // is directory
       fileOrDir = "folder";
    }
    else{ // if file
        fileOrDir = "file";
        parent = d->ui->treeWidget_properties->currentItem()->parent()->text(0);
    }

    // confirm deletion with user
    int ret = QMessageBox::warning(this, tr("Delete property"),
                                   "Do you confirm you want to delete property " + fileOrDir + " " + fileInfo.fileName() + "?",
                                   QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes){
        if (fileOrDir == "folder"){ // is directory
            if (QProcess::execute("rm -r " + this->propertiesPath + "/" + fileInfo.fileName()) != 0) {
                QMessageBox::warning(this, tr("Delete property"),
                                     "Error rm command");
                return;
            }
        }
        else{ // is file
            if (QProcess::execute("rm " + this->propertiesPath + "/" + parent + "/" + fileInfo.fileName()) != 0) {
                QMessageBox::warning(this, tr("Delete property"),
                                     "Error rm command");
                return;
            }
        }
        // delete tree node
        d->ui->treeWidget_properties->currentItem()->parent()->removeChild(d->ui->treeWidget_properties->currentItem());

        statusBar()->showMessage(tr("Property deleted."), 6000);
    }
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_properties_customContextMenuRequested Shows respective context menu when right-clicking a properties tree node.
 */
void ModelCheckingWindow::on_treeWidget_properties_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = d->ui->treeWidget_properties->indexAt(pos);
    if (index.isValid()) {
        if (d->ui->treeWidget_properties->currentItem()->text(1) == ""){ // is directory
            QString dirName = d->ui->treeWidget_properties->currentItem()->text(0);
            if(dirName == "properties"){
                this->contextMenuPropertiesTop->exec(d->ui->treeWidget_properties->viewport()->mapToGlobal(pos));
            } else {
                this->contextMenuProperties->exec(d->ui->treeWidget_properties->viewport()->mapToGlobal(pos));
            }
        } else { // is file
            if(d->ui->treeWidget_properties->currentItem()->text(0).right(4) == ".msc"){ // is msc file
                this->contextMenuPropertiesMSCFile->exec(d->ui->treeWidget_properties->viewport()->mapToGlobal(pos));
            } else {
                this->contextMenuPropertiesFile->exec(d->ui->treeWidget_properties->viewport()->mapToGlobal(pos));
            }
        }
    }
}

/*!
 * \brief ModelCheckingWindow::on_treeWidget_subtyping_customContextMenuRequested Shows respective context menu when right-clicking a subtypes tree node.
 */
void ModelCheckingWindow::on_treeWidget_subtyping_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = d->ui->treeWidget_subtyping->indexAt(pos);
    if (index.isValid()) {
        if (d->ui->treeWidget_subtyping->currentItem()->text(1) == ""){ // is directory
                this->contextMenuSubtypes->exec(d->ui->treeWidget_subtyping->viewport()->mapToGlobal(pos));
        } else { // is file
            this->contextMenuSubtypesFile->exec(d->ui->treeWidget_subtyping->viewport()->mapToGlobal(pos));
        }
    }
}

//! This emits visibleChanged after calling the standard setVisible method.
//! And it refreshes the view if this is now visible.
//! This is used to keep the "show model checking" menu action updated.
void ModelCheckingWindow::setVisible(bool visible)
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
//! Needed by ModelCheckingWindow::setVisible
void ModelCheckingWindow::refreshView()
{
    return;
}

/*!
 * \brief ModelCheckingWindow::on_pushButton_saveConfiguration_clicked Saves configuration (all user selections) to file (user provided name via dialog), performing necessary checks.
 */
void ModelCheckingWindow::on_pushButton_saveConfiguration_clicked()
{
    // check if configurations dir exists and create it otherwise
    if (!QDir(this->configurationsPath).exists()){
        QDir().mkdir(this->configurationsPath);
    }

    // Ask user for file name
    QString configurationFileName =
            QFileDialog::getSaveFileName(this, tr("Save configuration file"),
                                         this->configurationsPath,
                                         tr("XMEL Files (*.xmel *.xml)"));

    QFile configFile;
    if (!configurationFileName.isEmpty()){
        // append extension if not present
        QFileInfo configFileInfo(configurationFileName);
        if(configFileInfo.suffix() != "xml" && configFileInfo.suffix() != "xmel"){
            configurationFileName.append(".xml");
        }
        configFile.setFileName(configurationFileName);
    } else {
        return;
    }

    // get all user selections
    QStringList propsSelection = getPropertiesSelection(this->propertiesTopDirWidgetItem, {});
    QStringList subtypesSelection = getSubtypesSelection();
    QStringList functionSelection = getFunctionsSelection();

    // check if current configuration is valid
    QString warningMsg = "Current invalid configuration.";
    if (subtypesSelection.size() > 1){
        QMessageBox::warning(this, tr("Save configuration"),
                             warningMsg.append(" Select up to one subtyping."));
        return;
    }
    if (functionSelection.size() < 1){
        QMessageBox::warning(this, tr("Save configuration"),
                             warningMsg.append(" Select at least one Function."));
        return;
    }
    if (d->ui->lineEdit_maxNumEnvRICalls->text().size() > 0 && d->ui->comboBox_expAlgorithm->currentIndex() != 0){
        QMessageBox::warning(this, tr("Save configuration"),
                             warningMsg.append(" Environment calls constraint requires a DFS exploration!"));
        return;
    }

    // create and open configuration file
    if (!configFile.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("Save configuration"),
                             "Error when opening file: " + configFile.fileName());
        return;
    }

    // write to configuration file
    QStringList ifOptions = {};
    ifOptions.append(d->ui->lineEdit_maxNumScenarios->text());
    ifOptions.append(d->ui->checkBox_errorScenarios->isChecked() ? "true" : "false");
    ifOptions.append(d->ui->checkBox_successScenarios->isChecked() ? "true" : "false");
    ifOptions.append(d->ui->lineEdit_timeLimit->text());
    ifOptions.append(d->ui->lineEdit_maxNumEnvRICalls->text());
    ifOptions.append(d->ui->comboBox_expAlgorithm->currentText().left(3) == "DFS" ? "dfs" : "bfs");
    ifOptions.append(d->ui->lineEdit_maxNumStates->text());
    XmelWriter writer(propsSelection, subtypesSelection, functionSelection, ifOptions);
    if (writer.writeFile(&configFile, configurationFileName)){
        statusBar()->showMessage("Configuration file saved as " + configurationFileName, 6000);
    } else {
        QMessageBox::warning(this, tr("Save configuration"),
                             "Error when writing to file.");
        return;
    }
}

/*!
 * \brief ModelCheckingWindow::saveConfiguration Saves configuration (all user selections) to hidden default file '.mcconfig', performing necessary checks. Used when calling the MC engine.
 */
bool ModelCheckingWindow::saveConfiguration()
{
    QString fileName = ".mcconfig";
    QFile file;

    // check if configurations dir exists and create it otherwise
    if (!QDir(this->configurationsPath).exists()){
        QDir().mkdir(this->configurationsPath);
    }

    // set path to file object
    QString filePath = this->configurationsPath + "/" + fileName + ".xml";
    file.setFileName(filePath);

    // get all user selections
    QStringList propsSelection = getPropertiesSelection(this->propertiesTopDirWidgetItem, {});
    QStringList subtypesSelection = getSubtypesSelection();
    QStringList functionSelection = getFunctionsSelection();

    // check if current configuration is valid
    QString warningMsg = "Current invalid configuration.";
    if (subtypesSelection.size() > 1){
        QMessageBox::warning(this, tr("Save configuration"),
                             warningMsg.append(" Select up to one subtyping."));
        return false;
    }
    if (functionSelection.size() < 1){
        QMessageBox::warning(this, tr("Save configuration"),
                             warningMsg.append(" Select at least one Function."));
        return false;
    }
    if (d->ui->lineEdit_maxNumEnvRICalls->text().size() > 0 && d->ui->comboBox_expAlgorithm->currentIndex() != 0){
        QMessageBox::warning(this, tr("Save configuration"),
                             warningMsg.append(" Environment calls constraint requires a DFS exploration!"));
        return false;
    }

    // create and open configuration file
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("Save configuration"),
                             "Error when opening file.");
        return false;
    }

    // write to configuration file
    QStringList ifOptions = {};
    ifOptions.append(d->ui->lineEdit_maxNumScenarios->text());
    ifOptions.append(d->ui->checkBox_errorScenarios->isChecked() ? "true" : "false");
    ifOptions.append(d->ui->checkBox_successScenarios->isChecked() ? "true" : "false");
    ifOptions.append(d->ui->lineEdit_timeLimit->text());
    ifOptions.append(d->ui->lineEdit_maxNumEnvRICalls->text());
    ifOptions.append(d->ui->comboBox_expAlgorithm->currentText().left(3) == "DFS" ? "dfs" : "bfs");
    ifOptions.append(d->ui->lineEdit_maxNumStates->text());
    XmelWriter writer(propsSelection, subtypesSelection, functionSelection, ifOptions);
    if (writer.writeFile(&file, fileName + ".xml")){
        return true;
    } else {
        QMessageBox::warning(this, tr("Save configuration"),
                             "Error when writing to file.");
        return false;
    }
}

/*!
 * \brief ModelCheckingWindow::on_pushButton_loadConfiguration_clicked Loads configuration (all user selections) from file (user provided name via dialog), performing necessary checks.
 */
void ModelCheckingWindow::on_pushButton_loadConfiguration_clicked()
{
    // check if configurations dir exists and create it otherwise
    if (!QDir(this->configurationsPath).exists()){
        QDir().mkdir(this->configurationsPath);
    }

    // Ask user for file to load
    QString configFilePath =
            QFileDialog::getOpenFileName(this, tr("Open configuration file"),
                                         this->configurationsPath,
                                         tr("XMEL Files (*.xmel *.xml)"));
    if (configFilePath.isEmpty())
        return;

    // Open selected configuration file
    QFile file(configFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Load configuration"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(configFilePath),
                                  file.errorString()));
        return;
    }

    // Read from configuration file
    XmelReader reader;
    int readRet = reader.read(&file);
    if ( readRet != 0) {
        QMessageBox::warning(this, tr("Load configuration"),
                             tr("Invalid configuration file %1")
                             .arg(QDir::toNativeSeparators(configFilePath)));
        return;
    }

    // set all user selections
    setPropertiesSelection(reader.getPropertiesSelected());
    setSubtypesSelection(reader.getSubtypesSelected());
    setFunctionsSelection(reader.getFunctionsSelected());

    // check IF options validity
    if (reader.getIfConfig().at(1) == "false" && reader.getIfConfig().at(2) == "false") {
        QMessageBox::warning(this, tr("Load configuration"),
                             tr("Invalid configuration file %1")
                             .arg(QDir::toNativeSeparators(configFilePath)));
        return;
    }
    if (reader.getIfConfig().at(4).size() > 0 && reader.getIfConfig().at(5) != "dfs") {
        QMessageBox::warning(this, tr("Load configuration"),
                             tr("Invalid configuration file %1")
                             .arg(QDir::toNativeSeparators(configFilePath)));
        return;
    }

    // set IF config params
    d->ui->lineEdit_maxNumScenarios->setText(reader.getIfConfig().at(0));
    reader.getIfConfig().at(1) == "true" ? d->ui->checkBox_errorScenarios->setCheckState(Qt::Checked) : d->ui->checkBox_errorScenarios->setCheckState(Qt::Unchecked);
    reader.getIfConfig().at(2) == "true" ? d->ui->checkBox_successScenarios->setCheckState(Qt::Checked) : d->ui->checkBox_successScenarios->setCheckState(Qt::Unchecked);
    d->ui->lineEdit_timeLimit->setText(reader.getIfConfig().at(3));
    d->ui->lineEdit_maxNumEnvRICalls->setText(reader.getIfConfig().at(4));
    reader.getIfConfig().at(5) == "dfs" ? d->ui->comboBox_expAlgorithm->setCurrentIndex(0) : d->ui->comboBox_expAlgorithm->setCurrentIndex(1);
    d->ui->lineEdit_maxNumStates->setText(reader.getIfConfig().at(6));

    statusBar()->showMessage("Configuration file " + file.fileName() + " loaded", 6000);
}

/*!
 * \brief ModelCheckingWindow::setPropertiesSelection Rebuilds properties tree with property selection provided, keeping expanded nodes. Needed when loading configuration.
 */
void ModelCheckingWindow::setPropertiesSelection(QStringList propertiesSelected){

    Q_ASSERT(propertiesSelected.size() > 0);

    QFileInfo propertiesFileInfo(this->propertiesPath);

    // save user expanded nodes
    QStringList expandedNodes = getExpandedNodes(this->propertiesTopDirWidgetItem, {this->propertiesTopDirWidgetItem->text(0)});

    // destroy tree except root
    QTreeWidgetItem *treeRoot = this->propertiesTopDirWidgetItem;
    for (int i = treeRoot->childCount(); i > 0; i--){
        treeRoot->removeChild(treeRoot->child(i-1));
    }
    // rebuild tree with selection
    this->propertiesTopDirWidgetItem->setCheckState(0, listProperties(this->propertiesTopDirWidgetItem, propertiesFileInfo, propertiesSelected, expandedNodes, 0));

}

/*!
 * \brief ModelCheckingWindow::setSubtypesSelection Rebuilds subtypes tree with subtyping selection provided. Needed when loading configuration.
 */
void ModelCheckingWindow::setSubtypesSelection(QStringList subtypesSelected){

    Q_ASSERT(subtypesSelected.size() <= 1);

    QFileInfo subtypesFileInfo(this->subtypesPath);
    // destroy tree except root
    QTreeWidgetItem *treeRoot = this->subtypesTopDirWidgetItem;
    for (int i = treeRoot->childCount(); i > 0; i--){
        treeRoot->removeChild(treeRoot->child(i-1));
    }
    // rebuild tree with selection
    listSubtypes(this->subtypesTopDirWidgetItem, subtypesFileInfo, subtypesSelected);

}

/*!
 * \brief ModelCheckingWindow::setFunctionsSelection Rebuilds Functions tree with Function selection provided. Needed when loading configuration.
 */
void ModelCheckingWindow::setFunctionsSelection(QStringList functionsSelected){

    Q_ASSERT(functionsSelected.size() > 0);

    // destroy tree except root
    QTreeWidgetItem *treeRoot = this->functionsTopNodeWidgetItem;
    for (int i = treeRoot->childCount(); i > 0; i--){
        treeRoot->removeChild(treeRoot->child(i-1));
    }
    // rebuild tree with selection
    listModelFunctions(this->functionsTopNodeWidgetItem, functionsSelected);

}

/*!
 * \brief ModelCheckingWindow::on_checkBox_errorScenarios_stateChanged Ensures at least one type of scenarios is checked.
 */
void ModelCheckingWindow::on_checkBox_errorScenarios_stateChanged(int arg1){
    if(!d->ui->checkBox_errorScenarios->isChecked()){
        d->ui->checkBox_successScenarios->setCheckState(Qt::CheckState::Checked);
    }
}

/*!
 * \brief ModelCheckingWindow::on_checkBox_successScenarios_stateChanged Ensures at least one type of scenarios is checked.
 */
void ModelCheckingWindow::on_checkBox_successScenarios_stateChanged(int arg1){
    if(!d->ui->checkBox_successScenarios->isChecked()){
        d->ui->checkBox_errorScenarios->setCheckState(Qt::CheckState::Checked);
    }
}

}
