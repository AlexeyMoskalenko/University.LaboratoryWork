include(gtest_dependency.pri)

INCLUDEPATH += ../CodeForTesting

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    ../CodeForTesting/IKeypad.h \
    ../CodeForTesting/ILatch.h \
    ../CodeForTesting/lockcontroller.h \
    tst_testsuit.h

SOURCES += \
        ../CodeForTesting/lockcontroller.cpp \
        main.cpp
