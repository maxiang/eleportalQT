DEFINES += NO_SERISL NO_SQL
MAVLINKC=$$PWD/mavlink_c_library_v2
INCLUDEPATH += $$MAVLINKC

HEADERS += \
    $$PWD/inc/ardusub_api.h \
    $$PWD/inc/ardusub_def.h \
    $$PWD/inc/ardusub_ini.h \
    $$PWD/inc/ardusub_interface.h \
    $$PWD/inc/ardusub_io.h \
    $$PWD/inc/ardusub_log.h \
    $$PWD/inc/ardusub_msg.h \
    $$PWD/inc/ardusub_thread.h

SOURCES += \
    $$PWD/src/ardusub_ini.c \
    $$PWD/src/ardusub_interface.c \
    $$PWD/src/ardusub_io.c \
    $$PWD/src/ardusub_log.c \
    $$PWD/src/ardusub_msg.c \
    $$PWD/src/ardusub_thread.c
