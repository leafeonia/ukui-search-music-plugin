#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QMap>
#include <QMessageBox>

#include "musicInfo.h"
#include "plugininterface/search-plugin-iface.h"

namespace Zeeker{
class NetworkUtil : public QObject
{
    Q_OBJECT
public:
    explicit NetworkUtil(QVector<MusicInfo>& infos, QObject *parent = nullptr);
    void getList(QString name, int searchLimit, DataQueue<SearchPluginIface::ResultInfo>* searchResult);
    void downloadMusic(int idx);
    void musicFinish(QNetworkReply* reply, QString name); // use lambda as slot instead of Q_SLOT

Q_SIGNALS:
    void musicDownloadSuccess();
    void musicDownloadFail();

private Q_SLOTS:
    void listFinish();
    void imageFinish();

private:
    QNetworkAccessManager m_manager;
    QFile* m_file;
    QString m_name;
    int m_searchLimit;
    QVector<MusicInfo> m_infos;
    QMap<QUrl, int> m_imgUrlToIdx;
    DataQueue<SearchPluginIface::ResultInfo>* m_searchResult = nullptr; // Q: should not be filled by networkUtil? connect to musicPlugin instead?
};
}

#endif // NETWORKUTIL_H
