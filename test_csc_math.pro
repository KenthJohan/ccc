TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Wno-unused-function

HEADERS += csc_math.h
SOURCES += test_csc_math.c

