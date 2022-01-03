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

#include <QDir>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QMenu>
#include <QDebug>

namespace ive {

struct ModelCheckingWindow::ModelCheckingWindowPrivate {
    Ui::ModelCheckingWindow *ui { new Ui::ModelCheckingWindow };
    InterfaceDocument *document { nullptr };
};

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
    this->outputPath = projectDir + "/work/build/modelchecking/output";

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
    QStringList fileColumn;
    fileColumn.append(propertiesFileInfo.fileName());
    this->propertiesTopDirWidgetItem = new QTreeWidgetItem(fileColumn);
    this->propertiesTopDirWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    d->ui->treeWidget_properties->addTopLevelItem(this->propertiesTopDirWidgetItem);
    this->propertiesTopDirWidgetItem->setCheckState(0, listProperties(this->propertiesTopDirWidgetItem, propertiesFileInfo, {}));

    // Build subtyping tree view
    QFileInfo fileInfo2(this->subtypesPath);
    QStringList fileColumn2;
    fileColumn2.append(fileInfo2.fileName());
    QTreeWidgetItem *dir2 = new QTreeWidgetItem(fileColumn2);
    dir2->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    d->ui->treeWidget_subtyping->addTopLevelItem(dir2);
    listSubtypes(dir2, fileInfo2);

    // Build submodel tree view
    QStringList functionColumn3;
    functionColumn3.append("Model Functions");
    QTreeWidgetItem *topFunctionNode = new QTreeWidgetItem(functionColumn3);
    topFunctionNode->setIcon(0, this->style()->standardIcon(QStyle::SP_FileDialogListView));
    d->ui->treeWidget_submodel->addTopLevelItem(topFunctionNode);
    listModelFunctions(topFunctionNode);

    // Pre-build results tree view
    QFileInfo fileInfo4(this->outputPath);
    QStringList fileColumn4;
    fileColumn4.append(fileInfo4.fileName());
    QTreeWidgetItem *dir4 = new QTreeWidgetItem(fileColumn4);
    dir4->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    d->ui->treeWidget_results->addTopLevelItem(dir4);

    // Set validators on MC options value fileds
    d->ui->lineEdit_maxNumEnvRICalls->setValidator( new QIntValidator(0, 50, this) );
    d->ui->lineEdit_maxNumScenarios->setValidator( new QIntValidator(0, 100, this) );
    d->ui->lineEdit_maxNumStates->setValidator( new QIntValidator(0, 10000, this) );
    d->ui->lineEdit_timeLimit->setValidator( new QIntValidator(0, 14400, this) );
}

ModelCheckingWindow::~ModelCheckingWindow()
{
    delete d;
    d = nullptr;
}

Qt::CheckState ModelCheckingWindow::listProperties(QTreeWidgetItem *parentWidgetItem, QFileInfo &parentFileInfo, QStringList preSelection) {
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
            QTreeWidgetItem *childWidgetItem = new QTreeWidgetItem(childWidgetInfo);
            childWidgetItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
            parentWidgetItem->addChild(childWidgetItem);
            checkState = listProperties(childWidgetItem, childFileInfo, preSelection);
            childWidgetItem->setCheckState(0, checkState);
            if (checkState == Qt::Unchecked){parentIsFullyChecked = false;}
            if (checkState == Qt::Checked || checkState == Qt::PartiallyChecked){parentIsUnchecked = false;}
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
    if (parentIsFullyChecked && !parentIsUnchecked){return Qt::Checked;}
    if (!parentIsFullyChecked && parentIsUnchecked){return Qt::Unchecked;}
    // empty dir
    if (parentIsFullyChecked && parentIsUnchecked){return Qt::Unchecked;}
    return Qt::PartiallyChecked;

}

Qt::CheckState ModelCheckingWindow::getCheckState(QStringList selections, QString path){
    for (QString selection : selections){
        if (path.contains(selection, Qt::CaseSensitive)) {return Qt::Checked;}
    }
    return Qt::Unchecked;
}

