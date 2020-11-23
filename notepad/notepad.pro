QT       += core gui xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    codeeditor.cpp \
    highlighters.cpp \
    main.cpp \
    notepad.cpp \
    notepadlogic.cpp \
    searchdialog.cpp

HEADERS += \
    codeeditor.h \
    highlighters.h \
    notepad.h \
    notepadlogic.h \
    searchdialog.h

FORMS += \
    notepad.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
