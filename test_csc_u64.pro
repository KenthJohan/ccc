TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-function

HEADERS += csc_u32.h
HEADERS += csc_u64.h
SOURCES += test_csc_u64.c
