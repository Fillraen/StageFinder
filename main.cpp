#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Création d'un objet QApplication pour gérer l'application
    QApplication a(argc, argv);

    // Création d'un objet QSettings pour stocker les données de configuration
    QSettings settings("get", "getBdd");

    // Vérification que les valeurs par défaut ont été enregistrées, sinon les enregistrer
    if(settings.value("firstRegister").toString() != "true"){
        settings.setValue("HostName", "localhost");
        settings.setValue("UserName", "root");
        settings.setValue("Password", "");
        settings.setValue("DatabaseName", "nt_stagefinder");
        settings.setValue("Port", "3306");
        settings.setValue("firstRegister", "true");
    }

    // Création d'un objet MainWindow et affichage de la fenêtre principale
    MainWindow w;
    w.show();

    // Exécution de la boucle d'événements de l'application
    return a.exec();
}
