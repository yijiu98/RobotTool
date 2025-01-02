#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
    ,m_base_widget_p(std::make_shared<BaseWidget>())
{
    ui->setupUi(this);
    m_base_widget_p->show();

}

LoginWindow::~LoginWindow()
{
    delete ui;
}
