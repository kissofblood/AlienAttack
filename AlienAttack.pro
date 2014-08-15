#-------------------------------------------------
#
# Project created by QtCreator 2014-07-27T03:00:59
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlienAttack
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
        alienattack.cpp \
    titlescreen.cpp \
    battlefield.cpp \
    player.cpp \
    shot.cpp \
    spriteEnemy/enemy.cpp \
    spriteEnemy/enemygroup.cpp \
    level.cpp \
    spriteEnemy/helpenemy.cpp

HEADERS  += alienattack.h \
    titlescreen.h \
    battlefield.h \
    abstractsprite.h \
    player.h \
    shot.h \
    spriteEnemy/enemy.h \
    common.h \
    spriteEnemy/enemygroup.h \
    level.h \
    spriteEnemy/helpenemy.h

RESOURCES += \
    resource.qrc
