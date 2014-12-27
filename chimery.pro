QT += core gui widgets multimedia svg

TARGET = Chimery
TEMPLATE = app
ICON = $$PWD/art/chimery.icns
RESOURCES = chimery.qrc

SOURCES += main.cpp chimerymainwin.cpp chimeryaboutwin.cpp chimerymenus.cpp

HEADERS += chimerymainwin.h chimeryaboutwin.h chimerymenus.h
