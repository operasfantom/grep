QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  \
        tst_test.cpp \
        ../src/directory_controller.cpp \
        ../src/trigram_storage.cpp \
        ../src/file_iterator.cpp

HEADERS += \
        tst_test.h \
        ../src/directory_controller.h \
        ../src/trigram_storage.h \
        ../src/file_iterator.h

LIBS += -L/../src/
