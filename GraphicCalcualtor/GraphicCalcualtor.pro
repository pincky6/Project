QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Calculator/operands/MathError.cpp \
    edge.cpp \
    iplotbuilder.cpp \
    main.cpp \
    mainwindow.cpp \
    marchingsquares.cpp \
    node.cpp \
    plotarea.cpp \
    plotpointscoordinate.cpp \
    systemwidget.cpp\
    Calculator/operands/Matrix.cpp \
    Calculator/operands/Num.cpp \
    Calculator/operands/Vector.cpp \
    Calculator/operands/operands.cpp \
    Calculator/recursivedescent.cpp

HEADERS += \
    edge.h \
    iplotbuilder.h \
    mainwindow.h \
    marchingsquares.h \
    node.h \
    plotarea.h \
    plotpointscoordinate.h \
    systemwidget.h\
    Calculator/operands.h \
    Calculator/recursivedescent.h

FORMS += \
    mainwindow.ui \
    systemwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
