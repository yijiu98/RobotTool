#include "BaseWidget.h"
#include "ui_BaseWidget.h"
#include <QFile>

BaseWidget::BaseWidget(QWidget *parent)
    : MWidgetBase(parent)
    , ui(new Ui::BaseWidget)
    ,m_tool_widget_p(std::make_shared<ToolWidget>())
{
    ui->setupUi(this);
    loadStyle();
    this->setWindowTitle(QString("Yijiu98"));
    this->setTitleBar(ui->titleBar->getBackground());
    // ui->body->setMouseTracking(true);                       // 激活鼠标移动事件
    // ui->body->installEventFilter(this);        // 设置标题栏
    initWidget();

}
void BaseWidget::loadStyle()
{
    QFile file(":/QssFile/main.css");
    if (file.open(QFile::ReadOnly))
    {
        //用QTextStream读取样式文件不用区分文件编码 带bom也行
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }

        file.close();
        QString qss = list.join("\n");
        this->setStyleSheet("");
        qApp->setStyleSheet(qss);
    }
    else
    {
        qDebug()<<"open failed";
    }
}

void BaseWidget::initWidget()
{
    ui->stackedWidget->addWidget(m_tool_widget_p.get());
    connect(ui->titleBar,SIGNAL(subBtnClicked(SUBBTN)),this,SLOT(subBtnSlot(SUBBTN)));


}


BaseWidget::~BaseWidget()
{
    delete ui;
}

void BaseWidget::setWindowTitle(const QString &title)
{
    QWidget::setWindowTitle(title);
    ui->titleBar->setWindowTitle(title);
}
void BaseWidget::subBtnSlot(SUBBTN btn)
{
    if(btn == E_SUBBTN_TOOL)
    {
        qDebug("tool anxia");
        // m_tool_widget_p->show();
        ui->stackedWidget->setCurrentWidget(m_tool_widget_p.get());
    }
    else if(btn == E_SUBBTN_SET)
    {
        qDebug("set anxia");
    }
}
