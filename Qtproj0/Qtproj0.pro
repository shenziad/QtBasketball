QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SearchAve_log.cpp \
    SearchTopteam_log.cpp \
    addgamestat_log.cpp \
    avatargenerator.cpp \
    datamanage.cpp \
    main.cpp \
    mainwindow.cpp \
    playerdatatable.cpp \
    playerchartwidget.cpp \
    playerstats.cpp \
    playerselectdialog.cpp


HEADERS += \
    SearchAve_log.h \
    SearchTopteam_log.h \
    addgamestat_log.h \
    avatargenerator.h \
    datamanage.h \
    mainwindow.h \
    playerdatatable.h \
    playerchartwidget.h \
    playerstats.h \
    playerselectdialog.h

FORMS += \
    mainwindow.ui \
    addgamestat_log.ui \
    SearchTopteam_log.ui \
    SearchAve_log.ui

# RESOURCES += \
#     resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
