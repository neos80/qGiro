#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T10:21:16
#
#-------------------------------------------------

QT       += core gui gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.3
#QMAKE_TARGET_COMPANY = Oleg M.Kosorukov
#QMAKE_TARGET_PRODUCT = Giro
#QMAKE_TARGET_DESCRIPTION = App calc giro
#QMAKE_TARGET_COPYRIGHT = Oleg M.Kosorukov 2019

TARGET = qGiro
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    geo.cpp

HEADERS  += mainwindow.h \
    geo.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += ico/ico.qrc
RC_FILE = myapp.rc


CONFIG += release
CONFIG += static

QMAKE_LFLAGS += -static -static-libgcc
LIBS += -static-libgcc

# Выбираем директорию сборки исполняемого файла
# в зависимости от режима сборки проекта
CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/../GiroRelease/debug/
} else {
    DESTDIR = $$OUT_PWD/../bin/
}

# разделяем по директориям все выходные файлы проекта
MOC_DIR = common/moc
RCC_DIR = common/rcc
UI_DIR = common/ui
unix:OBJECTS_DIR =common/unix
win32:OBJECTS_DIR = common/win32
macx:OBJECTS_DIR = common/mac

#Сборка библиотек
isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 {
    DEPLOY_COMMAND = $$(QTDIR)/bin/windeployqt
}
macx {
    DEPLOY_COMMAND = $$(QTDIR)/bin/macdeployqt
}

CONFIG( debug, debug|release ) {
    # debug
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/../GiroRelease/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
} else {
    # release
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/../bin/$${TARGET}$${TARGET_CUSTOM_EXT}))
}

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

# Use += instead of = if you use multiple QMAKE_POST_LINKs
QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
#D:\Qt\5.12.1\mingw73_64/bin/windeployqt "D:\Qt Project\GiroPr\GiroRelease\..\bin\qGiro.exe"
