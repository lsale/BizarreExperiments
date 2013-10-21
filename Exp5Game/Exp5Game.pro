APP_NAME = Exp5Game

CONFIG += qt warn_on cascades10
INCLUDEPATH += ../external-libs/HttpLib

LIBS += -L../external-libs/HttpLib/lib -lHttpLib -lbbdevice

include(config.pri)
