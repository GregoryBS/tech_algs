TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    listgraph.cpp \
    matrixgraph.cpp \
    arcgraph.cpp \
    setgraph.cpp

HEADERS += \
    interface.h \
    listgraph.h \
    matrixgraph.h \
    arcgraph.h \
    setgraph.h
