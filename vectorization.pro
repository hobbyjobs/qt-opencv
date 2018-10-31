TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += -I/usr/local/include/opencv\
               -I/usr/local/include
LIBS += -L/usr/local/lib\
        -lopencv_highgui\
        -lopencv_imgcodecs\
        -lopencv_imgproc\
        -lopencv_core

SOURCES += \
        main.cpp \
    vectorize.cpp

HEADERS += \
    vectorize.h
