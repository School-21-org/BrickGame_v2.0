QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/rcc
UI_DIR = $$OUT_PWD/ui

QMAKE_CFLAGS += -Wall -Wextra -Werror -std=gnu11
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++17

SOURCES += \
    fieldwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    snakewindow.cpp \
    tetriswindow.cpp \
    ../../brick_game/snake/controller.cpp \
    ../../brick_game/snake/fsm.cpp \
    ../../brick_game/tetris/tetris_contr.c \
    ../../brick_game/tetris/tetris_fsm.c \
    ../../brick_game/tetris/tetris_additionals.c

HEADERS += \
    fieldwidget.h \
    mainwindow.h \
    snakewindow.h \
    tetriswindow.h \

FORMS += \
    mainwindow.ui \
    snakewindow.ui \
    tetriswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = brickgame
