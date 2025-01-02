#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>

namespace Ui {
class ToolWidget;
}

class ToolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolWidget(QWidget *parent = nullptr);
    ~ToolWidget();

private:
    Ui::ToolWidget *ui;
    void initWidget();
    void setIpAdress();
    void setIpDhcp();
    void updateCurrentIp();
    void executeCommand(const QString &command, const QString &successMessage, const QString &failureMessage);
    QStringList getInterfaceName();
private slots:
    void btnClickedDeal();


};

#endif // TOOLWIDGET_H
