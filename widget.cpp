#include "widget.h"
#include "ui_widget.h"
#include <QtWidgets/QMessageBox>
#include <QTime>

/*QDataStream & operator>> (QDataStream &stream, QString a)
{
    while (!stream.atEnd())
    {
        a += QString(stream);
    }

    return stream;
}*/

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_socket = new QTcpSocket;

    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&Widget::read_data);
    //QObject::connect(this,&Widget::success,this,&Widget::log);

    ui->pushButton_Send->setShortcut(QKeySequence(tr("ctrl+return")));

    ui->username->setText("A1caida");
    ui->password->setText("qwe123");
    ui->pushButton_Send->setEnabled(false);

}

Widget::~Widget()
{
    delete m_socket;
    delete ui;
}

void Widget::on_pushButton_Connect_clicked()    //连接按钮
{
    m_socket->abort();
    m_socket->connectToHost("127.0.0.1",728);

    if (!m_socket->waitForConnected())
    {
        QMessageBox msgBox;
        msgBox.setText("Время соединения истекло!");
        msgBox.resize(40,30);
        msgBox.exec();
        return;
    }

    QByteArray query = "0" + ui->username->text().toUtf8() + "+" + ui->password->text().toUtf8();
    m_socket->write(query);
}

void Widget::on_pushButton_find_clicked()
{
    QByteArray user = ui->user_pm->text().toUtf8();
    user.insert(0,"2");
    m_socket->write(user);
    m_socket->flush();

    nick = ui->user_pm->text().toUtf8();
    ui->listWidget_users->addItem(ui->user_pm->text().toUtf8());

}


void Widget::on_pushButton_Send_clicked()  //发送按钮
{
    if (ui->textEdit->toPlainText()== QString()) //空消息检测
    {
        QMessageBox msgb;
        msgb.setText("不能发送空消息！");
        msgb.resize(60,40);
        msgb.exec();
        return;
    }
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append("From Client:  "+QTime::currentTime().toString());

    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
    ui->textBrowser->append(ui->textEdit->toPlainText().toUtf8());


    QByteArray msg = ui->textEdit->toPlainText().toUtf8();
    msg.insert(0,"1");
    m_socket->write(msg);
    m_socket->flush();
    ui->textEdit->clear();
}

void Widget::read_data()
{
    /*QString test;
    QDataStream datastream(m_socket);
    datastream >> test;*/

    QByteArray Data = m_socket->readAll();
    QString a = QString(Data[0]);
    int ch = a.toInt();
    if (!Data.isEmpty())
    {
        //QString str = ui->textBrowser->toPlainText();
        switch (ch)
        {
            case 0:
            {
                Data.remove(0,1);

                if(QString(Data[0]) == "0")
                {
                    QMessageBox msgBox;
                    msgBox.setText("Неправильный логин или пароль");
                    msgBox.resize(40,30);
                    msgBox.exec();

                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("Добро пожаловать!");
                    msgBox.resize(40,30);
                    msgBox.exec();

                    ui->username->setEnabled(false);
                    ui->password->setText("Трактористы всегда в моде");
                    ui->password->setEnabled(false);

                    ui->pushButton_Connect->setEnabled(false);
                    ui->pushButton_reg->setEnabled(false);
                    ui->pushButton_Send->setEnabled(false);
                    ui->pushButton_Send->setEnabled(true);
                }
                break;
            };
            case 1:
            {
                Data.remove(0,1);
                QString str=QString(Data);

                ui->textBrowser->setTextColor(Qt::gray);
                ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
                ui->textBrowser->append("From Server:  "+QTime::currentTime().toString());

                ui->textBrowser->setTextColor(Qt::black);
                ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
                ui->textBrowser->append(str);
                break;
            };
            case 2:
            {
                Data.remove(0,1);
                QString str=QString(Data);
                //int magic = 0;

                while (!str.isEmpty())
                {
                    QString sender = str.mid(0,str.indexOf("/"));
                    str.remove(0,str.indexOf("/")+1);

                    QString time = str.mid(0,str.indexOf("/"));
                    str.remove(0,str.indexOf("/")+1);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
                    ui->textBrowser->append(sender+ ":  "+time);

                    str.remove(0,str.indexOf("/")+1);
                    QString msg = str.mid(0,str.indexOf("]"));
                    ui->textBrowser->setTextColor(Qt::black);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
                    ui->textBrowser->append(msg);
                    str.remove(0,str.indexOf("]")+1);
                }


                qDebug() << Data;


                break;
            };
            default:
            {

               break;
            };

        }
    }
}

void Widget::exit()
{
    m_socket->write("3");
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_Connect->setText("Соединять");
    QMessageBox msgBox;
    msgBox.setText("Отключить!");
    msgBox.resize(40,30);
    msgBox.exec();
}

void Widget::on_pushButton_reg_clicked()
{
    m_socket->abort();
    m_socket->connectToHost("127.0.0.1",728);

    if (!m_socket->waitForConnected())
    {
        QMessageBox msgBox;
        msgBox.setText("Время соединения истекло!");
        msgBox.resize(40,30);
        msgBox.exec();
        return;
    }

    QByteArray query = "4" + ui->username->text().toUtf8() + "+" + ui->password->text().toUtf8();
    m_socket->write(query);

    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_Send->setEnabled(false);
}

