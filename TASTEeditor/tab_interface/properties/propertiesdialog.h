#pragma once

#include <QDialog>

namespace Ui {
class PropertiesDialog;
}


namespace taste3 {
namespace aadl {

class AADLObject;
class PropertiesListModel;
class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(AADLObject* obj, QWidget *parent = nullptr);
    ~PropertiesDialog();

public slots:
    void open() override;
    void done(int r) override;

private:
    Ui::PropertiesDialog *ui;
    AADLObject* m_dataObject {nullptr};
    PropertiesListModel* m_modelAttrs;

    void updateTitle();
    QString objectTypeName() const;
};

} // namespace aadl
} // namespace taste3
