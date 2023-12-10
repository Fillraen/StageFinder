#include "dao_entreprise.h"
#include "Model/Enterprise.h"
#include "Model/connexion.h"
#include "Model/ReseauxSocial.h"
#include "Model/Domaine.h"

// Constructeur
dao_entreprise::dao_entreprise(): connexion(Connexion::getInstance()), // Initialise la référence connexion
                                  db(connexion.getDatabase()) // Initialise la référence db
{

}

// Récupère toutes les entreprises
QList < Entreprise > dao_entreprise::getAllEntreprises() {
  QList < Entreprise > entreprises;

  QSqlQuery query(db);
  query.exec(
    "SELECT * "
    "FROM entreprise;"
  );

  // Parcourt les résultats de la requête
  while (query.next()) {
    Entreprise entreprise;
    entreprise.id = query.value(0).toInt();
    entreprise.nom = query.value(1).toString();
    entreprise.adresse = query.value(2).toString();
    entreprise.ville = query.value(3).toString();
    entreprise.telephone = query.value(4).toString();
    entreprise.email = query.value(5).toString();
    entreprise.siteWeb = query.value(6).toString();

    // Récupère les domaines liés à l'entreprise
    QList < Domaine > entrepriseDomaine = getAllDomaine(entreprise.id);
    foreach(Domaine domaine, entrepriseDomaine) {
      entreprise.domaine.append(domaine);
    }

    // Récupère les réseaux sociaux liés à l'entreprise
    QList < ReseauxSocial > entrepriseRS = getAllRS(entreprise.id);
    foreach(ReseauxSocial rs, entrepriseRS) {
      entreprise.reseauxSociaux.append(rs);
    }

    entreprises.append(entreprise);
  }
  return entreprises;
}

// Récupère tous les réseaux sociaux liés à une entreprise
QList < ReseauxSocial > dao_entreprise::getAllRS(int idStudent) {
  QList < ReseauxSocial > reseauxSocials;
  QSqlQuery query(db);
  query.prepare(
    "SELECT rs.id, rs.nom, ers.identifiant "
    "FROM entreprise en "
    "JOIN entreprise_reseau_social ers ON en.id = ers.entreprise_id "
    "JOIN reseau_social rs ON ers.reseau_social_id = rs.id "
    "WHERE en.id = :idStudent;"
  );
  query.bindValue(":idStudent", idStudent);
  query.exec();

  // Parcourt les résultats de la requête
  while (query.next()) {
    ReseauxSocial rs;
    rs.id = query.value(0).toInt();
    rs.nom = query.value(1).toString();
    rs.pseudo = query.value(2).toString();

    reseauxSocials.append(rs);
  }

  return reseauxSocials;
}

// Récupère tous les domaines liés à une entreprise
QList < Domaine > dao_entreprise::getAllDomaine(int idStudent) {
  QList < Domaine > domaines;
  QSqlQuery query(db);
  query.prepare(
    "SELECT d.id, d.nom "
    "FROM entreprise e "
    "JOIN entreprise_domaine ed ON e.id = ed.entreprise_id "
    "JOIN domaine d ON ed.domaine_id = d.id "
    "WHERE e.id = :idStudent;"
  );
  query.bindValue(":idStudent", idStudent);
  query.exec();

  // Parcourt les résultats de la requête
  while (query.next()) {
    Domaine domaine;
    domaine.id = query.value(0).toInt();
    domaine.nom = query.value(1).toString();

    domaines.append(domaine);
  }

  return domaines;
}

// Supprime une entreprise
QString dao_entreprise::deleteEntreprise(int idEntreprise) {
  QSqlQuery query(db);

  // Prépare la requête SQL DELETE avec un paramètre pour l'ID de l'entreprise
  query.prepare(
    "DELETE FROM entreprise "
    "WHERE id = :id;"
  );
  query.bindValue(":id", idEntreprise);

  QString responceMsg;
  // Exécute la requête SQL et vérifie si elle réussit
  if (query.exec()) {
    responceMsg = "success";
  } else {
    // Si la requête échoue, retourne le message d'erreur
    responceMsg = "Erreur lors de la suppression de l'entreprise:" + query.lastError().text();
  }

  return responceMsg;
}

