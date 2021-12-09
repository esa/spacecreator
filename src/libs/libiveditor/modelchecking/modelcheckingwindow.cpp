/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include <QDir>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QMenu>

namespace ive {

struct ModelCheckingWindow::ModelCheckingWindowPrivate {
    Ui::ModelCheckingWindow *ui { new Ui::ModelCheckingWindow };
    InterfaceDocument *document { nullptr };
};

ModelCheckingWindow::ModelCheckingWindow(InterfaceDocument *document, QWidget *parent) :
    QMainWindow(parent)
    , d(new ModelCheckingWindowPrivate)
{
    d->ui->setupUi(this);

    d->document = document;

    // Set paths for properties, subtypes and results/oputputs
    this->propertiesPath = "./work/modelchecking/properties";
    this->subtypesPath = "./work/modelchecking/subtypes";
    this->outputPath = "./work/build/modelchecking/output";

    // Define right-click menus and set menu policy
    this->contextMenuPropertiesTop = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuProperties = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuPropertiesFile = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuPropertiesMSCFile = new QMenu(d->ui->treeWidget_properties);
    this->contextMenuSubtypes = new QMenu(d->ui->treeWidget_subtyping);
    this->contextMenuSubtypesFile = new QMenu(d->ui->treeWidget_subtyping);
    d->ui->treeWidget_properties->setContextMenuPolicy(Qt::CustomContextMenu);
    d->ui->treeWidget_subtyping->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *newProperty = this->contextMenuPropertiesTop->addAction("New property");
    QAction *deletePropertyDir = this->contextMenuProperties->addAction("Delete property");
    QAction *MSC2OBS = this->contextMenuPropertiesMSCFile->addAction("Convert MSC to Observer");
    QAction *deleteProperty = this->contextMenuPropertiesFile->addAction("Delete property");
    QAction *deleteMSC = this->contextMenuPropertiesMSCFile->addAction("Delete property");
    QAction *newSubtypes = this->contextMenuSubtypes->addAction("New subtypes");
    QAction *deleteSubtypes = this->contextMenuSubtypesFile->addAction("Delete subtypes");
    connect(newProperty, SIGNAL(triggered()), this, SLOT(addProperty()));
    connect(deletePropertyDir, SIGNAL(triggered()), this, SLOT(deleteProperty()));
    connect(MSC2OBS, SIGNAL(triggered()), this, SLOT(convertToObs()));
    connect(deleteProperty, SIGNAL(triggered()), this, SLOT(deleteProperty()));
    connect(deleteMSC, SIGNAL(triggered()), this, SLOT(deleteProperty()));
    connect(newSubtypes, SIGNAL(triggered()), this, SLOT(addSubtypes()));
    connect(deleteSubtypes, SIGNAL(triggered()), this, SLOT(deleteSubtypes()));

    // Make tree views show horizontal scroll bars
    d->ui->treeWidget_properties->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->treeWidget_subtyping->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->treeWidget_results->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Build properties tree view
    QFileInfo fileInfo(this->propertiesPath);
    QStringList fileColumn;
    fileColumn.append(fileInfo.fileName());
    QTreeWidgetItem *dir = new QTreeWidgetItem(fileColumn);
    dir->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    dir->setCheckState(0, Qt::Unchecked);
    d->ui->treeWidget_properties->addTopLevelItem(dir);
    listFile(dir, fileInfo, false, true);

    // Build subtyping tree view
    QFileInfo fileInfo2(this->subtypesPath);
    QStringList fileColumn2;
    fileColumn2.append(fileInfo2.fileName());
    QTreeWidgetItem *dir2 = new QTreeWidgetItem(fileColumn2);
    dir2->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    //dir2->setCheckState(0, Qt::Unchecked);
    d->ui->treeWidget_subtyping->addTopLevelItem(dir2);
    listFile(dir2, fileInfo2, true, true);

    // Pre-build results tree view
    QFileInfo fileInfo3(this->outputPath);
    QStringList fileColumn3;
    fileColumn3.append(fileInfo3.fileName());
    QTreeWidgetItem *dir3 = new QTreeWidgetItem(fileColumn3);
    dir3->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
    //dir2->setCheckState(0, Qt::Unchecked);
    d->ui->treeWidget_results->addTopLevelItem(dir3);
    //listFile(dir3, fileInfo3, true, false);

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

void ModelCheckingWindow::listFile(QTreeWidgetItem *parentWidgetItem, QFileInfo &parent, bool noSubDirectories, bool checkable) {
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
        else if(fileInfo.isDir()) {
            if (noSubDirectories) continue;
            QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
            child->setIcon(0, this->style()->standardIcon(QStyle::SP_DirIcon));
            if (checkable) {child->setCheckState(0, Qt::Unchecked);}
            parentWidgetItem->addChild(child);
            listFile(child, fileInfo, true, checkable);
        }
        else {
                if (fileInfo.suffix() == "msc" || fileInfo.suffix() == "pr"){
                    //fileColumn.append(fileInfo.completeSuffix() == "msc" ? "Message Sequence Chart" : "Observer");
                    fileColumn.append(fileInfo.filePath());
                    QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
                    if (fileInfo.suffix() == "msc") {
                        child->setIcon(0, this->style()->standardIcon(QStyle::SP_MediaPlay));
                    } else {
                        child->setIcon(0, this->style()->standardIcon(QStyle::SP_FileDialogContentsView));
                    }
                    if (checkable) {child->setCheckState(0, Qt::Unchecked);}
                    parentWidgetItem->addChild(child);
                }
                if (fileInfo.suffix() == "asn"){
                    //fileColumn.append("Abstract Syntax Notation One (ASN.1)");
                    fileColumn.append(fileInfo.filePath());
                    QTreeWidgetItem *child = new QTreeWidgetItem(fileColumn);
                    child->setIcon(0, this->style()->standardIcon(QStyle::SP_TitleBarNormalButton));
                    if (checkable) {child->setCheckState(0, Qt::Unchecked);}
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
    listFile(d->ui->treeWidget_results->topLevelItem(0), fileInfo3, true, false);
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
    //if (d->ui->treeWidget_properties->currentItem() == nullptr) return;
    bool ok;
    QString propertyType;
    //if (type == "Any"){
        QStringList propertyTypes = { "Boolean Stop Condition - LTL", "Boolean Stop Condition - Observer", "Message Sequence Chart", "Observer" };
        propertyType = QInputDialog::getItem(this, tr("New Property"), tr("Property type:"), propertyTypes, 0, false, &ok);
    //} else {
    //    propertyType = type;
    //    ok = true;
    //}


    if (ok && !propertyType.isEmpty()){

        //TODO change below
        this->addOBS();
        return;

        //if (propertyType == "Boolean Stop Condition - LTL") return;
        QString extenxion = propertyType == "Message Sequence Chart" ? "msc" : propertyType == "Boolean Stop Condition - LTL" ? "ltl" : "pr";
        QString label = "Property name:                                                                                         ";
        QString fileName = QInputDialog::getText(this, "New " + propertyType,
                                                label, QLineEdit::Normal,
                                                "new-property." + extenxion, &ok);
        if (ok && !fileName.isEmpty()){
            QString testMessage;
            testMessage = "creating property " + fileName + " of type " + propertyType;
            QProcess *process = new QProcess(this);
            QString cmd;
            cmd = "xterm -hold -e echo '" + testMessage + "'";
            process->start(cmd);
        }
    }
}

void ModelCheckingWindow::addBSC()
{
        bool ok;
        QString propertyType = "Boolean Stop Condition - Observer";
        QString extenxion = "pr";
        QString label = "Property name:                                                                                         ";
        QString fileName = QInputDialog::getText(this, "New " + propertyType,
                                                label, QLineEdit::Normal,
                                                "new-property." + extenxion, &ok);
        if (ok && !fileName.isEmpty()){
            // Call make rule
            // TODO use name from input (name shall not have the extension!)
            if (QProcess::execute("make create-bsc NAME=my-new-bsc") != 0) return;

            /*QString testMessage;
            testMessage = "creating property " + fileName + " of type " + propertyType;
            QProcess *process = new QProcess(this);
            QString cmd;
            cmd = "xterm -hold -e echo '" + testMessage + "'";
            process->start(cmd);*/
        }
}

void ModelCheckingWindow::addMSC()
{
        bool ok;
        QString propertyType = "Message Sequence Chart";
        QString extenxion = "msc";
        QString label = "Property name:                                                                                         ";
        QString fileName = QInputDialog::getText(this, "New " + propertyType,
                                                label, QLineEdit::Normal,
                                                "new-property." + extenxion, &ok);
        if (ok && !fileName.isEmpty()){
            // Call make rule
            // TODO use name from input without extention, but add suffix if already existing
            if (QProcess::execute("make create-msc NAME=my-new-msc") != 0) return;

            /*QString testMessage;
            testMessage = "creating property " + fileName + " of type " + propertyType;
            QProcess *process = new QProcess(this);
            QString cmd;
            cmd = "xterm -hold -e echo '" + testMessage + "'";
            process->start(cmd);*/
        }
}

void ModelCheckingWindow::addOBS()
{
        bool ok;
        QString propertyType = "Observer";
        QString extenxion = "pr";
        QString label = "Property name:                                                                                         ";
        QString fileName = QInputDialog::getText(this, "New " + propertyType,
                                                label, QLineEdit::Normal,
                                                "new-property." + extenxion, &ok);
        if (ok && !fileName.isEmpty()){
            // Call make rule
            if (QProcess::execute("make create-obs NAME=my-new-obs") != 0) return;

            /*QString testMessage;
            testMessage = "creating property " + fileName + " of type " + propertyType;
            QProcess *process = new QProcess(this);
            QString cmd;
            cmd = "xterm -hold -e echo '" + testMessage + "'";
            process->start(cmd);*/
        }

        //TODO add node to treeview
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

void ModelCheckingWindow::deleteProperty()
{
    QFileInfo fileInfo(d->ui->treeWidget_properties->currentItem()->text(0));
    QString fileOrDir, parent;
    if (d->ui->treeWidget_properties->currentItem()->text(1) == ""){ // is directory
       fileOrDir = "folder";
    }
    else{ // if file
        fileOrDir = "file";
        parent = d->ui->treeWidget_properties->currentItem()->parent()->text(0);
    }

    int ret = QMessageBox::warning(this, tr("Delete property"),
                                   "Do you confirm you want to delete property " + fileOrDir + " " + fileInfo.fileName() + "?",
                                   QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes){
        if (d->ui->treeWidget_properties->currentItem()->text(1) == ""){ // is directory
            if (QProcess::execute("rm -r " + this->propertiesPath + "/" + fileInfo.fileName()) != 0) {
                qDebug("error executing rm");
                return;
            }
        }
        else{ // if file
            if (QProcess::execute("rm " + this->propertiesPath + "/" + parent + "/" + fileInfo.fileName()) != 0) {
                qDebug("error executing rm");
                return;
            }
        }
    }

    // delete tree node
    d->ui->treeWidget_properties->currentItem()->parent()->removeChild(d->ui->treeWidget_properties->currentItem());
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
