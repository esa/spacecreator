#pragma once

#include <QDialog>

namespace aadlinterface {

class InterfaceDocument;

/*!
 * \brief The EndToEndView class shows the MSC message sequence through the AADL view
 */
class EndToEndView : public QDialog
{
    Q_OBJECT

public:
    explicit EndToEndView(InterfaceDocument *document, QWidget *parent = nullptr);
    ~EndToEndView() override;

    void setVisible(bool visible) override;

Q_SIGNALS:
    void visibleChanged(bool visible);

private Q_SLOTS:
    void refreshView();

private:
    struct EndToEndViewPrivate;
    EndToEndViewPrivate *d;
};

}
