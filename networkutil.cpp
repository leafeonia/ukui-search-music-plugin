#include "networkutil.h"

using namespace Zeeker;

NetworkUtil::NetworkUtil(QVector<MusicInfo>& infos, QObject *parent) : QObject(parent)
{
    m_infos = infos;
}

void NetworkUtil::getList(QString name, int searchLimit, DataQueue<SearchPluginIface::ResultInfo>* searchResult, size_t uniqueSymbol)
{
    MusicPlugin::m_mutex.lock();
    if (uniqueSymbol != MusicPlugin::uniqueSymbol) {
        MusicPlugin::m_mutex.unlock();
        return;
    } else {
        MusicPlugin::m_mutex.unlock();
    }

    m_name = name;
    m_searchLimit = searchLimit;
    m_searchResult = searchResult;
    QString listUrl = "http://music.163.com/api/search/pc?hlpretag=&hlposttag=&s=" + name + "&type=1&offset=0&total=true&limit=" + QString::number(m_searchLimit);
    QNetworkReply* listReply = m_manager.get(QNetworkRequest(QUrl(listUrl)));
    connect(listReply, &QNetworkReply::finished, this, [=](){
        listFinish(uniqueSymbol);
    });

}

void NetworkUtil::listFinish(size_t uniqueSymbol)
{
    m_infos.clear();
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    auto val = reply->readAll();
    QJsonDocument d = QJsonDocument::fromJson(val);
    QJsonArray songs = d.object()["result"].toObject()["songs"].toArray();
    int songCount = d.object()["result"].toObject()["songCount"].toInt();
    for (int i = 0; i < std::min(m_searchLimit, songCount); i++) { //TODO: handle invalid data
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
        if (size > 0) info.artists += artistArray[size - 1].toObject()["name"].toString();

        info.album = songObject["album"].toObject()["name"].toString();

        MusicPlugin::m_mutex.lock();
        if (uniqueSymbol != MusicPlugin::uniqueSymbol) {
            MusicPlugin::m_mutex.unlock();
            reply->deleteLater();
            return;
        } else {
            m_infos.push_back(info);
            MusicPlugin::m_mutex.unlock();
        }


        QUrl imgUrl = QUrl(songObject["album"].toObject()["picUrl"].toString());
        QNetworkReply* imgReply = m_manager.get(QNetworkRequest(imgUrl));
        connect(imgReply, &QNetworkReply::finished, this, [=](){
            imageFinish(uniqueSymbol, i);
        });

    }
    reply->deleteLater();

}

void NetworkUtil::imageFinish(size_t uniqueSymbol, int idx)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    MusicPlugin::m_mutex.lock();
    if (uniqueSymbol != MusicPlugin::uniqueSymbol) {
        MusicPlugin::m_mutex.unlock();
        reply->deleteLater();
        return;
    } else {
        MusicPlugin::m_mutex.unlock();
    }


    QByteArray b = reply->readAll();

    // judge file format by comparing magic number of file
    QString suffixString = QString(b.mid(1, 3));
    QString suffix = suffixString == "PNG" ? ".png" : ".jpg";
    QString imgPath = QDir::homePath() + "/.cache/ukui-search-musicPlugin/" + QString::number(m_infos[idx].id) + suffix;
    QFile file(imgPath);
    file.open(QIODevice::WriteOnly);
    file.write(b);
    file.close();

    SearchPluginIface::ResultInfo ri;
    ri.actionKey = QString::number(idx);
    ri.description.append(SearchPluginIface::DescriptionInfo{"artists", m_infos[idx].artists});
    ri.description.append(SearchPluginIface::DescriptionInfo{"album", m_infos[idx].album});
    ri.description.append(SearchPluginIface::DescriptionInfo{"imgPath", imgPath});
    ri.icon = QIcon(":/res/neteaseLogo.jpg");
    ri.name = m_infos[idx].name;
    ri.type = 0;
    m_searchResult->enqueue(ri);

    reply->deleteLater();
}

void NetworkUtil::downloadMusic(int idx)
{
    if (idx >= m_infos.size()) {
        qWarning() << "music index out of boundary";
        return;
    }
    QNetworkRequest request(QUrl("http://music.163.com/song/media/outer/url?id=" + QString::number(m_infos[idx].id) + ".mp3"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply* musicReply = m_manager.get(request);
    QString name = m_infos[idx].name;
    connect(musicReply, &QNetworkReply::finished, this, [musicReply, name, this]() {
        musicFinish(musicReply, name);
    });
}

void NetworkUtil::musicFinish(QNetworkReply* reply, QString name)
{
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    if (contentType.length() < 5 || contentType.left(5) != "audio") {
        emit musicDownloadFail();
        reply->deleteLater();
        return;
    }
    QByteArray b = reply->readAll();
    QFile file(QDir::homePath() + "/Music/" + name + ".mp3");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << b;
    file.close();
    emit musicDownloadSuccess();
    reply->deleteLater();
    //QMessageBox::information(parent(), "Success", "Music successfully downloaded to " + QDir::homePath() +"/Music");
}