void ModelCheckingWindow::listSubtypes(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent) {
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
                if (fileInfo.suffix() == "asn"){
                    fileColumn.append(fileInfo.filePath());
                    QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
                    child->setIcon(0, this->style()->standardIcon(QStyle::SP_TitleBarNormalButton));
                    child->setCheckState(0, Qt::Unchecked);
                    parentWidgetItem->addChild(child);
                }
        }
    }
}

void ModelCheckingWindow::listModelFunctions(QTreeWidgetItem *parentWidgetItem) {
    for (ivm::IVFunction *function : d->document->objectsModel()->allObjectsByType<ivm::IVFunction>()) {
        if(!function->hasNestedChildren()){
            QStringList functionColumn3;
            functionColumn3.append(function->title());
            QTreeWidgetItem *functionNode = new QTreeWidgetItem(functionColumn3);
            functionNode->setIcon(0, this->style()->standardIcon(QStyle::SP_MediaStop));
            functionNode->setCheckState(0, Qt::Checked);
            parentWidgetItem->addChild(functionNode);
        }
    }
}

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
        }
    }
}

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

void ModelCheckingWindow::on_treeWidget_properties_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //QDesktopServices::openUrl(QUrl::fromLocalFile(item->text(2)));
    if (item->text(1) == "") {return;}
    QFileInfo fileInfo(item->text(1));
    QProcess *process = new QProcess(this);
    QString cmd;
    if (fileInfo.completeSuffix() == "msc"){
        cmd = "msceditor -m " + item->text(1);
    } else{
        cmd = "opengeode " + item->text(1);
    }
    process->start(cmd);
}

void ModelCheckingWindow::on_treeWidget_subtyping_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //QDesktopServices::openUrl(QUrl::fromLocalFile(item->text(2)));
    if (item->text(1) == "") {return;}
    //QFileInfo fileInfo(item->text(1));
    QProcess *process = new QProcess(this);
    QString cmd;
    cmd = "kate " + item->text(1);
    process->start(cmd);
}

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

void ModelCheckingWindow::on_pushButton_callIF_clicked()
{
    // Call make rule
    if (QProcess::execute("make model-check CONFIG=the-config-file-name.xml") != 0) return;

    QString ifCallCmd;
    ifCallCmd = "if --max-env-calls=" + d->ui->lineEdit_maxNumEnvRICalls->text() + " --max-scenarios=" + d->ui->lineEdit_maxNumScenarios->text() + " --max-states=" + d->ui->lineEdit_maxNumStates->text() + " --exp-algorithm=" + d->ui->comboBox_expAlgorithm->currentText().toLower().left(3) + " --time-limit=" + d->ui->lineEdit_timeLimit->text();
    QProcess *process = new QProcess(this);
    QString cmd;
    cmd = "xterm -hold -e echo '" + ifCallCmd + "'";
    process->start(cmd);

    //Build results/output tree view
    QFileInfo fileInfo3(this->outputPath);
    listResults(d->ui->treeWidget_results->topLevelItem(0), fileInfo3);
}

void ModelCheckingWindow::on_treeWidget_results_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // QDesktopServices::openUrl(QUrl::fromLocalFile(item->text(2)));
    if (item->text(1) == "") {return;}
    QFileInfo fileInfo(item->text(1));
    QProcess *process = new QProcess(this);
    QString cmd;
    if (fileInfo.completeSuffix() == "msc"){
        cmd = "msceditor -m " + item->text(1);
    } else{
        cmd = "kate " + item->text(1);
    }
    process->start(cmd);
}

void ModelCheckingWindow::convertToObs()
{
    QFileInfo fileInfo(d->ui->treeWidget_properties->currentItem()->text(0));
    bool ok;
    QString fileName = QInputDialog::getText(this, tr("MSC to OBS"),
                                            tr("Observer file name:"), QLineEdit::Normal,
                                            fileInfo.baseName()+".pr", &ok);
    if (ok && !fileName.isEmpty()){
        // Call make rule
        if (QProcess::execute("make -C work/modelchecking/properties/mscdir/ msc2if") != 0) return;

        /*QString convertToObsCmd;
        convertToObsCmd = "msc2obs " + d->ui->treeWidget_properties->currentItem()->text(0) + " " + fileName;
        QProcess *process = new QProcess(this);
        QString cmd;
        cmd = "xterm -hold -e echo '" + convertToObsCmd + "'";
        process->start(cmd);*/
    }
}

