QT       += core gui widgets opengl openglwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Calculator/operands/MathError.cpp \
    "Marching Squares"/edge.cpp \
    Database/calculationstable.cpp \
    Database/plottable2d.cpp \
    Database/plottable3d.cpp \
    Database/recordtable.cpp \
    Database/settingstable.cpp \
    Database/typetable.cpp \
    Plot/abstractplotarea.cpp \
    Plot/iplotbuilder.cpp \
    Plot/plotarea2d.cpp \
    PlotBuilderWorkers/abstractplotcalculator.cpp \
    PlotBuilderWorkers/abstractplotworker.cpp \
    PlotBuilderWorkers/implicitplotworker.cpp \
    PlotBuilderWorkers/plotscheduler.cpp \
    PlotBuilderWorkers/stlmodelbuilder.cpp \
    PlotBuilderWorkers/stlmodelscheduler.cpp \
    PlotBuilderWorkers/xyzplotbuilder.cpp \
    PlotBuilderWorkers/xyzplotcalculator.cpp \
    Settings/calculatorsettingsconfig.cpp \
    Settings/settingswidget.cpp \
    main.cpp \
    mainwindow.cpp \
    "Marching Squares"/marchingsquares.cpp \
    "Marching Squares"/node.cpp \
    "Marching Cubes"/marching_cubes.cpp\
    figures.cpp\
    axis.cpp\
    Plot/plotpointscoordinate.cpp \
    Plot/plotarea3d.cpp\
    "System Widget"/systemwidget.cpp\
    Calculator/operands/Matrix.cpp \
    Calculator/operands/Num.cpp \
    Calculator/operands/Vector.cpp \
    Calculator/operands/operands.cpp \
    Calculator/recursivedescent.cpp \
    History/historywidgetitem.cpp \
    History/historywidget.cpp


HEADERS += \
    "Marching Squares"/edge.h \
    Database/abstracttable.h \
    Database/calculationstable.h \
    Database/plottable2d.h \
    Database/plottable3d.h \
    Database/recordtable.h \
    Database/recordtype.h \
    Database/serializablecontainers.h \
    Database/settingstable.h \
    Database/tablefactory.h \
    Database/typetable.h \
    Plot/abstractplotarea.h \
    Plot/iplotbuilder.h \
    Plot/plotarea2d.h \
    PlotBuilderWorkers/abstractplotcalculator.h \
    PlotBuilderWorkers/abstractplotworker.h \
    PlotBuilderWorkers/implicitplotworker.h \
    PlotBuilderWorkers/plotscheduler.h \
    PlotBuilderWorkers/stlmodelbuilder.h \
    PlotBuilderWorkers/stlmodelscheduler.h \
    PlotBuilderWorkers/xyzplotbuilder.h \
    PlotBuilderWorkers/xyzplotcalculator.h \
    Settings/calculatorsettingsconfig.h \
    Settings/settingswidget.h \
    mainwindow.h \
    "Marching Squares"/marchingsquares.h \
    "Marching Squares"/node.h \
    "Marching Cubes"/marching_cubes.h\
    "Marching Cubes"/types.h\
    figures.h\
    axis.h\
    Plot/plotpointscoordinate.h \
    Plot/plotarea3d.h\
    "System Widget"/systemwidget.h\
    Calculator/operands.h \
    Calculator/recursivedescent.h \
    History/historywidgetitem.h \
    History/historywidget.h \
    Settings/settingsmodel.h

FORMS += \
    Settings/settingswidget.ui \
    mainwindow.ui \
    systemwidget.ui \
    historywidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc
