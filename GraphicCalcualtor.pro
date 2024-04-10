QT       += core gui widgets opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Calculator/operands/MathError.cpp \
    "Marching Squares"/edge.cpp \
    Plot/iplotbuilder.cpp \
    PlotBuilderWorkers/abstractplotbuilder.cpp \
    PlotBuilderWorkers/implicitworkerbuilder.cpp \
    main.cpp \
    mainwindow.cpp \
    "Marching Squares"/marchingsquares.cpp \
    "Marching Squares"/node.cpp \
    Plot/plotarea.cpp \
    Plot/plotpointscoordinate.cpp \
    "System Widget"/systemwidget.cpp\
    Calculator/operands/Matrix.cpp \
    Calculator/operands/Num.cpp \
    Calculator/operands/Vector.cpp \
    Calculator/operands/operands.cpp \
    Calculator/recursivedescent.cpp

HEADERS += \
    "Marching Squares"/edge.h \
    Plot/iplotbuilder.h \
    PlotBuilderWorkers/abstractplotbuilder.h \
    PlotBuilderWorkers/implicitworkerbuilder.h \
    mainwindow.h \
    "Marching Squares"/marchingsquares.h \
    "Marching Squares"/node.h \
    Plot/plotarea.h \
    Plot/plotpointscoordinate.h \
    "System Widget"/systemwidget.h\
    Calculator/operands.h \
    Calculator/recursivedescent.h

FORMS += \
    mainwindow.ui \
    systemwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
