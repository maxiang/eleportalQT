QT += serialport network
LINKPATH = $$PWD/link
HEADERS += $$LINKPATH/abstractlink.h\
           $$LINKPATH/linkconfiguration.h\
           $$LINKPATH/udplink.h\
           $$LINKPATH/seriallink.h\
           $$LINKPATH/abstractlinknamespace.h\
           $$LINKPATH/parser.h\
           $$LINKPATH/pingparserext.h
SOURCES += $$LINKPATH/abstractlink.cpp \
           $$LINKPATH/linkconfiguration.cpp\
           $$LINKPATH/udplink.cpp\
           $$LINKPATH/seriallink.cpp\
           $$LINKPATH/pingparserext.cpp
INCLUDEPATH += $$LINKPATH
