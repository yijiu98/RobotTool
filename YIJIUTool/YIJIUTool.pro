QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET  = chervon
TEMPLATE    = app

RC_ICONS += other/tool.ico

wasm {
DEFINES     += emsdk
RESOURCES   += other/font.qrc
} else {
DESTDIR     = $$PWD/../bin
}
CONFIG(release, debug|release){
#DESTDIR = $${DLLDIR}
OBJECTS_DIR = $$PWD/../bin/release/.obj
MOC_DIR = $$PWD/../bin/release/.moc
RCC_DIR = $$PWD/../bin/release/.rcc
UI_DIR = $$PWD/../bin/release/.ui
}
SOURCES += \
    main.cpp

# HEADERS += \
#     loginwindow.h

# FORMS += \
#     loginwindow.ui
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/LoginWindow
INCLUDEPATH += $$PWD/MainWindow
INCLUDEPATH += $$PWD/ToolWidget

include ($$PWD/LoginWindow/LoginWindow.pri)
include ($$PWD/MainWindow/MainWindow.pri)
include ($$PWD/ToolWidget/ToolWidget.pri)

#  定义程序版本号
VERSION = 1.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
