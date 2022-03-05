QT += multimedia

HEADERS += \
    $$PWD/audioread.h \
    $$PWD/audiowrite.h \
    $$PWD/world.h

SOURCES += \
    $$PWD/audioread.cpp \
    $$PWD/audiowrite.cpp

LIBS += $$PWD/speex/lib/libspeex.lib
