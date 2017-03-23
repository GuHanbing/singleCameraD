INCLUDEPATH += /usr/local/include/opencv \
               /usr/local/include/opencv2 \
               /usr/local/include/eigen3
LIBS    += /usr/local/lib/*.so
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

