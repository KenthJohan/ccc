TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= warn_on

QMAKE_CFLAGS = -Wno-unused-function

#QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -Wno-unused-function
#QMAKE_CXXFLAGS += -Wno-unused-function
#QMAKE_CFLAGS = -Wno-unused-function

HEADERS += csc_rgb.h
SOURCES += test_csc_rgb.c
