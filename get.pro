QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Model/connexion.cpp \
    Service/dao_classe.cpp \
    Service/dao_domaine.cpp \
    Service/dao_entreprise.cpp \
    Service/dao_etudiant.cpp \
    Service/dao_rs.cpp \
    Service/dao_stage.cpp \
    enterpriseform.cpp \
    main.cpp \
    mainwindow.cpp \
    stageform.cpp \
    studentform.cpp

HEADERS += \
    Model/Classe.h \
    Model/Domaine.h \
    Model/Enterprise.h \
    Model/Etudiant.h \
    Model/ReseauxSocial.h \
    Model/Stage.h \
    Model/connexion.h \
    Service/dao_classe.h \
    Service/dao_domaine.h \
    Service/dao_entreprise.h \
    Service/dao_etudiant.h \
    Service/dao_rs.h \
    Service/dao_stage.h \
    enterpriseform.h \
    mainwindow.h \
    stageform.h \
    studentform.h

FORMS += \
    enterpriseform.ui \
    mainwindow.ui \
    stageform.ui \
    studentform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
