#-------------------------------------------------
#
# Project created by QtCreator 2016-05-16T20:13:04
#
#-------------------------------------------------
QT     += core gui sql
QT     += network
CONFIG += c++11
CONFIG += qt plugin
unix {

}

win32 {
    QT += axcontainer
}

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else: DESTDIR = build/release

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeControl_v2_00_001
TEMPLATE = app

RC_FILE = w_time_control_qt.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_createtask.cpp \
    dialog_info.cpp \
    dialog_settings.cpp \
    dialog_projectsettings.cpp \
    controller_mainwindow.cpp \
    controller_main.cpp \
    controller_projectsettings.cpp \
    dialog_createcall.cpp \
    controller_createcall.cpp \
    dialog_viewcalllist.cpp \
    controller_viewcalllist.cpp \
    controller_find.cpp \
    dialog_find.cpp \
    dialog_timereport.cpp \
    controller_timereport.cpp \
    controller_exportexcel.cpp \
    dialog_reporttask.cpp \
    controller_reporttask.cpp \
    controller_exportcalc.cpp \
    dialog_claculateproj.cpp

HEADERS  += mainwindow.h \
    dialog_createtask.h \
    dialog_info.h \
    dialog_settings.h \
    dialog_projectsettings.h \
    controller_mainwindow.h \
    controller_main.h \
    controller_projectsettings.h \
    dialog_createcall.h \
    controller_createcall.h \
    dialog_viewcalllist.h \
    controller_viewcalllist.h \
    controller_find.h \
    dialog_find.h \
    dialog_timereport.h \
    controller_timereport.h \
    controller_exportexcel.h \
    dialog_reporttask.h \
    controller_reporttask.h \
    controller_exportcalc.h \
    dialog_claculateproj.h

FORMS    += mainwindow.ui \
    dialog_createtask.ui \
    dialog_info.ui \
    dialog_settings.ui \
    dialog_projectsettings.ui \
    dialog_createcall.ui \
    dialog_viewcalllist.ui \
    dialog_find.ui \
    dialog_timereport.ui \
    dialog_reporttask.ui \
    dialog_claculateproj.ui

RESOURCES += \
    qrc.qrc
