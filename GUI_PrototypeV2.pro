# Add core and GUI modules of Qt
QT += core gui

# Add the OpenGL module for OpenGL functionality
QT += opengl

# Include the widgets module if using Qt 5 or higher
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Include the openglwidgets module for QOpenGLWidget
QT += openglwidgets

# Set the C++17 standard for compiling code
CONFIG += c++17

# Link against the OpenGL32 library on Windows
LIBS += -lopengl32

# Uncomment the following line to disable deprecated APIs before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Specify the source files to be compiled
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    oglutilities.cpp \
    oglwidgetspc.cpp

# Specify the header files
HEADERS += \
    SPC_SF.h \
    mainwindow.h \
    myglobals.h \
    oglutilities.h \
    oglwidgetspc.h \
    point2.h \
    turtleg.h \
    vec2.h

# Specify the form files for processing
FORMS += \
    mainwindow.ui

# Default rules for deployment.
# Set the target installation path based on the platform
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../../source/repos/AdvancedPracOpenGL/AdvancedPracOpenGL/breast-cancer-wisconsin.txt

RESOURCES += \
    resources.qrc
