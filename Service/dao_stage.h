#ifndef DAO_STAGE_H
#define DAO_STAGE_H

#include "Model/Stage.h"
#include "dao_etudiant.h"
#include "dao_entreprise.h"

#include <QList>
#include <QSqlQuery>

class dao_stage
{
public:
    dao_stage();
    ~dao_stage();

    QList<Stage> getAllStages();
    bool addStage(Stage);
    QString deleteStage(int idStage);
    QString updateStage(Stage);

private:
    Connexion& connexion;
    QSqlDatabase& db;
    QMessageBox reply;
    dao_entreprise *daoEntreprise;
    dao_etudiant *daoEtudiant;
};

#endif // DAO_STAGE_H
