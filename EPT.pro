######################################################################
# Automatically generated by qmake (2.01a) Sat Dec 28 00:13:16 2013
######################################################################

TEMPLATE = app
TARGET = ept
DEPENDPATH += .
INCLUDEPATH += . ../Dodge/include
QMAKE_CXXFLAGS = -std=c++0x -DDEBUG -g
QMAKE_LIBDIR = ../Dodge/lib
LIBS += -lDodge

# Input
HEADERS += QtApplication.hpp QtMainWindow.hpp
SOURCES += main.cpp QtApplication.cpp QtMainWindow.cpp