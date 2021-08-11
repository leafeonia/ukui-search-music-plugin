QT += core xml widgets dbus concurrent network

TEMPLATE = lib
DEFINES += MUSICPLUGIN_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    musicplugin.cpp \
    networkutil.cpp

HEADERS += \
    musicInfo.h \
    musicPlugin_global.h \
    musicplugin.h \
    networkutil.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../Desktop/uk2/build-ukui-search-Desktop-Debug/libsearch/ -lukui-search

INCLUDEPATH += $$PWD/../Desktop/uk2/build-ukui-search-Desktop-Debug/libsearch
DEPENDPATH += $$PWD/../Desktop/uk2/build-ukui-search-Desktop-Debug/libsearch

#QMAKE_CXXFLAGS += -I/usr/include/chinese-seg -I/usr/include/ukui-search -I/usr/include/chinese-seg/cppjieba

#LIBS += -lchinese-segmentation -lukui-search

#TODO -libsearch

# Q_DECLARE_INTERFACE(Zeeker::SearchPluginIface, SearchPluginIface_iid) //TODO

RESOURCES += \
    resources.qrc

DISTFILES += \
    common.json
