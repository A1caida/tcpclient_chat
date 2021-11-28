#ifndef JSON_WORKER_H
#define JSON_WORKER_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>

class json_worker
{
    //Q_OBJECT
public:
    json_worker();
    bool create(QString nickname);
    bool add_pm();
    bool add_userlist(QString user);

private:
    QString nick;
};

#endif // JSON_WORKER_H
