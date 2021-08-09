#include "musicplugin.h"

using namespace Zeeker;

MusicPlugin::MusicPlugin(QObject *parent) : QObject(parent)
{
    m_networkUtil = new NetworkUtil(m_infos);
    initDetailPage();

    // create cache directory if not exists
    QString cachePath = QDir::homePath() + "/.cache/ukui-search-musicPlugin/";
    if (!QDir(cachePath).exists())
    {
        QDir().mkpath(cachePath);
    }
}

QString MusicPlugin::getPluginName() {
    return tr("Music Plugin");
}

void MusicPlugin::KeywordSearch(QString keyword, DataQueue<ResultInfo> *searchResult) {

    m_networkUtil->getList(keyword, 4, searchResult);

}

QList<SearchPluginIface::Actioninfo> MusicPlugin::getActioninfo(int type) {
    return m_actionInfo;
}

void MusicPlugin::openAction(int actionkey, QString key, int type) {

}

QWidget *MusicPlugin::detailPage(const SearchPluginIface::ResultInfo &ri)
{
    m_currentActionKey = ri.actionKey;
    m_iconLabel->setPixmap(QPixmap(ri.description.at(2).value).scaled(180, 180));
    QFontMetrics fontMetrics = m_nameLabel->fontMetrics();
    QString showname = fontMetrics.elidedText(ri.name, Qt::ElideRight, 274); //当字体长度超过215时显示为省略号
    //m_nameLabel->setText(QString("<h3 style=\"font-weight:normal;\">%1</h3>").arg(FileUtils::escapeHtml(showname)));  // Q: glib
    m_nameLabel->setText(QString(showname));
    if(QString::compare(showname, ri.name)) {
        m_nameLabel->setToolTip(ri.name);
    }
    m_pluginLabel->setText(tr("Music"));

    m_artistsLabel2->setText(m_artistsLabel2->fontMetrics().elidedText(ri.description.at(0).value, Qt::ElideRight, m_artistsLabel2->width()));
    //m_artistsLabel2->setToolTip(m_currentActionKey);
    m_albumLabel2->setText(m_albumLabel2->fontMetrics().elidedText(ri.description.at(1).value, Qt::ElideRight, m_albumLabel2->width()));
    return m_detailPage;
}

void MusicPlugin::initDetailPage()
{
    m_detailPage = new QWidget();
    m_detailPage->setFixedWidth(360);
    m_detailPage->setAttribute(Qt::WA_TranslucentBackground);
    m_detailLyt = new QVBoxLayout(m_detailPage);
    m_detailLyt->setContentsMargins(8, 0, 16, 0);
    m_iconLabel = new QLabel(m_detailPage);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedHeight(200);

    m_nameFrame = new QFrame(m_detailPage);
    m_nameFrameLyt = new QHBoxLayout(m_nameFrame);
    m_nameFrame->setLayout(m_nameFrameLyt);
    m_nameFrameLyt->setContentsMargins(8, 0, 0, 0);
    m_nameLabel = new QLabel(m_nameFrame);
    m_nameLabel->setMaximumWidth(280);
    m_pluginLabel = new QLabel(m_nameFrame);
    m_pluginLabel->setEnabled(false);
    m_nameFrameLyt->addWidget(m_nameLabel);
    m_nameFrameLyt->addStretch();
    m_nameFrameLyt->addWidget(m_pluginLabel);

    m_line_1 = new QFrame(m_detailPage);
    m_line_1->setLineWidth(0);
    m_line_1->setFixedHeight(1);
    m_line_1->setStyleSheet("QFrame{background: rgba(0,0,0,0.2);}");

    m_artistsFrame = new QFrame(m_detailPage);
    m_artistsFrameLyt = new QHBoxLayout(m_artistsFrame);
    m_artistsLabel1 = new QLabel(m_artistsFrame);
    m_artistsLabel2 = new QLabel(m_artistsFrame);
    m_artistsLabel1->setText(tr("Artist"));
    m_artistsLabel2->setFixedWidth(240);
    m_artistsLabel2->setAlignment(Qt::AlignRight);
    m_artistsFrameLyt->addWidget(m_artistsLabel1);
    m_artistsFrameLyt->addStretch();
    m_artistsFrameLyt->addWidget(m_artistsLabel2);

    m_albumFrame = new QFrame(m_detailPage);
    m_albumFrameLyt = new QHBoxLayout(m_albumFrame);
    m_albumLabel1 = new QLabel(m_albumFrame);
    m_albumLabel2 = new QLabel(m_albumFrame);
    m_albumLabel2->setAlignment(Qt::AlignRight);
    m_albumLabel1->setText(tr("Album"));
    m_albumLabel2->setFixedWidth(240);
    m_albumFrameLyt->addWidget(m_albumLabel1);
    m_albumFrameLyt->addStretch();
    m_albumFrameLyt->addWidget(m_albumLabel2);

    m_line_2 = new QFrame(m_detailPage);
    m_line_2->setLineWidth(0);
    m_line_2->setFixedHeight(1);
    m_line_2->setStyleSheet("QFrame{background: rgba(0,0,0,0.2);}");

    m_actionFrame = new QFrame(m_detailPage);
    m_actionFrameLyt = new QVBoxLayout(m_actionFrame);
    m_actionFrameLyt->setContentsMargins(8, 0, 0, 0);
    m_actionLabel1 = new ActionLabel(tr("Download"), m_currentActionKey, m_actionFrame);

    m_actionFrameLyt->addWidget(m_actionLabel1);
    m_actionFrame->setLayout(m_actionFrameLyt);

    m_detailLyt->addSpacing(50);
    m_detailLyt->addWidget(m_iconLabel);
    m_detailLyt->addWidget(m_nameFrame);
    m_detailLyt->addWidget(m_line_1);
    m_detailLyt->addWidget(m_artistsFrame);
    m_detailLyt->addWidget(m_albumFrame);
    m_detailLyt->addWidget(m_line_2);
    m_detailLyt->addWidget(m_actionFrame);
    m_detailPage->setLayout(m_detailLyt);
    m_detailLyt->addStretch();

    connect(m_actionLabel1, &ActionLabel::actionTriggered, [ & ](){
        m_networkUtil->downloadMusic(m_currentActionKey.toInt());
    });
}

// Q: copyRight?

