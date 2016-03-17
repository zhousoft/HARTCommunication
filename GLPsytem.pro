#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T16:45:35
#
#-------------------------------------------------

QT       += core gui\
         serialport\
         multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GLPsytem
TEMPLATE = app

#QWT设置
DEFINES += QWT_DLL #如果要继承qwt类实现自定义类必须加此句
LIBS += -lqwt
INCLUDEPATH += D:\Qt\Qt5.4.1\5.4\mingw491_32\include\Qwt
#end QWT设置

SOURCES += main.cpp\
        widget.cpp \
    mytoolbutton.cpp \
    dropenablewidget.cpp \
    titlewidget.cpp \
    mypushbutton.cpp \
    globalvalue.cpp \
    deviceinfowidget.cpp \
    processmonitorwidget.cpp \
    currentwidget.cpp \
    twolinewuwei.cpp \
    samplecountweidget.cpp \
    otherwidget.cpp \
    lineconnectwidget.cpp \
    rightinfowidget.cpp \
    leidawuwei.cpp \
    dampingtimewidget.cpp \
    offlinewindow.cpp \
    mainwindow.cpp \
    hartasciihandle.cpp

HEADERS  += widget.h \
    mytoolbutton.h \
    dropenablewidget.h \
    titlewidget.h \
    mypushbutton.h \
    globalvalue.h \
    deviceinfowidget.h \
    processmonitorwidget.h \
    currentwidget.h \
    twolinewuwei.h \
    samplecountweidget.h \
    otherwidget.h \
    lineconnectwidget.h \
    rightinfowidget.h \
    leidawuwei.h \
    dampingtimewidget.h \
    offlinewindow.h \
    mainwindow.h \
    hartasciihandle.h

FORMS    += widget.ui \
    deviceinfowidget.ui \
    processmonitorwidget.ui \
    currentwidget.ui \
    twolinewuwei.ui \
    samplecountweidget.ui \
    otherwidget.ui \
    lineconnectwidget.ui \
    rightinfowidget.ui \
    leidawuwei.ui \
    dampingtimewidget.ui

RESOURCES += \
    images.qrc
RC_ICONS = logo.ico
