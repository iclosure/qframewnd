
CONFIG += c++11
TEMPLATE = app

QT += core gui widgets qml quick

win32:QT += winextras

CONFIG += c++11

win32:LIBS += -luser32 -lgdi32

macos:LIBS += -framework AppKit

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    mylistmodel1.h \
    testlistmodel.h \
    myobject1.h \
    mainwindow.h \
    appwindoweventfilter.h

SOURCES += main.cpp \
    mylistmodel1.cpp \
    testlistmodel.cpp \
    myobject1.cpp \
    mainwindow.cpp \

win32 {
    #SOURCES += appwindoweventfilter.cpp
}

macos: {
    OBJECTIVE_SOURCES += \
        appwindoweventfilter.m

    #SOURCES += appwindoweventfilter.m
}

DISTFILES += \
    test1.xml
