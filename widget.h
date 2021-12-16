#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QCryptographicHash>

struct personal_msg
{
    int sender;
    int reciever;
    QString msg;
    QString time;
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void exit();

private slots:
    void on_pushButton_Connect_clicked();
    void on_pushButton_Send_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_reg_clicked();

    void read_data();

    void on_listWidget_users_itemDoubleClicked(QListWidgetItem *item);

signals:
    void success();
private:
    Ui::Widget *ui;
    QTcpSocket *m_socket;
    QString nick = "";

    void display_msg(QString user, QString msg);
    void check_con();
};

#endif // WIDGET_H
