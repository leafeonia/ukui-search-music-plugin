#include "musicplugin.h"

using namespace Zeeker;

MusicPlugin::MusicPlugin(QObject *parent) : QObject(parent)
{
    networkUtil.get("windfall");
    SearchPluginIface::Actioninfo test { 0, tr("Test")};
    m_actionInfo << test;
}

QString MusicPlugin::getPluginName() {
    return tr("Music Plugin");
}

void MusicPlugin::KeywordSearch(QString keyword,DataQueue<ResultInfo> *searchResult) {
    SearchPluginIface::ResultInfo ri;
    ri.actionKey = "actionKey";
    ri.description.append(SearchPluginIface::DescriptionInfo{"","description"});
    /*ri.description = QVector<SearchPluginIface::DescriptionInfo>() \
                    << SearchPluginIface::DescriptionInfo{tr("Path:"), path} \
                    << SearchPluginIface::DescriptionInfo{tr("Modified time:"), info.lastModified().toString("yyyy/MM/dd hh:mm:ss")};*/
    ri.icon = QIcon::fromTheme("folder");
    ri.name = "FA";
    ri.type = 0;
    searchResult->enqueue(ri);
}

QList<SearchPluginIface::Actioninfo> MusicPlugin::getActioninfo(int type) {
    return m_actionInfo;
}

void MusicPlugin::openAction(int actionkey, QString key, int type) {

}

QWidget *MusicPlugin::detailPage(const SearchPluginIface::ResultInfo &ri)
{
    return m_detailPage; //TODO
}

