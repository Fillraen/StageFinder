#include "dao_domaine.h"
#include "Model/Domaine.h"

// Constructeur
dao_domaine::dao_domaine()
    : connexion(Connexion::getInstance()), // Initialise la référence connexion
      db(connexion.getDatabase())          // Initialise la référence db
{
}

// Récupère la liste de tous les domaines
QList<Domaine> dao_domaine::getAllDomains()
{
    QList<Domaine> domaines;
    QSqlQuery query(db);
    query.exec("SELECT * FROM domaine;");

    // Parcourt les résultats
    while (query.next())
    {
        Domaine domaine;
        domaine.id = query.value(0).toInt();
        domaine.nom = query.value(1).toString();

        // Ajoute le domaine à la liste
        domaines.append(domaine);
    }

    return domaines;
}

// Supprime les domaines liés à une entreprise
QString dao_domaine::deleteDomaineEntreprise(int idEntreprise)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM entreprise_domaine WHERE entreprise_id = :idEntreprise;");
    query.bindValue(":idEntreprise", idEntreprise);
    QString responseMsg;

    // Exécute la requête SQL et vérifie si elle réussit
    if (query.exec())
    {
        responseMsg = "success";
    }
    else
    {
        // Si la requête échoue, retourne le message d'erreur
        responseMsg = "Erreur lors de la suppression de l'étudiant:" + query.lastError().text();
    }

    return responseMsg;
}

// Ajoute une liste de domaines à une entreprise
QString dao_domaine::addListDomaine(QList<Domaine> domaines, int idEntreprise)
{
    QSqlQuery query(db);
    QString responseMsg;

    // Parcourt la liste des domaines
    foreach (Domaine domaine, domaines)
    {
        query.prepare("INSERT INTO entreprise_domaine (entreprise_id, domaine_id) VALUES (:entreprise_id, :domaine_id);");
        query.bindValue(":entreprise_id", idEntreprise);
        query.bindValue(":domaine_id", domaine.id);

        // Exécute la requête SQL et vérifie si elle réussit
        if (!query.exec())
        {
            responseMsg = "Erreur lors de l'insertion des réseaux sociaux : " + query.lastError().text();
            return responseMsg;
        }
    }

    responseMsg = "success";
    return responseMsg;
}

// Destructeur
dao_domaine::~dao_domaine()
{
// Ajoutez ici le code nécessaire pour libérer les ressources, si nécessaire
}
