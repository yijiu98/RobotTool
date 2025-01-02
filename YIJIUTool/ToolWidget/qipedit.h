#ifndef QIPEdit_H
#define QIPEdit_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>

class QIPEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QIPEdit(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    QString GetIP() const;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void textChanged(const QString &text);
public slots:
    void clear();
    bool setIP(const QString& strIP);

    //设置背景颜色
    void setBgColor(const QString &bgColor);
    //设置边框颜色
    void setBorderColor(const QString &borderColor);
    //设置边框圆角角度
    void setBorderRadius(int borderRadius);
private:
    void setQss();

private:
    QLabel* m_labelDot1;
    QLabel* m_labelDot2;
    QLabel* m_labelDot3;

    QLineEdit* m_lineEditIP1;
    QLineEdit* m_lineEditIP2;
    QLineEdit* m_lineEditIP3;
    QLineEdit* m_lineEditIP4;

    QString m_strIP;
    QString m_bgColor;              //背景颜色
    QString m_borderColor;          //边框颜色
    int m_borderRadius;             //边框圆角角度
};

#endif // QIPEdit_H
