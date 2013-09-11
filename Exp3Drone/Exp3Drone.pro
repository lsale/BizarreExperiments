APP_NAME = Exp3Drone

CONFIG += qt warn_on cascades10
QT += network

device {
    
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
        INCLUDEPATH=/Users/doturner/Documents/workspace-bbndk/ffmpeg/arm-qnx/debug/include
        LIBS += -L/Users/doturner/Documents/workspace-bbndk/ffmpeg/arm-qnx/debug/lib -L/Users/doturner/Documents/workspace-bbndk/x264/arm-qnx/debug/lib
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
        INCLUDEPATH=/Users/doturner/Documents/workspace-bbndk/ffmpeg/arm-qnx/release/include
        LIBS += -L/Users/doturner/Documents/workspace-bbndk/ffmpeg/arm-qnx/release/lib -L/Users/doturner/Documents/workspace-bbndk/x264/arm-qnx/release/lib
    }
}

simulator {
    
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}

LIBS += -lscreen -lavcodec -lx264 -lavformat -lavutil

include(config.pri)

