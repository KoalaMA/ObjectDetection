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
    drawsides.cpp

HEADERS += \
    tree.h \
    document.h \
    osg.h \
    gettex.h \
    texturetovertex.h \
    datamanagement.h \
    drawsides.h



unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losg

#INCLUDEPATH += $$PWD/../../../../usr/include/osg
#DEPENDPATH += $$PWD/../../../../usr/include/osg

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgWidget

#INCLUDEPATH += $$PWD/../../../../usr/include/osgWidget
#DEPENDPATH += $$PWD/../../../../usr/include/osgWidget

#unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgVolume

#INCLUDEPATH += $$PWD/../../../../usr/include/osgVolume
#DEPENDPATH += $$PWD/../../../../usr/include/osgVolume

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgViewer

#INCLUDEPATH += $$PWD/../../../../usr/include/osgViewer
#DEPENDPATH += $$PWD/../../../../usr/include/osgViewer

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgUtil

#INCLUDEPATH += $$PWD/../../../../usr/include/osgUtil
#DEPENDPATH += $$PWD/../../../../usr/include/osgUtil


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgQt

#INCLUDEPATH += $$PWD/../../../../usr/include/osgQt
#DEPENDPATH += $$PWD/../../../../usr/include/osgQt


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgManipulator

#INCLUDEPATH += $$PWD/../../../../usr/include/osgManipulator
#DEPENDPATH += $$PWD/../../../../usr/include/osgManipulator

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgGA

#INCLUDEPATH += $$PWD/../../../../usr/include/osgGA
#DEPENDPATH += $$PWD/../../../../usr/include/osgGA

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgDB

#INCLUDEPATH += $$PWD/../../../../usr/include/osgDB
#DEPENDPATH += $$PWD/../../../../usr/include/osgDB

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/ -losgFX
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/ -losgFX

else:unix: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -losgFX

#INCLUDEPATH += $$PWD/../../../../usr/include/osgFX
#DEPENDPATH += $$PWD/../../../../usr/include/osgFX

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_core

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_features2d

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_highgui

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_objdetect

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_ts

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_imgproc

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu


unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lopencv_photo
#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lmysqlclient

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu


unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lmysqlcppconn

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
