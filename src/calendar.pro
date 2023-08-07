QT = core widgets

CONFIG += c++20 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        calendarwidget.cpp \
        dateeventdialog.cpp \
        datelabel.cpp \
        main.cpp \
        mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    dateeventdialog.ui

HEADERS += \
    calendarwidget.h \
    dateeventdialog.h \
    datelabel.h \
    mainwindow.h

RESOURCES += \
    resources.qrc
