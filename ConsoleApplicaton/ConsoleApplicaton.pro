TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    document.cpp \
    tree.cpp \
    osg.cpp \
    gettex.cpp \
    texturetovertex.cpp \
    datamanagement.cpp \
    drawpoints.cpp \
    rebuildtextimage.cpp

HEADERS += \
    tree.h \
    document.h \
    osg.h \
    gettex.h \
    texturetovertex.h \
    datamanagement.h \
    drawpoints.h \
    rebuildtextimage.h \
    pointpositionjudge.h


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losg


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgWidget


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgViewer


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgUtil


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgQt


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgManipulator


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgGA


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgDB


unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lmysqlclient


unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lmysqlcppconn





unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_core

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_features2d

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_highgui

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_objdetect

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_ts

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_imgproc

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_photo

