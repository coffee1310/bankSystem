QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lWs2_32

SOURCES += \
    authorization.cpp \
    connection.cpp \
    graphic.cpp \
    histogram.cpp \
    main.cpp \
    main_menu.cpp \
    main_widget.cpp \
    mainwindow.cpp \
    registration.cpp

HEADERS += \
    authorization.h \
    connection.h \
    graphic.h \
    histogram.h \
    main_menu.h \
    main_widget.h \
    mainwindow.h \
    registration.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
