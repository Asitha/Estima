######################################################################
# Automatically generated by qmake (2.01a) Tue Mar 20 23:07:25 2012
######################################################################

QT += sql gui xml

TEMPLATE = app
TARGET = 
DEPENDPATH += . addons core gui main storage
INCLUDEPATH += . addons core storage gui main

# Input
HEADERS += addons/printview.h \
           core/boqgenerator.h \
           core/CoreStructures.h \
           core/estimaEnums.h \
           core/unitratecalculator.h \
           core/units.h \
           core/worksheetwidget.h \
           gui/aboutdialog.h \
           gui/addremoveurc.h \
           gui/addresource.h \
           gui/projectpropertiesform.h \
           main/estima.h \
           storage/dbmanager.h \
           storage/item.h \
           storage/mysqlconnector.h \
           storage/sqlitedbmanager.h \
           storage/storagemanager.h \
           storage/xmlrenderer.h \
           storage/xmlwriter.h \
    core/boqdelegate.h \
    gui/resourcedatabrowser.h

FORMS += core/worksheetwidget.ui \
         gui/aboutdialog.ui \
         gui/addremoveurc.ui \
         gui/addresource.ui \
         gui/projectpropertiesform.ui \
         main/mainwindow.ui \
    gui/resourcedatabrowser.ui

SOURCES += addons/printview.cpp \
           core/boqgenerator.cpp \
           core/unitratecalculator.cpp \
           core/units.cpp \
           core/worksheetwidget.cpp \
           gui/aboutdialog.cpp \
           gui/addremoveurc.cpp \
           gui/addresource.cpp \
           gui/projectpropertiesform.cpp \
           main/estima.cpp \
           main/main.cpp \
           storage/dbmanager.cpp \
           storage/mysqlconnector.cpp \
           storage/sqlitedbmanager.cpp \
           storage/storagemanager.cpp \
           storage/xmlrenderer.cpp \
           storage/xmlwriter.cpp \
    core/boqdelegate.cpp \
    gui/resourcedatabrowser.cpp
