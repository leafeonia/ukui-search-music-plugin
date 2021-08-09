#ifndef MUSICPLUGIN_H
#define MUSICPLUGIN_H

#include <QObject>
#include <QThreadPool>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QAction>
#include <QVector>

#include "musicPlugin_global.h"
#include "plugininterface/search-plugin-iface.h"
#include "plugininterface/action-label.h"
//#include "file-utils.h" //TODO
#include "networkutil.h"
#include "musicInfo.h"

namespace Zeeker{
class MUSICPLUGIN_EXPORT MusicPlugin : public QObject, public SearchPluginIface
{
    Q_OBJECT
public:
    MusicPlugin(QObject *parent = nullptr);
    PluginType pluginType() {return PluginType::SearchPlugin;}
    const QString name() {return tr("Music Search");}
    const QString description() {return tr("Music search.");}
    const QIcon icon() {return QIcon::fromTheme("folder");}
    void setEnable(bool enable) {m_enable = enable;}
    bool isEnable() {return m_enable;}

    QString getPluginName();
    void KeywordSearch(QString keyword,DataQueue<ResultInfo> *searchResult);
    QList<Actioninfo> getActioninfo(int type);
    void openAction(int actionkey, QString key, int type);
    QWidget *detailPage(const ResultInfo &ri);

public Q_SLOTS:
    void musicDownloadSuccess();
    void musicDownloadFail();


private:
    void initDetailPage();
    QWidget *m_detailPage = nullptr;
    QString m_currentActionKey;
    QVBoxLayout *m_detailLyt = nullptr;
    QLabel *m_iconLabel = nullptr;
    QFrame *m_nameFrame = nullptr;
    QHBoxLayout *m_nameFrameLyt = nullptr;
    QLabel *m_nameLabel = nullptr;
    QLabel *m_pluginLabel = nullptr;
    QFrame *m_line_1 = nullptr;
    QFrame *m_artistsFrame = nullptr;
    QLabel *m_artistsLabel1 = nullptr;
    QLabel *m_artistsLabel2 = nullptr;
    QHBoxLayout *m_artistsFrameLyt = nullptr;
    QFrame *m_albumFrame = nullptr;
    QLabel *m_albumLabel1 = nullptr;
    QLabel *m_albumLabel2 = nullptr;
    QHBoxLayout *m_albumFrameLyt = nullptr;

    QFrame *m_line_2 = nullptr;
    QFrame *m_actionFrame = nullptr;
    QVBoxLayout *m_actionFrameLyt = nullptr;
    ActionLabel *m_actionLabel1 = nullptr;
    ActionLabel *m_actionLabel2 = nullptr;
    ActionLabel *m_actionLabel3 = nullptr;

    QLabel* m_statusLabel = nullptr;

    QVBoxLayout * m_actionLyt = nullptr;

    bool m_enable = true;
    QList<SearchPluginIface::Actioninfo> m_actionInfo;
    NetworkUtil* m_networkUtil; //TODO: delete
    QVector<MusicInfo> m_infos;
};
}

#endif // MUSICPLUGIN_H
