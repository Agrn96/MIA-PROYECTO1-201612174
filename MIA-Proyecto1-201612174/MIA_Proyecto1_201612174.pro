QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

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
        main.cpp \
        obcat.cpp \
        obchgrp.cpp \
        obcp.cpp \
        obedit.cpp \
        obexec.cpp \
        obfdisk.cpp \
        obfind.cpp \
        oblogin.cpp \
        obloss.cpp \
        obmkdir.cpp \
        obmkdisk.cpp \
        obmkfs.cpp \
        obmkgrp.cpp \
        obmkusr.cpp \
        obmount.cpp \
        obmrdisk.cpp \
        obmv.cpp \
        obrecovery.cpp \
        obren.cpp \
        obrep.cpp \
        obrm.cpp \
        obrmgrp.cpp \
        obrmusr.cpp \
        obtouch.cpp \
        obumount.cpp \
        parser.cpp \
        scanner.cpp \
        structs.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists.txt \
    CMakeLists.txt.user \
    MIA_Proyecto1_201612174.pro.user \
    lexico.l \
    parser.y

HEADERS += \
    obcat.h \
    obchgrp.h \
    obcp.h \
    obedit.h \
    obexec.h \
    obfdisk.h \
    obfind.h \
    oblogin.h \
    obloss.h \
    obmkdir.h \
    obmkdisk.h \
    obmkfs.h \
    obmkgrp.h \
    obmkusr.h \
    obmount.h \
    obmrdisk.h \
    obmv.h \
    obrecovery.h \
    obren.h \
    obrep.h \
    obrm.h \
    obrmgrp.h \
    obrmusr.h \
    obtouch.h \
    obumount.h \
    parser.h \
    scanner.h \
    structs.h \
    structs.h
