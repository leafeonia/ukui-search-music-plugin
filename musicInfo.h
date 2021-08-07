#ifndef MUSICINFO_H
#define MUSICINFO_H

namespace Zeeker {
class MusicInfo {
public:

    int id; // netease music id
    QString name;
    QVector<QString> artists;
    QString album;
    QFile* image = nullptr;

    //TODO: delete
};
}
#endif // MUSICINFO_H
