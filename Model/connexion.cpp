#include "Model/connexion.h"

// Récupère l'instance unique
Connexion& Connexion::getInstance()
{
    static Connexion instance;
    return instance;
}

// Récupère la référence à la base de données
QSqlDatabase& Connexion::getDatabase()
{
    return db;
}

// Constructeur
Connexion::Connexion()
{
    // Charge les paramètres de connexion
    QSettings settings("get", "getBdd");

    // Configure la connexion
    db = QSqlDatabase::addDatabase("QMYSQL", "BDDstagefinder");
    db.setHostName(settings.value("HostName").toString());
    db.setUserName(settings.value("UserName").toString());
    db.setPassword(settings.value("Password").toString());
    db.setDatabaseName(settings.value("DatabaseName").toString());
    db.setPort(settings.value("Port").toInt());

    // Tente d'ouvrir la connexion
    if (!db.open())
    {
        // Affiche l'erreur si échec
        qDebug() << "Erreur de connexion:" << db.lastError().text();
    }
}

bool Connexion::isConnected() {
    return db.isOpen();
}

// Connexion.cpp
void Connexion::reloadActualConnection() {
    // Ferme la connexion actuelle
    if (db.isOpen()) {
        db.close();
    }

    // Charge les paramètres de connexion à partir des QSettings
    QSettings settings("get", "getBdd");

    // Configure la connexion avec les nouvelles valeurs
    db.setHostName(settings.value("HostName").toString());
    db.setUserName(settings.value("UserName").toString());
    db.setPassword(settings.value("Password").toString());
    db.setDatabaseName(settings.value("DatabaseName").toString());
    db.setPort(settings.value("Port").toInt());

    // Tente d'ouvrir la connexion
    if (!db.open()) {
        // Affiche l'erreur si échec
        qDebug() << "Erreur de connexion:" << db.lastError().text();
    }
}

QSqlError Connexion::getLastError() {
    return db.lastError();
}

// Destructeur
Connexion::~Connexion()
{
    // Aucune action à effectuer
}
