QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    dialogs.cpp \
    info.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialog.h \
    dialogs.h \
    info.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    dialogs.ui \
    info.ui \
    mainwindow.ui

TRANSLATIONS += \
    kurs_ru_RU.ts\
    kurs_en_US.ts\
    kurs_it_IT.ts


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc

DISTFILES += \
    kurs_en_US.qm \
    kurs_it_IT.ts \
    kurs_ru_RU.ts
