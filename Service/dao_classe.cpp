#include "dao_classe.h"
#include "Model/Classe.h"

// Constructeur
dao_classe::dao_classe()
    : connexion(Connexion::getInstance()), // Initialise la référence connexion
      db(connexion.getDatabase())          // Initialise la référence db
{
}

// Récupère la liste de toutes les classes
QList<Classe> dao_classe::getAllClasse()
{
    QList<Classe> classes;
    QSqlQuery query(db);

    // Exécute la requête SQL sur plusieurs lignes pour une meilleure lisibilité
    query.exec("SELECT c.id, p.annee, s.nom "
               "FROM classe c "
               "JOIN promo p ON c.promo_id = p.id "
               "JOIN section s ON c.section_id = s.id;");

    // Parcourt les résultats
    while (query.next())
    {
        Classe classe;
        classe.classe_id = query.value(0).toInt();
        classe.promo = query.value(1).toString();
        classe.section = query.value(2).toString();

        // Ajoute la classe à la liste
        classes.append(classe);
    }

    return classes;
}

// Destructeur
dao_classe::~dao_classe()
{
// Ajoutez ici le code nécessaire pour libérer les ressources, si nécessaire
}
