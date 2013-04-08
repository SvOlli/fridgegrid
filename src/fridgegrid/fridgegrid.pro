
PROJECT = fridgegrid
TEMPLATE = app
QT += xml

include ( ../buildconfig.pri )

#RESOURCES += Resources/Icons.qrc
#TRANSLATIONS += Translations/otts_de.ts
CODECFORTR += UTF-8

SOURCES += DragLabel.cpp
HEADERS += DragLabel.hpp

SOURCES += DragLabelPropertiesDialog.cpp
HEADERS += DragLabelPropertiesDialog.hpp

SOURCES += DragWidget.cpp
HEADERS += DragWidget.hpp

SOURCES += MainWindow.cpp
HEADERS += MainWindow.hpp

SOURCES += main.cpp

