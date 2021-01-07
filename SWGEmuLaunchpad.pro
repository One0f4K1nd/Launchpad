#-------------------------------------------------
#
# Project created by QtCreator 2013-10-30T09:37:04
#
#-------------------------------------------------

QT  += core gui network xml webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SWGMTGEmuLaunchpad
TEMPLATE = app


SOURCES += main.cpp\
    downloader.cpp \
        mainwindow.cpp \
    settings.cpp \
    statusxmlcontenthandler.cpp \
    loginservers.cpp \
    addloginserver.cpp \
    configparser.cpp \
    gameprocess.cpp \
    selfupdater.cpp \
    installfromswg.cpp \
    utils.cpp \
    gamemods.cpp \
    macroeditor.cpp \
    filescanner.cpp \
    macroiconsgraphicsview.cpp \
    editmacro.cpp \
    gamemacro.cpp \
    macroitemrect.cpp

HEADERS  += mainwindow.h \
    downloader.h \
    settings.h \
    statusxmlcontenthandler.h \
    loginservers.h \
    loginserver.h \
    addloginserver.h \
    configparser.h \
    gameprocess.h \
    selfupdater.h \
    singleinstance.h \
    installfromswg.h \
    utils.h \
    gamemods.h \
    macroeditor.h \
    filescanner.h \
    macroiconsgraphicsview.h \
    editmacro.h \
    gamemacro.h \
    macroitemrect.h

FORMS    += mainwindow.ui \
    settings.ui \
    loginservers.ui \
    addloginserver.ui \
    gameprocess.ui \
    selfupdater.ui \
    installfromswg.ui \
    gamemods.ui \
    macroeditor.ui \
    editmacro.ui

OTHER_FILES += \
    logo_yellow.png \
    emu.rc \
    LICENSE.txt \
    swgemu.svg \
    search.svg \
    play.svg \
    logo-emu.svg \
    cogs.svg \
    required.txt \
    required3.txt \
    requiredsupportfiles.txt \
    info.svg

RESOURCES += \
    rsources.qrc

win32 {
    win32-msvc:LIBS += Ws2_32.Lib Wldap32.Lib Crypt32.Lib

    SOURCES += windebugmonitor.cpp
    HEADERS += windebugmonitor.h
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}

RC_FILE = emu.rc


ICON = emu.ico

DISTFILES += \
    ModTheGalaxy_Logo.jpg \
    required3.txt \
    requiredsupportfiles.txt

#LIBS += -L$$PWD/./ -lzip

##pragma comment(lib, "Ws2_32.Lib")
##pragma comment(lib, "Wldap32.Lib")
##pragma comment(lib, "Crypt32.Lib")

#INCLUDEPATH += "D:/git/curl/builds/libcurl-vc14-x86-release-static-ipv6/include"
#LIBS += -L"D:/git/curl/builds/libcurl-vc14-x86-release-static-ipv6/lib" -llibcurl_a
#DEFINES += CURL_STATICLIB

# using shell_path() to correct path depending on platform
# escaping quotes and backslashes for file paths
#copydll.commands = $(COPY_FILE) \"$$shell_path($$PWD\\zip.dll)\" \"$$shell_path($$OUT_PWD)\"
#first.depends = $(first) copydll
#export(first.depends)
#export(copydll.commands)
#QMAKE_EXTRA_TARGETS += first copydll
