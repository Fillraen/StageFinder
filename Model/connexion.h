#ifndef CONNEXION_H
#define CONNEXION_H
#include <QtSql>​
#include <QMessageBox>
#include <QDebug>

class Connexion
{
public:
    // Cette méthode statique renvoie une référence à l'instance unique de la classe Connexion.
    // Elle crée l'instance unique la première fois qu'elle est appelée et renvoie simplement
    // une référence à cette instance lors des appels ultérieurs.
    static Connexion& getInstance();

    // Cette méthode renvoie une référence à l'objet QSqlDatabase qui est utilisé pour interagir
    // avec la base de données.
    QSqlDatabase& getDatabase();
    bool isConnected();
    void reloadActualConnection();
    QSqlError getLastError();
private:
    Connexion();
    ~Connexion();

    // Le constructeur de copie est supprimé pour éviter la copie d'instances de la classe Connexion.
    // Cela garantit qu'il n'y a qu'une seule instance de la classe pour toute l'application.
    Connexion(const Connexion&) = delete;

    // L'opérateur d'affectation est supprimé pour empêcher l'affectation d'une instance de la classe
    // Connexion à une autre. Cela garantit également qu'il n'y a qu'une seule instance de la classe
    // pour toute l'application.
    Connexion& operator=(const Connexion&) = delete;

    QSqlDatabase db;
};

#endif // CONNEXION_H
