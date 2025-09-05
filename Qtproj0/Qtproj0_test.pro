TEMPLATE = app
CONFIG += qt widgets

QT += widgets

TARGET = Qtproj0

HEADERS += \
    addgamestat_log.h \
    datamanage.h \
    mainwindow.h \
    playerdatatable.h \
    playerstats.h \
    playerselectdialog.h \
    SearchAve_log.h \
    SearchTopteam_log.h

SOURCES += \
    addgamestat_log.cpp \
    datamanage.cpp \
    main.cpp \
    mainwindow.cpp \
    playerdatatable.cpp \
    playerstats.cpp \
    playerselectdialog.cpp \
    SearchAve_log.cpp \
    SearchTopteam_log.cpp

FORMS += \
    addgamestat_log.ui \
    mainwindow.ui
