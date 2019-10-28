#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"
#include "propertieslistmodel.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectiface.h"
#include "app/commandsstack.h"

#include <QTableView>

#include <QDebug>

namespace taste3 {
namespace aadl {

PropertiesDialog::PropertiesDialog(AADLObject* obj, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
    , m_dataObject(obj)
    , m_modelAttrs(new PropertiesListModel(this))
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    ui->viewAttrs->setModel(m_modelAttrs);

    if(m_dataObject)
    {
        updateTitle();
        m_modelAttrs->setDataObject(m_dataObject);
        ui->viewAttrs->tableView()->resizeColumnsToContents();
    }
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

void PropertiesDialog::updateTitle()
{
    QString title = tr("Properties");
    if(m_dataObject)
        title = tr("%1 %2").arg(objectTypeName(),title);

    setWindowTitle( title);
}

QString PropertiesDialog::objectTypeName() const
{
    if(!m_dataObject)
        return QString();

    switch(m_dataObject->aadlType())
    {
    case AADLObject::AADLObjectType::AADLFunctionType:
        return tr("Function Type");
    case AADLObject::AADLObjectType::AADLFunction:
        return tr("Function");
    case AADLObject::AADLObjectType::AADLIface:
    {
        QString ifaceDirection;
        if(auto iface = qobject_cast<AADLObjectIface*>(m_dataObject))
            ifaceDirection = iface->isProvided() ? tr("Provided") : tr("Required");
        return ifaceDirection.isEmpty() ? tr("Interface") : tr("%1 Interface").arg(ifaceDirection);
    }
    case AADLObject::AADLObjectType::AADLComment:
        return tr("Comment");
    case AADLObject::AADLObjectType::AADLConnection:
        return tr("Connection");
    default:
        return QString();
    }
}

void PropertiesDialog::open()
{
    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change properties"));
    QDialog::open();
}

void PropertiesDialog::done(int r)
{
    taste3::cmd::CommandsStack::current()->endMacro();

    if(QDialog::Rejected == r)
        taste3::cmd::CommandsStack::current()->undo();

    QDialog::done(r);
}


} // namespace aadl
} // namespace taste3
