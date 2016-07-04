#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T00:50:55
#
#-------------------------------------------------

QT       += core gui
QT += sql#подключение библиотек работы с sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



    #Everything below takes effect with CONFIG += static
    CONFIG += static
    win32 {
        QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    }
    #CONFIG += static-runtime
    #QTPLUGIN += qsqlite \
                #qjpeg \

    DEFINES += STATIC #Equivalent to "#define STATIC" in source code
    message("Static build.")

LIBS += "C:/Qt/4.8.6/plugins/sqldrivers/libqsqlite4.a"
LIBS += "C:/Qt/4.8.6/plugins/sqldrivers/qsqlite4.dll"
LIBS += "C:/Qt/4.8.6/plugins/imageformats/libqjpeg4.a"
LIBS += "C:/Qt/4.8.6/plugins/imageformats/qjpeg4.dll"
LIBS += "C:/Qt/4.8.6/bin/QtCore4.dll"
LIBS += "C:/Qt/4.8.6/lib/libQtCore4.a"
INCLUDEPATH+=C:/Qt/4.8.6/include/QtSql/
INCLUDEPATH+=C:/Qt/4.8.6/include/Qt/
INCLUDEPATH+=C:/Qt/4.8.6/plugins/sqldrivers/
INCLUDEPATH+=C:/Qt/4.8.6/plugins/imageformats/
INCLUDEPATH+=C:/Qt/4.8.6/include/QtCore/

TARGET = TestSqLite
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thirdparty/additional.cpp \
    dialogclients.cpp \
    dialogaddeditclient.cpp \
    dialogaddeditorder.cpp \
    dialogabout.cpp \
    dialogfunform.cpp

HEADERS  += mainwindow.h \
    thirdparty/additional.h \
    dialogclients.h \
    dialogaddeditclient.h \
    dialogaddeditorder.h \
    dialogabout.h \
    dialogfunform.h

FORMS    += mainwindow.ui \
    dialogclients.ui \
    dialogaddeditclient.ui \
    dialogaddeditorder.ui \
    dialogabout.ui \
    dialogfunform.ui

RESOURCES += \
    res_icon.qrc
