#include "dao_rs.h"
#include "Model/ReseauxSocial.h"

// Constructeur
dao_rs::dao_rs() : connexion(Connexion::getInstance()), // Initialisez la référence connexion avec l'instance du singleton
                   db(connexion.getDatabase())          // Initialisez la référence db avec l'objet QSqlDatabase obtenu à partir de connexion
{
}

// Récupérer tous les réseaux sociaux
QList<ReseauxSocial> dao_rs::getAllSocialNetwork()
{
    QList<ReseauxSocial> socialNetworks;

    QSqlQuery query(db);
    query.exec("SELECT * FROM reseau_social;");
    while (query.next())
    {
        ReseauxSocial socialNetwork;
        socialNetwork.id = query.value(0).toInt();
        socialNetwork.nom = query.value(1).toString();
        socialNetwork.pseudo = query.value(2).toString();

        socialNetworks.append(socialNetwork);
    }
    return socialNetworks;
}

// Supprimer les réseaux sociaux d'un étudiant
QString dao_rs::deleteRsStudent(int idStudent)
{

    QSqlQuery query(db);
    query.prepare("DELETE FROM etudiant_reseau_social WHERE etudiant_id = :idStudent;");
    query.bindValue(":idStudent", idStudent);
    QString responceMsg;
    // Exécutez la requête SQL et vérifiez si elle réussit
    if (query.exec())
    {
        responceMsg = "success";
    }
    else
    {
        // Si la requête échoue, retournez le message d'erreur
        responceMsg = "Erreur lors de la suppression de l'étudiant:" + query.lastError().text();
    }
    return responceMsg;
}

// Supprimer les réseaux sociaux d'une entreprise
QString dao_rs::deleteRsEntreprise(int idEntreprise)
{

    QSqlQuery query(db);
    query.prepare("DELETE FROM entreprise_reseau_social WHERE entreprise_id = :idEntreprise;");
    query.bindValue(":idEntreprise", idEntreprise);
    QString responceMsg;
    // Exécutez la requête SQL et vérifiez si elle réussit
    if (query.exec())
    {
        responceMsg = "success";
    }
    else
    {
        // Si la requête échoue, retournez le message d'erreur
        responceMsg = "Erreur lors de la suppression de l'étudiant:" + query.lastError().text();
    }
    return responceMsg;
}

// Ajouter une liste de réseaux sociaux pour un étudiant
QString dao_rs::addListRSStudent(QList<ReseauxSocial> ReseauxSociaux, int idStudent)
{
    QSqlQuery query(db);
    QString responseMsg;

    foreach (ReseauxSocial rs, ReseauxSociaux)
    {
        query.prepare("INSERT INTO etudiant_reseau_social (etudiant_id, reseau_social_id, identifiant) VALUES (:etudiant_id, :rs_id, :pseudo);");
        query.bindValue(":etudiant_id", idStudent);
        query.bindValue(":rs_id", rs.id);
        query.bindValue(":pseudo", rs.pseudo);
        if (!query.exec())
        {
            responseMsg = "Erreur lors de l'insertion des réseaux sociaux : " + query.lastError().text();

            return responseMsg;
        }
    }
    responseMsg = "success";
    return responseMsg;
}

// Ajouter une liste de réseaux sociaux pour une entreprise
QString dao_rs::addListRSEntreprise(QList<ReseauxSocial> ReseauxSociaux, int idEntreprise)
{
    QSqlQuery query(db);
    QString responseMsg;
    foreach (ReseauxSocial rs, ReseauxSociaux)
    {
        query.prepare("INSERT INTO entreprise_reseau_social (entreprise_id, reseau_social_id, identifiant) VALUES (:entreprise_id, :rs_id, :pseudo);");
        query.bindValue(":entreprise_id", idEntreprise);
        query.bindValue(":rs_id", rs.id);
        query.bindValue(":pseudo", rs.pseudo);
        if (!query.exec())
        {
            responseMsg = "Erreur lors de l'insertion des réseaux sociaux : " + query.lastError().text();
            return responseMsg;
        }
    }
    responseMsg = "success ajout";

    return responseMsg;
}

// Obtenir l'ID d'un réseau social à partir de son nom
int dao_rs::getRSId(QString nom)
{
    QList<ReseauxSocial> socialNetworks = getAllSocialNetwork();
    int rsId = -1;
    foreach (ReseauxSocial rs, socialNetworks)
    {
        if (rs.nom == nom)
        {
            rsId = rs.id;
            break;
        }
    }
    return rsId;
}
// Destructeur
dao_rs::~dao_rs()
{
// Ajoutez ici le code nécessaire pour libérer les ressources, si nécessaire
}
