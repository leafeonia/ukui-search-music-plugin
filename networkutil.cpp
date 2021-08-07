#include "networkutil.h"

using namespace Zeeker;

NetworkUtil::NetworkUtil(QVector<MusicInfo>& infos, QObject *parent) : QObject(parent)
{
    m_infos = infos;
}

void NetworkUtil::get(QString name, int searchLimit, DataQueue<SearchPluginIface::ResultInfo>* searchResult)
{
    m_name = name;
    m_searchLimit = searchLimit;
    m_searchResult = searchResult;
    qInfo() << "Getting from server..";
    QString listUrl = "http://music.163.com/api/search/pc?hlpretag=&hlposttag=&s=" + name + "&type=1&offset=0&total=true&limit=" + QString::number(m_searchLimit);
    QNetworkReply* listReply = m_manager.get(QNetworkRequest(QUrl(listUrl)));
    connect(listReply, &QNetworkReply::finished, this, &NetworkUtil::listFinish);

}

void NetworkUtil::listFinish()
{
    for (MusicInfo info: m_infos) { // Q: too many new delete operations
        if (info.image) delete info.image;
    }
    m_infos.clear();
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    auto val = reply->readAll();
    QJsonDocument d = QJsonDocument::fromJson(val);
    QJsonArray songs = d.object()["result"].toObject()["songs"].toArray();
    for (int i = 0; i < m_searchLimit; i++) { //TODO: handle invalid data
        MusicInfo info;
        QJsonObject songObject = songs[i].toObject();

        info.id = songObject["id"].toInt();

        info.name = songObject["name"].toString();

        info.artists = "";
        QJsonArray artistArray = songObject["artists"].toArray();
        int size = artistArray.size();
        for (int j = 0; j < size - 1; j++) {
            info.artists += artistArray[j].toObject()["name"].toString() + "/";
        }
        if (size > 0) info.artists += artistArray[size - 1];

        info.album = songObject["album"].toObject()["name"].toString();

        m_infos.push_back(info); // Q: multiple times?

        QUrl imgUrl = QUrl(songObject["album"].toObject()["picUrl"].toString());
        m_imgUrlToIdx[imgUrl] = i;
        QNetworkReply* imgReply = m_manager.get(QNetworkRequest(imgUrl));
        connect(imgReply, &QNetworkReply::finished, this, &NetworkUtil::imageFinish);


//        QNetworkRequest request(QUrl("http://music.163.com/song/media/outer/url?id=" + QString::number(id) + ".mp3"));
//        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
//        QNetworkReply* musicReply = m_manager.get(request);
//        connect(musicReply, &QNetworkReply::finished, this, &NetworkUtil::musicFinish);
    }


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
    int idx = m_imgUrlToIdx[reply->url()];
    m_infos[idx].image = new QFile();
    m_infos[idx].image->open(QIODevice::WriteOnly);
    m_infos[idx].image->write(b);
    m_infos[idx].image->close();

    SearchPluginIface::ResultInfo ri;
    ri.actionKey = m_infos[idx].artists;
    ri.description.append(SearchPluginIface::DescriptionInfo{"key","value"});
    ri.description.append(SearchPluginIface::DescriptionInfo{"key2",m_infos[idx].album});
    ri.icon = QIcon::fromTheme("folder");
    ri.name = m_infos[idx].name;
    ri.type = 0;
    m_searchResult->enqueue(ri);

    reply->deleteLater();
}