void ModelCheckingWindow::addProperty()
{
    bool ok1;
    QString propertyType = "";
    QStringList propertyTypes = { "Boolean Stop Condition - LTL", "Boolean Stop Condition - Observer", "Message Sequence Chart", "Observer" };
    propertyType = QInputDialog::getItem(this, tr("New Property"), tr("Property type:"), propertyTypes, 0, false, &ok1);

    if (ok1 && !propertyType.isEmpty()){
        QString makeRule = "";
        if(propertyType == "Observer") {makeRule = "create-obs";}
        if(propertyType == "Message Sequence Chart") {makeRule = "create-msc";}
        if(propertyType == "Boolean Stop Condition - Observer") {makeRule = "create-bsc";}
        if(propertyType == "Boolean Stop Condition - LTL") {qDebug("LTL not yet supported"); return;}
        bool ok2;
        QString label = "Property name:                                                                                         ";
        QString propertyName = QInputDialog::getText(this, "New " + propertyType,
                                                label, QLineEdit::Normal,
                                                "new-property", &ok2);

        if (ok2 && !propertyName.isEmpty()){
            // CHECK FILE NAME
            // check if file with same name exists already, append suffix in that case
            QFile file;
            QString filePath = this->propertiesPath + "/" + propertyName;
            file.setFileName(filePath);
            while(file.exists())
            {
                //qDebug("That name already exists!");
                filePath = filePath + "-another";
                propertyName = propertyName + "-another";
                file.setFileName(filePath);
            }
            // CALL MAKE RULE
            QString makeCall = "make " + makeRule + " NAME=" + propertyName;
            QProcess *makeCallerProcess = new QProcess(this);
            //makeCallerProcess->setWorkingDirectory(this->projectDir+"/");
            QString qDirAppPath = QDir::currentPath();
            QDir::setCurrent(this->projectDir+"/");
            if (makeCallerProcess->execute(makeCall) != 0) {
                // TODO report in dialog or console instead
                qDebug("error executing make: %s (from %s)", qPrintable(makeCall), qPrintable(QDir::currentPath()));
                //return;
            } //else { TODO correct makefile errors and uncomment this else statement
                // REFRESH TREEVIEW with preselection
                QFileInfo propertiesFileInfo(this->propertiesPath);
                // save user selection
                QStringList preSelection = getPropertiesSelection(this->propertiesTopDirWidgetItem, {});
                // destroy tree except root
                QTreeWidgetItem *treeRoot = this->propertiesTopDirWidgetItem;
                for (int i = treeRoot->childCount(); i > 0; i--){
                    treeRoot->removeChild(treeRoot->child(i-1));
                }
                // rebuild tree with saved selection
                // TODO (improvement) the new tree has all nodes collapsed, so user expanded folders are lost
                this->propertiesTopDirWidgetItem->setCheckState(0, listProperties(this->propertiesTopDirWidgetItem, propertiesFileInfo, preSelection));
            //}
            QDir::setCurrent(qDirAppPath);
        }
    }
}

QStringList ModelCheckingWindow::getPropertiesSelection(QTreeWidgetItem *propertyWidgetItem, QStringList selections){
    for (int i = 0; i < propertyWidgetItem->childCount(); i++)
    {
        QTreeWidgetItem *child = propertyWidgetItem->child(i);
        if (child->childCount() == 0 && child->text(1) != ""){ // is property file
            if (child->checkState(0) == Qt::Checked){ // and is checked
                selections.append(child->parent()->text(0) + "/" + child->text(0));
            }
        } else if (child->childCount() > 0) {
            selections = selections + getPropertiesSelection(child, {});
        }
    }

    return selections;
}

