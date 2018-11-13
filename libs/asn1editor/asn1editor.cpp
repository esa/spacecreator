#include <QFileDialog>
#include <QMessageBox>

#include "asn1editor.h"
#include "asn1treeview.h"
#include "xmlparser.h"

#include "ui_asn1editor.h"

Asn1Editor::Asn1Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Asn1Editor)
{
    ui->setupUi(this);

    m_ans1TreeView = new Asn1TreeView(this);

    qobject_cast<QVBoxLayout*>(layout())->insertWidget(1, m_ans1TreeView);

    ui->btnFrame->setVisible(false);

    connect(ui->openBtn, &QPushButton::clicked, this, &Asn1Editor::openFile);
    connect(ui->typesCB, &QComboBox::currentTextChanged, this, &Asn1Editor::showAsn1Type);
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

void Asn1Editor::showAsn1Type(const QString &text)
{
    m_ans1TreeView->setAsn1Model(m_asn1Types[text].toMap());
}

void Asn1Editor::loadFile(const QString &file)
{
    XMLParser parser(file);

    connect(&parser, &XMLParser::parseError, this, &Asn1Editor::showParseError);

    m_asn1Types = parser.parseAsn1Xml();
    ui->typesCB->addItems(m_asn1Types.keys());
}

