DEFINES += CUTELOGGER_LIBRARY

INCLUDEPATH += $$PWD

SOURCES += $$PWD/Logger.cpp \
           $$PWD/AbstractAppender.cpp \
           $$PWD/AbstractStringAppender.cpp \
           $$PWD/ConsoleAppender.cpp \
           $$PWD/FileAppender.cpp \
           $$PWD/RollingFileAppender.cpp

HEADERS += $$PWD/Logger.h \
           $$PWD/CuteLogger_global.h \
           $$PWD/AbstractAppender.h \
           $$PWD/AbstractStringAppender.h \
           $$PWD/ConsoleAppender.h \
           $$PWD/FileAppender.h \
           $$PWD/RollingFileAppender.h
