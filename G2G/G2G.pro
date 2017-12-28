QT += core gui opengl widgets

TARGET = Getber2Gcode
TEMPLATE = app

CONFIG += c++14 #console

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    res/resources.qrc \

DISTFILES += \
    qdarkstyle/style.qss \
    mytreewidget.h \
    mytreewidget.cpp \


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
    drillforapertureform.h \
    forms/drillingtoolpathform.h \
    forms/pockettoolpathform.h \
    forms/profiletoolpathform.h \
    forms/toolpathforminterface.h \
    forms/toolpathnameform.h \
    gerber/gerber.h \
    gerber/mathparser.h \
    gerberfileholder.h \
    graphicsview/colorsettings.h \
    graphicsview/edid.h \
    graphicsview/mygraphicsscene.h \
    graphicsview/mygraphicsview.h \
    graphicsview/qdruler.h \
    mainwindow.h \
    settingsdialog.h \
    tooldatabase/tooldatabase.h \
#    tooldatabase/treeitem.h \
#    tooldatabase/treemodel.h \
    toolpathcreator.h \
    tooldatabase/tooldatabase.h \
    tooldatabase/tooledit.h \
    tooldatabase/toolmodel.h \
    tooldatabase/tooltreeview.h \
    tooldatabase/toolitem.h \
    tooldatabase/tool.h \
    filetree/model.h \
    filetree/treeview.h \
    filetree/abstractitem.h \
    filetree/milling.h \
    filetree/fileitem.h \
    filetree/folderitem.h \
    gerber/aperture.h \
    gerber/graphicsitem.h \
    gerber/parser.h


SOURCES += \
    clipper/clipper.cpp \
    clipper/myclipper.cpp \
    drillforapertureform.cpp \
    forms/drillingtoolpathform.cpp \
    forms/pockettoolpathform.cpp \
    forms/profiletoolpathform.cpp \
    forms/toolpathforminterface.cpp \
    forms/toolpathnameform.cpp \
    gerber/mathparser.cpp \
    gerberfileholder.cpp \
    graphicsview/colorsettings.cpp \
    graphicsview/edid.cpp \
    graphicsview/mygraphicsscene.cpp \
    graphicsview/mygraphicsview.cpp \
    graphicsview/qdruler.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    tooldatabase/tooldatabase.cpp \
#    tooldatabase/treeitem.cpp \
#    tooldatabase/treemodel.cpp \
    toolpathcreator.cpp \
    tooldatabase/tooldatabase.cpp \
    tooldatabase/tooledit.cpp \
    tooldatabase/toolmodel.cpp \
    tooldatabase/tooltreeview.cpp \
    tooldatabase/toolitem.cpp \
    tooldatabase/tool.cpp \
    filetree/model.cpp \
    filetree/treeview.cpp \
    filetree/abstractitem.cpp \
    filetree/milling.cpp \
    filetree/fileitem.cpp \
    filetree/folderitem.cpp \
    gerber/parser.cpp \
    gerber/graphicsitem.cpp \
    gerber/aperture.cpp


