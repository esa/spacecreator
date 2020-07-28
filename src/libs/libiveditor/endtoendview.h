#pragma once

#include <QDialog>

namespace aadlinterface {

class EndToEndView : public QDialog
{
    Q_OBJECT

public:
    explicit EndToEndView(QWidget *parent = nullptr);
    ~EndToEndView() override;

    //! This emits visibleChanged after calling the standard setVisible method
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