// Ajoute une entreprise
bool dao_entreprise::addEntreprise(Entreprise entreprise) {

  QSqlQuery query(db);

  // Prépare la requête pour insérer une nouvelle entreprise
  query.prepare(
    "INSERT INTO entreprise (nom, adresse, ville, telephone, email, site_web) "
    "VALUES (:nom, :adresse, :ville, :telephone, :email, :site_web);"
  );
  query.bindValue(":nom", entreprise.nom);
  query.bindValue(":adresse", entreprise.adresse);
  query.bindValue(":ville", entreprise.ville);
  query.bindValue(":telephone", entreprise.telephone);
  query.bindValue(":email", entreprise.email);
  query.bindValue(":site_web", entreprise.siteWeb);

  // Exécute la requête et vérifie si elle a réussi
  if (query.exec()) {
    // Récupère l'ID de la nouvelle entreprise
    int newEntrepriseId = query.lastInsertId().toInt();

    // Insère les réseaux sociaux pour la nouvelle entreprise
    foreach(ReseauxSocial reseauSocial, entreprise.reseauxSociaux) {
      QSqlQuery rsQuery(db);
      rsQuery.prepare(
        "INSERT INTO entreprise_reseau_social (entreprise_id, reseau_social_id, identifiant) "
        "VALUES (:entreprise_id, :rs_id, :pseudo);"
      );
      rsQuery.bindValue(":entreprise_id", newEntrepriseId);
      rsQuery.bindValue(":rs_id", reseauSocial.id);
      rsQuery.bindValue(":pseudo", reseauSocial.pseudo);

      if (!rsQuery.exec()) {
        // Si une erreur se produit lors de l'insertion des réseaux sociaux, retourne false
        return false;
      }
    }

    // Insère les domaines pour la nouvelle entreprise
    foreach(Domaine domaine, entreprise.domaine) {
      QSqlQuery domaineQuery(db);
      domaineQuery.prepare(
        "INSERT INTO entreprise_domaine (entreprise_id, domaine_id) "
        "VALUES (:entreprise_id, :domaine_id);"
      );
      domaineQuery.bindValue(":entreprise_id", newEntrepriseId);
      domaineQuery.bindValue(":domaine_id", domaine.id);

      if (!domaineQuery.exec()) {
        // Si une erreur se produit lors de l'insertion des domaines, retourne false
        return false;
      }
    }
    // Si tout se passe bien, retourne true
    return true;
  } else {
    // Si une erreur se produit lors de l'insertion de l'entreprise, retourne false
    return false;
  }
}

// Met à jour une entreprise
QString dao_entreprise::updateEntreprise(Entreprise entreprise) {
  QSqlQuery query(db);
  // Prépare la requête pour mettre à jour une entreprise
  query.prepare(
    "UPDATE entreprise "
    "SET nom=:nom, adresse=:adresse, ville=:ville, telephone=:telephone, email=:email, site_web=:site_web "
    "WHERE id=:id;"
  );
  query.bindValue(":nom", entreprise.nom);
  query.bindValue(":adresse", entreprise.adresse);
  query.bindValue(":ville", entreprise.ville);
  query.bindValue(":telephone", entreprise.telephone);
  query.bindValue(":email", entreprise.email);
  query.bindValue(":site_web", entreprise.siteWeb);
  query.bindValue(":id", entreprise.id);

  QString responceMsg;
  // Exécute la requête SQL et vérifie si elle réussit
  if (query.exec()) {
    responceMsg = "success";
  } else {
    // Si la requête échoue, retourne le message d'erreur
    responceMsg = "Erreur lors de la mise à jour de l'entreprise: " + query.lastError().text();
  }

  return responceMsg;
}

// Récupère une entreprise par son ID
Entreprise dao_entreprise::getEntById(int id) {
  Entreprise entreprise;
  QSqlQuery query(db);

  // Prépare la requête pour récupérer une entreprise par son ID
  query.prepare(
    "SELECT * "
    "FROM entreprise "
    "WHERE id = :id_entreprise;"
  );
  query.bindValue(":id_entreprise", id);
  query.exec();

  // Parcourt les résultats de la requête
  while (query.next()) {
    entreprise.id = query.value(0).toInt();
    entreprise.nom = query.value(1).toString();
    entreprise.adresse = query.value(2).toString();
    entreprise.ville = query.value(3).toString();
    entreprise.telephone = query.value(4).toString();
    entreprise.email = query.value(5).toString();
    entreprise.siteWeb = query.value(6).toString();

    // Récupère les domaines liés à l'entreprise
    QList < Domaine > entrepriseDomaine;
    entrepriseDomaine = getAllDomaine(entreprise.id);

    foreach(Domaine domaine, entrepriseDomaine) {
      entreprise.domaine.append(domaine);
    }

    // Récupère les réseaux sociaux liés à l'entreprise
    QList < ReseauxSocial > entrepriseRS;
    entrepriseRS = getAllRS(entreprise.id);

    foreach(ReseauxSocial rs, entrepriseRS) {
      entreprise.reseauxSociaux.append(rs);
    }
  }

  return entreprise;
}

