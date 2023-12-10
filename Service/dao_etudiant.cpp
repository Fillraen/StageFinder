#include "dao_etudiant.h"
#include "Model/connexion.h"
#include "Model/Etudiant.h"
#include "Model/Classe.h"
#include "Model/ReseauxSocial.h"

// Constructeur
dao_etudiant::dao_etudiant() : connexion(Connexion::getInstance()), // Initialisez la référence connexion avec l'instance du singleton
                               db(connexion.getDatabase())           // Initialisez la référence db avec l'objet QSqlDatabase obtenu à partir de connexion
{
}

// Récupérer tous les étudiants
QList<Etudiant> dao_etudiant::getAllStudents()
{
    QList<Etudiant> etudiants;

    QSqlQuery query(db);
    query.exec(
        "SELECT e.* , s.nom , p.annee "
        "FROM etudiant e "
        "JOIN classe c ON e.classe_id = c.id "
        "JOIN section s ON c.section_id = s.id "
        "JOIN promo p ON c.promo_id = p.id "
        "ORDER BY e.id;"
    );

    while (query.next())
    {
        Etudiant etudiant;
        etudiant.id = query.value(0).toInt();
        etudiant.nom = query.value(1).toString();
        etudiant.prenom = query.value(2).toString();
        etudiant.email = query.value(3).toString();
        etudiant.telephone = query.value(4).toString();
        etudiant.site_web = query.value(6).toString();
        Classe classe;
        classe.classe_id = query.value(5).toInt();
        classe.section = query.value(7).toString();
        classe.promo = query.value(8).toString();
        etudiant.classe = classe;

        QList<ReseauxSocial> studentRS;
        studentRS = getAllRS(etudiant.id);

        foreach (ReseauxSocial rs, studentRS)
        {
            etudiant.reseauxSociaux.append(rs);
        };

        etudiants.append(etudiant);
    }
    return etudiants;
}

// Récupérer tous les réseaux sociaux d'un étudiant
QList<ReseauxSocial> dao_etudiant::getAllRS(int idStudent)
{
    QList<ReseauxSocial> reseauxSocials;
    QSqlQuery query(db);
    query.prepare(
        "SELECT rs.id AS reseau_social_id, rs.nom AS reseau_social_nom, ers.identifiant AS identifiant, rs.url AS URL "
        "FROM etudiant et "
        "JOIN etudiant_reseau_social ers ON et.id = ers.etudiant_id "
        "JOIN reseau_social rs ON ers.reseau_social_id = rs.id "
        "WHERE et.id = :idStudent;"
    );
    query.bindValue(":idStudent", idStudent);
    query.exec();

    while (query.next())
    {
        ReseauxSocial rs;
        rs.id = query.value(0).toInt();
        rs.nom = query.value(1).toString();
        rs.pseudo = query.value(2).toString();
        rs.url = query.value(3).toString();

        reseauxSocials.append(rs);
    }

    return reseauxSocials;
}

// Supprimer un étudiant
QString dao_etudiant::deleteStudent(int idStudent)
{
    QSqlQuery query(db);
    query.prepare(
        "DELETE "
        "FROM etudiant "
        "WHERE id = :id;"
    );
    query.bindValue(":id", idStudent);
    QString responceMsg;

    if (query.exec())
    {
        responceMsg = "success";
    }
    else{
        // Si la requête échoue, retournez le message d'erreur
        responceMsg = "Erreur lors de la suppression de l'étudiant:" + query.lastError().text();
    }

    return responceMsg;
}

// Mettre à jour un étudiant
QString dao_etudiant::updateStudent(Etudiant etudiant)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE etudiant "
        "SET nom=:nom, prenom=:prenom, email=:email, telephone=:telephone, site_web=:site_web, classe_id=:classe_id "
        "WHERE id=:id;"
    );
    query.bindValue(":nom", etudiant.nom);
    query.bindValue(":prenom", etudiant.prenom);
    query.bindValue(":email", etudiant.email);
    query.bindValue(":telephone", etudiant.telephone);
    query.bindValue(":site_web", etudiant.site_web);
    query.bindValue(":classe_id", etudiant.classe.classe_id);
    query.bindValue(":id", etudiant.id);

    QString responceMsg;
    // Exécutez la requête SQL et vérifiez si elle réussit
    if (query.exec()) {
        responceMsg = "success";
    } else {
        // Si la requête échoue, retournez le message d'erreur
        responceMsg = "Erreur lors de la mise à jour de l'étudiant: " + query.lastError().text();
    }
    return responceMsg;
}