void ModelCheckingWindow::addSubtypes()
{
    bool ok;
    QString fileName = QInputDialog::getText(this, tr("New subtypes"),
                                            tr("Subtyping file name:"), QLineEdit::Normal,
                                            "new-subtypes", &ok);
    if (ok && !fileName.isEmpty()){
        // CHECK FILE NAME
        // check if file with same name exists already, append suffix in that case
        QFile file;
        QString filePath = this->subtypesPath + "/" + fileName + ".asn";
        file.setFileName(filePath);
        while(file.exists())
        {
            //qDebug("That name already exists!");
            filePath = filePath.left(filePath.size()-4) + "-1.asn";
            fileName = fileName + "-1";
            file.setFileName(filePath);
        }

        //TODO remove this code in comment
        /*if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            //qDebug("File created successfully!");
            file.close();
        }
        else
        {
            qDebug("Failed creating file!");
            return;
        }*/

        // CALL MAKE RULE
        if (QProcess::execute("make create-subtype NAME="+fileName) != 0) {
            qDebug("error executing make");
            return;
        }

        //TODO remove this code in comment
        // rebuild tree view
        /*QThread::sleep(1);
        QTreeWidgetItem *treeViewRoot = d->ui->treeWidget_subtyping->topLevelItem(0);
        for (int i = treeViewRoot->childCount(); i > 0; i--){
            treeViewRoot->removeChild(treeViewRoot->child(i-1));
        }
        QFileInfo fileInfo(this->subtypesPath);
        listFile(treeViewRoot, fileInfo, true, true);*/

        // ADD NEW TREE NODE
        QStringList fileColumn;
        QFileInfo fileInfo = filePath;
        fileColumn.append(fileInfo.fileName());
        //fileColumn.append("Abstract Syntax Notation One (ASN.1)");
        fileColumn.append(fileInfo.filePath());
        QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
        child->setIcon(0, this->style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        child->setCheckState(0, Qt::Unchecked);
        d->ui->treeWidget_subtyping->topLevelItem(0)->addChild(child);
    }
}

void ModelCheckingWindow::deleteSubtypes()
{
    QFileInfo fileInfo(d->ui->treeWidget_subtyping->currentItem()->text(0));

    int ret = QMessageBox::warning(this, tr("Delete subtypes"),
                                   "Do you confirm you want to delete subtypes file " + fileInfo.fileName() + "?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes){
        // delete file
        // TODO use file methods instead
        QString rmSubtypesCmd;
        rmSubtypesCmd = "rm " + this->subtypesPath + "/" + fileInfo.fileName();
        /*QProcess *process = new QProcess(this);
        QString cmd;
        cmd = "xterm -e " + rmSubtypesCmd;
        process->start(cmd);*/
        if (QProcess::execute(rmSubtypesCmd) != 0) return;

        // rebuild tree view (we shall not rebuild otherwise selection is lost)
        // TODO delete code in comment
        /*QThread::sleep(1);
        QTreeWidgetItem *treeViewRoot = d->ui->treeWidget_subtyping->topLevelItem(0);
        for (int i = treeViewRoot->childCount(); i > 0; i--){
            treeViewRoot->removeChild(treeViewRoot->child(i-1));
        }
        QFileInfo fileInfo(this->subtypesPath);
        listFile(treeViewRoot, fileInfo, true, true);*/

        // delete tree node
        d->ui->treeWidget_subtyping->topLevelItem(0)->removeChild(d->ui->treeWidget_subtyping->currentItem());
    }
}

// TODO write code documentation
/*!
 * \brief Deletes a property file or property directory/folder.
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
                qDebug("error executing rm");
                return;
            }
        }
        else{ // is file
            if (QProcess::execute("rm " + this->propertiesPath + "/" + parent + "/" + fileInfo.fileName()) != 0) {
                qDebug("error executing rm");
                return;
            }
        }
        // delete tree node
        d->ui->treeWidget_properties->currentItem()->parent()->removeChild(d->ui->treeWidget_properties->currentItem());
    }
}

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

void ModelCheckingWindow::refreshView()
{
    return;
}
}
