QT       += core gui quickwidgets qml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
#subdir
INCLUDEPATH += $$PWD/src/inc
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
    src/main.cpp \
    src/mainwindow.cpp\
    src/qFlightInstruments.cpp\
    src/VideoReceiver.cpp

HEADERS += \
    src/inc/mainwindow.h\
    src/inc/VideoReceiver.h\
    src/inc/qFlightInstruments.h
    src/inc/ardusub_api_test.h

FORMS += \
    ui/mainwindow.ui

TRANSLATIONS += \
    translation/en_AU.ts
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc/assets.qrc

GstreamerDir=$$(GSTREAMER_1_0_ROOT_X86_64)

INCLUDEPATH += $${GstreamerDir}/include/glib-2.0
DEPENDPATH += $${GstreamerDir}/include/glib-2.0
INCLUDEPATH += $${GstreamerDir}/include/gstreamer-1.0
DEPENDPATH += $${GstreamerDir}/include/gstreamer-1.0

LIBS += $${GstreamerDir}/lib/glib-2.0.lib
LIBS += $${GstreamerDir}/lib/gstreamer-1.0.lib
LIBS += $${GstreamerDir}/lib/gstvideo-1.0.lib
LIBS += $${GstreamerDir}/lib/gobject-2.0.lib
LIBS += $${GstreamerDir}/lib/gthread-2.0.lib
LIBS += $${GstreamerDir}/lib/gio-2.0.lib
LIBS += $${GstreamerDir}/lib/intl.lib

INCLUDEPATH += $$PWD/dependencies/inc
DEPENDPATH  += $$PWD/dependencies/inc
LIBS += $$PWD/dependencies/lib/libardusub.dll.a
#boost-1_73
Boost_INCLUDE_DIR=C:/Boost/include/boost-1_73
BOOST_LIBRARYDIR=C:/Boost/lib
#ping-cpp
INCLUDEPATH += $$PWD/dependencies/ping-cpp
INCLUDEPATH += $$PWD/dependencies/ping-cpp/lib/fmt/include

INCLUDEPATH += $$PWD/dependencies/ping-cpp/src/device
INCLUDEPATH += $$PWD/dependencies/ping-cpp/src/generate
INCLUDEPATH += $$PWD/dependencies/ping-cpp/src/hal
INCLUDEPATH += $$PWD/dependencies/ping-cpp/src/message

CONFIG(debug, debug|release) {
LIBS += $$PWD/dependencies/lib/debug/libDEVICE.a
LIBS += $$PWD/dependencies/lib/debug/libfmt.a
LIBS += $$PWD/dependencies/lib/debug/libHAL.a

} else {
LIBS += $$PWD/dependencies/lib/libDEVICE.a
LIBS += $$PWD/dependencies/lib/libfmt.a
LIBS += $$PWD/dependencies/lib/libHAL.a
}
