
CONFIG += maemo5

QT += core gui widgets

maemo5 {lessThan(QT_MAJOR_VERSION, 6) {QT += maemo5}
greaterThan(QT_MAJOR_VERSION, 5) {
    INCLUDEPATH += /usr/include/qt6
    #LIBS+= -L /usr/libs
    LIBS+= -lqt6maemo
}
}

TARGET = hextool

SOURCES += main.cpp hextool.cpp \
    common/value.c \
    common/signed.c \
    common/registeroperation.c \
    common/register.c \
    common/numbits.c \
    common/handlebinarydigit.c \
    optionsdialog.cpp \
    copydialog.cpp
HEADERS += hextool.h \
    common/value.h \
    common/signed.h \
    common/registeroperation.h \
    common/register.h \
    common/numbits.h \
    common/handlebinarydigit.h \
    common/debug.h \
    optionsdialog.h \
    copydialog.h
FORMS += optionsdialog.ui \
    copydialog.ui
    
RESOURCES += \
    image.qrc

maemo5 {
        FORMS += hextool.ui
        DEFINES += Q_WS_MAEMO_5
        target.path = /usr/bin
        desktopfile.files = deploy/leste/$${TARGET}.desktop
        desktopfile.path = /usr/share/applications/hildon
        icon.files = deploy/icons/64x64/$${TARGET}.png
        icon.path = /usr/share/icons/hicolor/64x64/apps
        startscript.files = deploy/leste/$${TARGET}.sh
        startscript.path = /usr/share/hextool
        INSTALLS += icon startscript
    }
!maemo5 {
DEFINES += BUILD_FOR_DESKTOP
FORMS += hextool_desk.ui

DESKTOPAPP_ICONS = 48x48 64x64
desktopfile.files = genwall.desktop
desktopfile.path = /usr/share/applications
!isEmpty(DESKTOPAPP_ICONS):for(size, DESKTOPAPP_ICONS) {
    icon$${size}.files = deploy/icons/$${size}/$${TARGET}.png
    icon$${size}.path = /usr/share/icons/hicolor/$${size}/apps
    INSTALLS += icon$${size}
}
target.path=/usr/bin
}

INSTALLS += desktopfile \
        target
