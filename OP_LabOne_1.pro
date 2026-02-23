QT += core gui widgets multimedia multimediawidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    convert.c \
    entrypoint.c \
    logic.c \
    main.cpp \
    mainwindow.cpp \
    memory.c

HEADERS += \
    appcontext.h \
    convert.h \
    entrypoint.h \
    logic.h \
    mainwindow.h \
    memory.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