// Ajouter un étudiant
bool dao_etudiant::addStudent(Etudiant etudiant)
{
    QSqlQuery query(db);

    // Préparer la requête pour insérer un nouvel étudiant
    query.prepare(
        "INSERT INTO etudiant (nom, prenom, email, telephone, site_web, classe_id) "
        "VALUES (:nom, :prenom, :email, :telephone, :site_web, :classe_id)"
    );
    query.bindValue(":nom", etudiant.nom);
    query.bindValue(":prenom", etudiant.prenom);
    query.bindValue(":email", etudiant.email);
    query.bindValue(":telephone", etudiant.telephone);
    query.bindValue(":site_web", etudiant.site_web);
    query.bindValue(":classe_id", etudiant.classe.classe_id);

    // Exécutez la requête et vérifiez si elle a réussi
    if (query.exec())
    {
        // Récupérez l'ID du nouvel étudiant
        int newEtudiantId = query.lastInsertId().toInt();

        // Insérez les réseaux sociaux pour le nouvel étudiant
        foreach (ReseauxSocial reseauSocial, etudiant.reseauxSociaux)
        {
            QSqlQuery rsQuery(db);
            rsQuery.prepare(
                "INSERT INTO etudiant_reseau_social (etudiant_id, reseau_social_id, identifiant) "
                "VALUES (:etudiant_id, :rs_id, :pseudo)"
            );
            rsQuery.bindValue(":etudiant_id", newEtudiantId);
            rsQuery.bindValue(":rs_id", reseauSocial.id);
            rsQuery.bindValue(":pseudo", reseauSocial.pseudo);

            if (!rsQuery.exec())
            {
                // Si une erreur se produit lors de l'insertion des réseaux sociaux, retournez false
                return false;
            }
        }

        // Si tout se passe bien, retournez true
        return true;
    }
    else
    {
        // Si une erreur se produit lors de l'insertion de l'étudiant, retour
        return false;
      }
}


Etudiant dao_etudiant::getEtuById(int id)
{
    Etudiant etudiant;
    QSqlQuery query(db);
    query.prepare(
        "SELECT e.*, s.nom, p.annee "
        "FROM etudiant e "
        "JOIN classe c ON e.classe_id = c.id "
        "JOIN section s ON c.section_id = s.id "
        "JOIN promo p ON c.promo_id = p.id "
        "WHERE e.id = :id_etudiant "
        "ORDER BY e.id;"
    );
    query.bindValue(":id_etudiant", id);
    query.exec();

    while (query.next())
    {
        etudiant.id = query.value(0).toInt();
        etudiant.nom = query.value(1).toString();
        etudiant.prenom = query.value(2).toString();
        etudiant.email = query.value(3).toString();
        etudiant.telephone = query.value(4).toString();
        etudiant.site_web = query.value(6).toString();
        Classe classe;
        classe.classe_id = query.value(5).toInt();
        classe.section = query.value(7).toString();
        classe.promo = query.value(8).toString();
        etudiant.classe = classe;

        QList<ReseauxSocial> studentRS;
        studentRS = getAllRS(etudiant.id);

        foreach (ReseauxSocial rs, studentRS)
        {
            etudiant.reseauxSociaux.append(rs);
        };
    }

    return etudiant;
}

QList<Etudiant> dao_etudiant::getStudentByEnt(int entrepriseId){
    QList<Etudiant> students;

    // Préparer la requête SQL
    QSqlQuery query(db);
    QString sql = "SELECT e.id "
                  "FROM etudiant e "
                  "INNER JOIN stage ON e.id = stage.etudiant_id "
                  "INNER JOIN entreprise ON entreprise.id = stage.entreprise_id "
                  "WHERE entreprise.id = :id";
    query.prepare(sql);
    query.bindValue(":id", entrepriseId);

    if (query.exec()) {
        while (query.next()) {
            Etudiant student;
            student = getEtuById(query.value(0).toInt());
            students.append(student);
        }
    }else {
        // Afficher une erreur si la requête a échoué
        qDebug() << "Erreur lors de l'exécution de la requête SQL : " << query.lastError().text();
    }
    return students;
}



// Destructeur
dao_etudiant::~dao_etudiant()
{
// Ajoutez ici le code nécessaire pour libérer les ressources, si nécessaire
}
