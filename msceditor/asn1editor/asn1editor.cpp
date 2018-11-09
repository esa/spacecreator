#include <QFileDialog>
#include <QMessageBox>

#include "asn1editor.h"
#include "xmlparser.h"

#include "ui_asn1editor.h"

Asn1Editor::Asn1Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Asn1Editor)
{
    ui->setupUi(this);

    ui->btnFrame->setVisible(false);

    connect(ui->openBtn, &QPushButton::clicked, this, &Asn1Editor::openFile);
}

Asn1Editor::~Asn1Editor()
{
    delete ui;
}

void Asn1Editor::openFile()
{
    const QString filename = QFileDialog::getOpenFileName(this,
                                                          tr("ASN1"),
                                                          QString(""),
                                                          tr("XML files (*.xml);;All files (*.*)"));
    loadFile(filename);
}

void Asn1Editor::showParseError(const QString &error)
{
    QMessageBox::warning(this, tr("Error"), error);
}

void Asn1Editor::loadFile(const QString &file)
{
    XMLParser parser(file);

    connect(&parser, &XMLParser::parseError, this, &Asn1Editor::showParseError);

    ui->typesCB->addItems(parser.parseAsn1Xml().keys());
}

