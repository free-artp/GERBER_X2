QT += core gui opengl widgets

TARGET = Getber2Gcode
TEMPLATE = app

CONFIG += c++14 #console

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    res/resources.qrc \
    qdarkstyle/style.qrc \
    qtreeviewstylesheet/qtreeviewstylesheet.qrc


DISTFILES += \
    qdarkstyle/style.qss

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON = res/toolpath/raster_climb.png
#macx: ICON = resources/icon.icns
win32-msvc* {
    LIBS += -lsetupapi -lAdvapi32
    RC_FILE = myapp.rc
}
win32* {
    LIBS += -lsetupapi -lAdvapi32 -lpsapi
    RC_FILE = myapp.rc
}

HEADERS += \
    application.h \
    clipper/clipper.hpp \
    clipper/myclipper.h \
    edittool.h \
    forms/drillingtoolpathform.h \
    forms/pockettoolpathform.h \
    forms/profiletoolpathform.h \
    forms/toolpathforminterface.h \
    forms/toolpathnameform.h \
    gerber/gerber.h \
    gerber/gerberaperture.h \
    gerber/gerberparser.h \
    gerber/mathparser.h \
    gerberfileholder.h \
    graphicsview/colorsettings.h \
    graphicsview/edid.h \
    graphicsview/mygraphicsscene.h \
    graphicsview/mygraphicsview.h \
    graphicsview/qdruler.h \
    mainwindow.h \
    mytreewidget.h \
    settingsdialog.h \
    tooldatabase/tooldatabase.h \
    toolpathcreator.h \
    tooldatabase/treeitem.h \
    tooldatabase/treemodel.h \
    drillforapertureform.h \
    gerber/gerbergraphicsitem.h


SOURCES += \
    clipper/clipper.cpp \
    clipper/myclipper.cpp \
    edittool.cpp \
    forms/drillingtoolpathform.cpp \
    forms/pockettoolpathform.cpp \
    forms/profiletoolpathform.cpp \
    forms/toolpathforminterface.cpp \
    forms/toolpathnameform.cpp \
    gerber/gerberaperture.cpp \
    gerber/gerberparser.cpp \
    gerber/mathparser.cpp \
    gerberfileholder.cpp \
    graphicsview/colorsettings.cpp \
    graphicsview/edid.cpp \
    graphicsview/mygraphicsscene.cpp \
    graphicsview/mygraphicsview.cpp \
    graphicsview/qdruler.cpp \
    main.cpp \
    mainwindow.cpp \
    mytreewidget.cpp \
    settingsdialog.cpp \
    tooldatabase/tooldatabase.cpp \
    toolpathcreator.cpp \
    tooldatabase/treeitem.cpp \
    tooldatabase/treemodel.cpp \
    drillforapertureform.cpp \
    gerber/gerbergraphicsitem.cpp


