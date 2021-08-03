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

class NetworkUtil : public QObject
{
    Q_OBJECT
public:
    explicit NetworkUtil(QObject *parent = nullptr);
    void get(QString name);

Q_SIGNALS:
    void downloadFinish();

private Q_SLOTS:
    void listFinish();
    void musicFinish();
    void imageFinish();

private:
    QNetworkAccessManager m_manager;
    QFile* m_file;
    QString m_name;
};

#endif // NETWORKUTIL_H
