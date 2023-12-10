#include "dao_stage.h"
#include "Model/connexion.h"
#include <QList>
#include <QSqlQuery>

dao_stage::dao_stage() : connexion(Connexion::getInstance()), // Initialisez la référence connexion avec l'instance du singleton
                         db(connexion.getDatabase()),          // Initialisez la référence db
                         daoEntreprise(new dao_entreprise),
                         daoEtudiant(new dao_etudiant)
{

}

// Récupérer la liste de tous les stages
QList<Stage> dao_stage::getAllStages()
{
    QList<Stage> stages;
    QSqlQuery query(db);
    query.exec("SELECT * FROM stage;");

    while (query.next())
    {
        Stage stage;
        stage.id = query.value(0).toInt();

        int idEtu = query.value(1).toInt();
        int idEnt = query.value(2).toInt();
        stage.etudiant = daoEtudiant->getEtuById(idEtu);
        stage.entreprise = daoEntreprise->getEntById(idEnt);

        stage.dateDebut = query.value(3).toDate();
        stage.dateFin = query.value(4).toDate();
        stage.poste = query.value(5).toString();
        stage.desc = query.value(6).toString();

        stages.append(stage);
    }

    return stages;
}

// Ajouter un stage
bool dao_stage::addStage(Stage stage)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO stage (etudiant_id, entreprise_id, date_debut, date_fin, poste, description) VALUES (:id_etudiant, :id_entreprise, :date_debut, :date_fin, :poste, :description);");
    query.bindValue(":id_etudiant", stage.etudiant.id);
    query.bindValue(":id_entreprise", stage.entreprise.id);
    query.bindValue(":date_debut", stage.dateDebut);
    query.bindValue(":date_fin", stage.dateFin);
    query.bindValue(":poste", stage.poste);
    query.bindValue(":description", stage.desc);

    if (query.exec()) {
        return true;
    } else {
        QString errorMsg = query.lastError().text();
        QMessageBox reply;
        reply.setText(errorMsg);
        reply.exec();
        return false;
    }
}

// Supprimer un stage
QString dao_stage::deleteStage(int idStage)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM stage WHERE id = :id_stage;");
    query.bindValue(":id_stage", idStage);

    if (query.exec())
    {
        return "success";
    }
    else
    {
        return query.lastError().text();
    }
}

// Mettre à jour un stage
QString dao_stage::updateStage(Stage stage)
{
    QSqlQuery query(db);
    query.prepare("UPDATE stage SET etudiant_id = :id_etudiant, entreprise_id = :id_entreprise, date_debut = :date_debut, date_fin = :date_fin, poste = :poste, description = :description WHERE id = :id_stage;");
    query.bindValue(":id_etudiant", stage.etudiant.id);
    query.bindValue(":id_entreprise", stage.entreprise.id);
    query.bindValue(":date_debut", stage.dateDebut);
    query.bindValue(":date_fin", stage.dateFin);
    query.bindValue(":poste", stage.poste);
    query.bindValue(":description", stage.desc);
    query.bindValue(":id_stage", stage.id);

    if (query.exec())
    {
        return "success";
    }
    else
    {
        QMessageBox error;
        error.setText(query.lastError().text());
        error.exec();
        return query.lastError().text();
    }
}

// Destructeur
dao_stage::~dao_stage()
{
    // Ajoutez ici le code nécessaire pour libérer les ressources, si nécessaire
    delete daoEntreprise;
    delete daoEtudiant;
}
