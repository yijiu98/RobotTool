#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "mwidgetbase.h"
#include "titlebar.h"
#include "../ToolWidget/toolwidget.h"
namespace Ui {
class BaseWidget;
}

class BaseWidget : public MWidgetBase
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    ~BaseWidget();

private:
    Ui::BaseWidget *ui;
private:
    void setWindowTitle(const QString& title);
    void windowLayout();
    void loadStyle();
    void initWidget();

private slots:
    void subBtnSlot(SUBBTN btn);

private:
    std::shared_ptr<ToolWidget> m_tool_widget_p;
};


#endif // BASEWIDGET_H
