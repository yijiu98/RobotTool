#include "QIPEdit.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QApplication>
#include <QClipboard>
#include <QLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

QIPEdit::QIPEdit(QWidget *parent) : QWidget(parent)
{
    m_bgColor = "#FFFFFF";
    m_borderColor = "#A6B5B8";
    m_borderRadius = 10;

    m_labelDot1 = new QLabel(this);
    m_labelDot1->setAlignment(Qt::AlignCenter);
    m_labelDot1->setText(".");

    m_labelDot2 = new QLabel(this);
    m_labelDot2->setAlignment(Qt::AlignCenter);
    m_labelDot2->setText(".");

    m_labelDot3 = new QLabel(this);
    m_labelDot3->setAlignment(Qt::AlignCenter);
    m_labelDot3->setText(".");

    m_lineEditIP1 = new QLineEdit;
    m_lineEditIP1->setObjectName("lineEditIP1");
    m_lineEditIP1->setAlignment(Qt::AlignCenter);
    m_lineEditIP1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_lineEditIP1, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));


    m_lineEditIP2 = new QLineEdit;
    m_lineEditIP2->setObjectName("lineEditIP2");
    m_lineEditIP2->setAlignment(Qt::AlignCenter);
    m_lineEditIP2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_lineEditIP2, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

    m_lineEditIP3 = new QLineEdit;
    m_lineEditIP3->setObjectName("lineEditIP3");
    m_lineEditIP3->setAlignment(Qt::AlignCenter);
    m_lineEditIP3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_lineEditIP3, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

    m_lineEditIP4 = new QLineEdit;
    m_lineEditIP4->setObjectName("lineEditIP4");
    m_lineEditIP4->setAlignment(Qt::AlignCenter);
    m_lineEditIP4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_lineEditIP4, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

    QRegularExpression regExp(R"(2[0-4]\d|25[0-5]|[01]?\d\d?)");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);

    m_lineEditIP1->setValidator(validator);
    m_lineEditIP2->setValidator(validator);
    m_lineEditIP3->setValidator(validator);
    m_lineEditIP4->setValidator(validator);

    m_lineEditIP1->setContextMenuPolicy(Qt::NoContextMenu);
    m_lineEditIP2->setContextMenuPolicy(Qt::NoContextMenu);
    m_lineEditIP3->setContextMenuPolicy(Qt::NoContextMenu);
    m_lineEditIP4->setContextMenuPolicy(Qt::NoContextMenu);

    m_lineEditIP1->installEventFilter(this);
    m_lineEditIP2->installEventFilter(this);
    m_lineEditIP3->installEventFilter(this);
    m_lineEditIP4->installEventFilter(this);

    QFrame *frame = new QFrame(this);
    frame->setObjectName("frameIP");

    setQss();

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setSpacing(0);
    verticalLayout->addWidget(frame);

    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_lineEditIP1);
    layout->addWidget(m_labelDot1);
    layout->addWidget(m_lineEditIP2);
    layout->addWidget(m_labelDot2);
    layout->addWidget(m_lineEditIP3);
    layout->addWidget(m_labelDot3);
    layout->addWidget(m_lineEditIP4);
}

QSize QIPEdit::sizeHint() const
{
    return QSize(250, 30);
}

QSize QIPEdit::minimumSizeHint() const
{
    return QSize(50,30);
}

QString QIPEdit::GetIP() const
{
    if(m_lineEditIP1->text().isEmpty() || m_lineEditIP2->text().isEmpty() ||
        m_lineEditIP3->text().isEmpty() || m_lineEditIP4->text().isEmpty())
    {
        return "";
    }
    return m_strIP;
}

