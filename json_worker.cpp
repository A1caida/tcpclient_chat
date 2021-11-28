#include "json_worker.h"

json_worker::json_worker()
{

}

bool json_worker::create(QString nickname)
{
    nick = nickname;

    QFile file;
    file.setFileName(nick+".json");

    QJsonObject da;
    da.insert("nick",QJsonValue::fromVariant(nick));

    QJsonDocument document;
    document.setObject(da);

    QByteArray bytes = document.toJson( QJsonDocument::Indented );

    if(!file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ))
    {
        qDebug() << "file open failed:";
        return false;
    }

    QTextStream iStream(&file);

    iStream << bytes;
    file.close();

    return true;
}

bool json_worker::add_pm()
{
    return true;
}

bool json_worker::add_userlist(QString user)//фикс потом ок да
{
    QFile file;
    file.setFileName(nick+".json");

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "file open failed:";
        return false;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray userlist = {user};

    QJsonArray arrLog = json.array();
    arrLog.push_back( userlist );

    QJsonDocument doc( arrLog );

    if( !file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate  ))
    {
        qDebug() << "file open failed:";
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}
