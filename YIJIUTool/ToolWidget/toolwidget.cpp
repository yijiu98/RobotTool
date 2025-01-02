#include "toolwidget.h"
#include "qobjectdefs.h"
#include "ui_toolwidget.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QNetworkInterface>
#include <QList>
#include <QHostAddress>
#include <QProcess>
#include <QStringDecoder> // 引入 QStringDecoder
ToolWidget::ToolWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolWidget)
{
    ui->setupUi(this);
    initWidget();
}

ToolWidget::~ToolWidget()
{
    delete ui;
}

void ToolWidget::initWidget()
{
    ui->set_ip->setIP("192.168.31.199");
    ui->set_mask->setIP("255.255.255.0");
    ui->set_gate_way->setIP("192.168.31.1");
    connect(ui->btn_set,SIGNAL(clicked()),this,SLOT(btnClickedDeal()));
    connect(ui->btn_dhcp,SIGNAL(clicked()),this,SLOT(btnClickedDeal()));
    updateCurrentIp();
}

void ToolWidget::setIpAdress()
{


    // QString command = QString("netsh interface ip set address name=\"%1\" static %2 255.255.255.0")
    //                       .arg(interfaceName)
    //                       .arg(ui->set_ip->GetIP());
    // QString command = QString("netsh interface ip set address name=\"以太网 2\" static 192.168.31.200 255.255.255.0 192.168.31.1");
    // QString command = QString("netsh interface ip set address name=\"以太网 2\" static ")+ui->set_ip->GetIP()+" "+ui->set_mask->GetIP()+" "+ui->set_gate_way->GetIP();
    QString command = QString("netsh interface ip set address name=")+QString("\"")+ui->combox_net->currentText()+QString("\"")+QString(" static ")+ui->set_ip->GetIP()+" "+ui->set_mask->GetIP()+" "+ui->set_gate_way->GetIP();
    qDebug()<<command;
    executeCommand(command, "设置静态 IP 地址成功！", "设置静态 IP 地址失败！");
}

// QStringList ToolWidget::getInterfaceName() {
//     QProcess process;
//     process.start("cmd.exe", QStringList() << "/c" << "netsh interface show interface");
//     process.waitForFinished();

//     // 将输出从 GBK 转换为 UTF-8
//     QByteArray rawOutput = process.readAllStandardOutput();
//     // 将 GBK 编码转换为 UTF-8
//     QString output = QString::fromLocal8Bit(rawOutput);

//     // 输出转换后的内容
//     // qDebug() << output;
//     // 正则表达式匹配
//     QRegularExpression re(R"(已启用.*已连接\s+([^\r\n]+))");
//     QRegularExpressionMatchIterator matchIterator = re.globalMatch(output);
//     QStringList interfaceNames;

//     while (matchIterator.hasNext()) {
//         QRegularExpressionMatch match = matchIterator.next();
//         interfaceNames.append(match.captured(1).trimmed());
//     }

//     // 调试输出匹配结果
//     qDebug() << "Matched Interface Names:" << interfaceNames;
//     for (const QString &interfaceName : interfaceNames) {
//         qDebug() << "Interface Name:" << interfaceName;
//     }

//     return interfaceNames; // 返回所有匹配到的接口名称
// }
QStringList ToolWidget::getInterfaceName() {
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "netsh interface show interface");
    process.waitForFinished();

    // 获取输出并转换编码
    QByteArray rawOutput = process.readAllStandardOutput();
    QString output = QString::fromLocal8Bit(rawOutput);

    // 按行分割输出
    QStringList lines = output.split("\n");

    // 正则表达式匹配接口名称（跳过前三列）
    QRegularExpression re(R"(\s+\S+\s+\S+\s+([\S ]+))");  // 匹配接口名称（最后一列）

    QStringList interfaceNames;

    // 从第二行开始处理，跳过表头
    for (int i = 2; i < lines.size(); ++i) {
        const QString &line = lines[i];

        // 使用正则表达式进行匹配
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            // 捕获接口名称并去除前后空格
            interfaceNames.append(match.captured(1).trimmed());
        }
    }

    // 调试输出匹配结果
    for (const QString &interfaceName : interfaceNames) {
        qDebug() << "Interface Name:" << interfaceName;
    }

    return interfaceNames;  // 返回所有匹配到的接口名称
}







void ToolWidget::setIpDhcp()
{
    QString command = "netsh interface ip set address name=\"以太网 2\" source=dhcp";
    executeCommand(command, "启用 DHCP 成功！", "启用 DHCP 失败！");
    updateCurrentIp();
}

void ToolWidget::btnClickedDeal()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(btn == ui->btn_set)
    {
        setIpAdress();
    }
    else if(btn == ui->btn_dhcp)
    {
        setIpDhcp();
    }

}
void ToolWidget::updateCurrentIp()
{
    QStringList interfaceNames = getInterfaceName();
    for (const QString &name : interfaceNames) {
        if (ui->combox_net->findText(name) == -1) {  // 如果找不到该名称，返回 -1
            ui->combox_net->addItem(name);  // 如果不存在，则添加到 QComboBox
        }
    }
    // 使用 Lambda 表达式获取当前 IP 地址
    auto getCurrentIP = []() -> QString {

        // 获取所有网络接口
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

        for (const QNetworkInterface &iface : interfaces) {
            // 检查接口是否启用并运行
            if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
                iface.flags().testFlag(QNetworkInterface::IsRunning)) {

                // 获取该接口的所有 IP 地址
                for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
                    QHostAddress ipAddress = entry.ip();

                    // 过滤 IPv4 地址
                    if (ipAddress.protocol() == QAbstractSocket::IPv4Protocol &&
                        ipAddress != QHostAddress::LocalHost) {
                        return ipAddress.toString(); // 直接返回找到的第一个非本地 IP
                    }
                }
            }
        }

        return "No active IPv4 address found";
    };

    // 调用 Lambda 表达式并更新 IP
    ui->cur_ip->setIP(getCurrentIP());
}
void ToolWidget::executeCommand(const QString &command, const QString &successMessage, const QString &failureMessage) {
    QProcess process;
    // qDebug()<<"command:"<<command;
    process.start("cmd.exe", QStringList() << "/c" << command);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        qDebug()<<"success";
    } else {
        qDebug()<<"failed";
    }
}