bool QIPEdit::eventFilter(QObject *watched, QEvent *event)
{
    QLineEdit* lineEditIP = qobject_cast<QLineEdit*>(watched);
    if(lineEditIP)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Period)                 //如果当前按下了小数点则移动焦点到下一个输入框
            {
                if(!lineEditIP->text().isEmpty())                  //文本框不为空时后移
                {
                    this->focusNextChild();
                    QLineEdit* lineEditNext = qobject_cast<QLineEdit*>(focusWidget());
                    lineEditNext->selectAll();
                }
            }
            else if (keyEvent->key() == Qt::Key_Backspace && lineEditIP->cursorPosition() == 0 )    //如果当前按下了小数点则移动焦点到下一个输入框
            {

                if(lineEditIP != m_lineEditIP1 && focusNextPrevChild(false))
                {
                    QLineEdit* lineEditPrev = qobject_cast<QLineEdit*>(focusWidget());
                    lineEditPrev->setCursorPosition(lineEditPrev->text().size());
                    lineEditPrev->backspace();
                }
            }
            else if(keyEvent->key() == Qt::Key_Left && lineEditIP->cursorPosition() == 0)
            {
                if(lineEditIP != m_lineEditIP1 && focusNextPrevChild(false))
                {
                    QLineEdit* lineEditPrev = qobject_cast<QLineEdit*>(focusWidget());
                    lineEditPrev->setCursorPosition(lineEditPrev->text().size());

                }
            }
            else if(keyEvent->key() == Qt::Key_Right && lineEditIP->cursorPosition() == lineEditIP->text().size())
            {
                if(lineEditIP != m_lineEditIP4 && focusNextPrevChild(true))
                {
                    QLineEdit* lineEditNext = qobject_cast<QLineEdit*>(focusWidget());
                    lineEditNext->setCursorPosition(0);
                }
                return false;
            }
            else if (keyEvent->matches(QKeySequence::Paste))
            {
                QClipboard* clipBoard = QApplication::clipboard();
                QString strIP = clipBoard->text();
                setIP(strIP);
                return  false;
            }
        }
        else if (event->type() == QEvent::InputMethod)
        {
            QInputMethodEvent* inputMethodEvent = static_cast<QInputMethodEvent*>(event);
            QString strText = inputMethodEvent->commitString();
            QString strText2 = inputMethodEvent->preeditString();
            qDebug()<<strText << "11111" << strText2;
            if(strText.contains("。") && !lineEditIP->text().isEmpty())
            {
                this->focusNextChild();
                QLineEdit* lineEditNext = qobject_cast<QLineEdit*>(focusWidget());
                lineEditNext->selectAll();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void QIPEdit::clear()
{
    m_lineEditIP1->clear();
    m_lineEditIP2->clear();
    m_lineEditIP3->clear();
    m_lineEditIP4->clear();
    m_lineEditIP1->setFocus();
}

bool QIPEdit::setIP(const QString &strIP)
{
    //先检测IP地址是否合法
    QRegularExpression regExp(R"(((2[0-4]\d|25[0-5]|[01]?\d\d?)\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?))");
    if (!regExp.match(strIP).hasMatch())
    {
        return false;
    }

    if (strIP != m_strIP)
    {
        m_strIP = strIP;
        //将IP地址填入各个网段
        QStringList list = m_strIP.split(".");
        m_lineEditIP1->setText(list.at(0));
        m_lineEditIP2->setText(list.at(1));
        m_lineEditIP3->setText(list.at(2));
        m_lineEditIP4->setText(list.at(3));
        m_lineEditIP4->setFocus();
        m_lineEditIP4->setCursorPosition(m_lineEditIP4->text().size());
    }
    return true;
}

void QIPEdit::setBgColor(const QString &bgColor)
{
    if(m_bgColor != bgColor)
    {
        m_bgColor = bgColor;
        setQss();
    }
}

void QIPEdit::setBorderColor(const QString &borderColor)
{
    if(m_borderColor != borderColor)
    {
        m_borderColor = borderColor;
        setQss();
    }
}

void QIPEdit::setBorderRadius(int borderRadius)
{
    if(m_borderRadius != borderRadius)
    {
        m_borderRadius = borderRadius;
        setQss();
    }
}

void QIPEdit::setQss()
{
    QStringList qss;
    qss.append(QString("QFrame#frameIP{border:1px solid %1;border-radius:%2px;}").arg(m_borderColor).arg(m_borderRadius));
    qss.append(QString("QLabel{min-width:15px;background-color:%1;}").arg(m_bgColor));
    qss.append(QString("QLineEdit{background-color:%1;border:none;}").arg(m_bgColor));
    qss.append(QString("QLineEdit#lineEditIP1{border-top-left-radius:%1px;border-bottom-left-radius:%1px;}").arg(m_borderRadius));
    qss.append(QString("QLineEdit#lineEditIP4{border-top-right-radius:%1px;border-bottom-right-radius:%1px;}").arg(m_borderRadius));
    setStyleSheet(qss.join(""));
}

void QIPEdit::textChanged(const QString &text)
{
    int len = text.length();
    int value = text.toInt();
    if (len == 3)
    {
        if (value >= 100 && value <= 255)
        {
            this->focusNextChild();
        }
    }

    m_strIP = QString("%1.%2.%3.%4").arg(m_lineEditIP1->text().toInt()).arg(m_lineEditIP2->text().toInt()).
              arg(m_lineEditIP3->text().toInt()).arg(m_lineEditIP4->text().toInt());
    // qDebug()<<"IP:"<<m_strIP;
}
