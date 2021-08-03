#ifndef MUSICPLUGIN_H
#define MUSICPLUGIN_H

#include "musicPlugin_global.h"
#include "plugininterface/search-plugin-iface.h"
#include "networkutil.h"

namespace Zeeker{
class MUSICPLUGIN_EXPORT MusicPlugin : public QObject, public SearchPluginIface
{
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

private:
    bool m_enable = true;
    QList<SearchPluginIface::Actioninfo> m_actionInfo;
    NetworkUtil networkUtil;
    QWidget *m_detailPage = nullptr;
};
}

#endif // MUSICPLUGIN_H
