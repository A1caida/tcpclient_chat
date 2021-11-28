#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "json_worker.h"

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

    void read_data();

    void on_pushButton_reg_clicked();

signals:
    void success();
private:
    Ui::Widget *ui;
    QTcpSocket *m_socket;
    int auth = 0;
    json_worker json;


};

#endif // WIDGET_H
