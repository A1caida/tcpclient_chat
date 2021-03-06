#include "widget.h"
#include "ui_widget.h"
#include <QtWidgets/QMessageBox>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_socket = new QTcpSocket;

    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&Widget::read_data);

    ui->pushButton_Send->setShortcut(QKeySequence(tr("Enter")));//dunno it doesn't work

    ui->username->setText("A1caida");
    ui->password->setText("qwe123");
    ui->pushButton_Send->setEnabled(false);
}

Widget::~Widget()
{
    delete m_socket;
    delete ui;
}


void Widget::display_msg(QString user, QString msg)
{
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append(user + ":  "+QTime::currentTime().toString());

    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
    ui->textBrowser->append(msg);
}

void Widget::check_con()
{
    if (!m_socket->waitForConnected())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка!");
        msgBox.setText("Время соединения истекло!");
        msgBox.resize(40,30);
        msgBox.exec();
        ui->username->setEnabled(true);
        ui->password->setText("");
        ui->password->setEnabled(true);

        ui->pushButton_Connect->setEnabled(true);
        ui->pushButton_reg->setEnabled(true);
        ui->pushButton_Send->setEnabled(false);
        ui->pushButton_find->setEnabled(false);
        return;
    }
}

void Widget::on_pushButton_Connect_clicked()
{
    m_socket->abort();
    m_socket->connectToHost("127.0.0.1",7272);

    check_con();

    QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Keccak_512);
    hash->addData(ui->password->text().toUtf8());

    QByteArray query = "0" + ui->username->text().toUtf8() + "+" +hash->result().toBase64();
    m_socket->write(query);
}

void Widget::on_pushButton_find_clicked()
{
    check_con();

    QByteArray user = ui->user_pm->text().toUtf8();
    user.insert(0,"2");
    m_socket->write(user);
    m_socket->flush();

    nick = ui->user_pm->text().toUtf8();

    bool check = false;
    for (int i = 0; i < ui->listWidget_users->count(); i++)
    {
        if (ui->listWidget_users->item(i)->data(Qt::DisplayRole).toString() == nick)
        {
            check = true;
            break;
        }
    }

    if(!check)
        ui->listWidget_users->addItem(ui->user_pm->text().toUtf8());

}

void Widget::on_pushButton_Send_clicked()
{
    if (ui->textEdit->toPlainText()== QString())
    {
        QMessageBox msgb;
        msgb.setWindowTitle("Ошибка!");
        msgb.setText("Нельзя отправить пустоту!");
        msgb.resize(60,40);
        msgb.exec();
        return;
    }

    display_msg(ui->username->text().toUtf8(),ui->textEdit->toPlainText().toUtf8());

    check_con();

    QByteArray msg = nick.toUtf8() + "/" + ui->textEdit->toPlainText().toUtf8();
    msg.insert(0,"1");
    m_socket->write(msg);
    m_socket->flush();
    ui->textEdit->clear();
}

void Widget::read_data()
{
    QByteArray Data = m_socket->readAll();
    QString a = QString(Data[0]);
    int ch = a.toInt();
    if (!Data.isEmpty())
    {
        switch (ch)
        {
            case 0:
            {
                Data.remove(0,1);

                if(QString(Data[0]) == "0")
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Ошибка!");
                    msgBox.setText("Неправильный логин или пароль");
                    msgBox.resize(40,30);
                    msgBox.exec();
                    exit();

                }
                else if(QString(Data[0]) == "1")
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Успешный вход!");
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
                    ui->pushButton_find->setEnabled(true);
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Ошибка!");
                    msgBox.setText("Этот пользователь уже существует!");
                    msgBox.resize(40,30);
                    msgBox.exec();
                    exit();
                }

                break;
            };
            case 1:
            {
                Data.remove(0,2);
                QString msg=QString(Data);
                QString username = msg.mid(1,msg.mid(0,1).toInt());
                msg.remove(0,msg.mid(0,1).toInt()+1);

                if(nick != username)//человек разговаривает с другим пользователем
                {
                    bool check = false;
                    for (int i = 0; i < ui->listWidget_users->count(); i++)
                    {
                        if (ui->listWidget_users->item(i)->data(Qt::DisplayRole).toString() == username)
                        {
                            ui->listWidget_users->item(i)->setText(username+"(Новое сообщение)");
                            check = true;
                            break;
                        }
                        else if(ui->listWidget_users->item(i)->data(Qt::DisplayRole).toString() == username+"(Новое сообщение)")
                        {
                            check = true;
                            break;
                        }
                    }

                    if(!check)
                    {
                        ui->listWidget_users->addItem(username+"(Новое сообщение)");
                    }
                    break;
                }

                display_msg(nick,msg);

                break;
            };
            case 2:
            {
                Data.remove(0,1);
                QString str=QString(Data);
                ui->textBrowser->clear();

                while (!str.isEmpty())
                {
                    QVector<int> size;
                    for (int i = 0;i < 3;i++)
                    {
                        QString temp = str.mid(0,str.indexOf("/"));
                        str.remove(0,str.indexOf("/")+1);
                        size.push_back(temp.toInt());
                    }

                    QString sender = str.mid(0,size[0]);
                    str.remove(0,size[0]);
                    QString time = str.mid(0,size[1]);
                    str.remove(0,size[1]);
                    QString msg = str.mid(0,size[2]);
                    str.remove(0,size[2]);

                    ui->textBrowser->setTextColor(Qt::gray);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
                    ui->textBrowser->append(sender+ ":  "+time);

                    ui->textBrowser->setTextColor(Qt::black);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
                    ui->textBrowser->append(msg);

                }
                break;
            };
            case 5:
            {
                Data.remove(0,1);
                QString str=QString(Data);
                QMessageBox msgBox;
                msgBox.setText(str);
                msgBox.resize(40,30);
                msgBox.exec();
            }
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
    QMessageBox msgBox;
    msgBox.setWindowTitle("Ошибка!");
    msgBox.setText("Связь потеряна.");// это кринж и костыль
    msgBox.resize(40,30);
    msgBox.exec();
}

void Widget::on_pushButton_reg_clicked()
{
    m_socket->abort();
    m_socket->connectToHost("127.0.0.1",7272);

    check_con();

    QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Keccak_512);
    hash->addData(ui->password->text().toUtf8());
    qDebug() << hash->result().toBase64();

    QByteArray query = "4" + ui->username->text().toUtf8() + "+" + hash->result().toBase64();
    m_socket->write(query);
}

void Widget::on_listWidget_users_itemDoubleClicked(QListWidgetItem *item)
{
    QString name =  item->text().toUtf8();
    name = name.mid(0,name.indexOf("("));

    item->setText(name);
    QByteArray user = item->text().toUtf8();
    user.insert(0,"2");
    m_socket->write(user);
    m_socket->flush();

    nick = item->text().toUtf8();
}
