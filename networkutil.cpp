#include "networkutil.h"

NetworkUtil::NetworkUtil(QObject *parent) : QObject(parent)
{

}

void NetworkUtil::get(QString name)
{
    m_name = name;
    qInfo() << "Getting from server..";
    QString listUrl = "http://music.163.com/api/search/pc?hlpretag=&hlposttag=&s=" + name + "&type=1&offset=0&total=true&limit=1";
    QNetworkReply* listReply = m_manager.get(QNetworkRequest(QUrl(listUrl)));
    connect(listReply, &QNetworkReply::finished, this, &NetworkUtil::listFinish);

}

void NetworkUtil::listFinish()
{

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    auto val = reply->readAll();
    qInfo() << val;
    QJsonDocument d = QJsonDocument::fromJson(val);
    QJsonObject object = d.object();
    int id = object["result"].toObject()["songs"].toArray()[0].toObject()["id"].toInt();
    qInfo() << id;
    QString imgUrl = object["result"].toObject()["songs"].toArray()[0].toObject()["album"].toObject()["picUrl"].toString();
    qInfo() << imgUrl;
    QNetworkRequest request(QUrl("http://music.163.com/song/media/outer/url?id=" + QString::number(id) + ".mp3"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    QNetworkReply* musicReply = m_manager.get(request);
    connect(musicReply, &QNetworkReply::finished, this, &NetworkUtil::musicFinish);


    QNetworkReply* imgReply = m_manager.get(QNetworkRequest(QUrl(imgUrl)));
    connect(imgReply, &QNetworkReply::finished, this, &NetworkUtil::imageFinish);

    reply->deleteLater();

}

void NetworkUtil::musicFinish()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray b = reply->readAll();
    qInfo() << b.size();
    QFile file(QDir::homePath() + "/Music/" + m_name + ".mp3");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << b;
    reply->deleteLater();
}

void NetworkUtil::imageFinish()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray b = reply->readAll();
    qInfo() << "image size: " << b.size();
    m_file = new QFile();
    m_file->setFileName("/home/leafeon/Desktop/image.jpg");
    m_file->open(QIODevice::WriteOnly);
    m_file->write(b);
    m_file->close();
    reply->deleteLater();
}

