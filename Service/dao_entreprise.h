#ifndef DAO_ENTREPRISE_H
#define DAO_ENTREPRISE_H

#include "Model/connexion.h"
#include "Model/Enterprise.h"
#include "Model/ReseauxSocial.h"
#include "Model/Domaine.h"
#include <QList>
#include <QSqlQuery>
class dao_entreprise
{
public:
    dao_entreprise();

    QList<Entreprise> getAllEntreprises();
    QList<Domaine> getAllDomaine(int idEntreprise);
    QList<ReseauxSocial> getAllRS(int idEntreprise);
    bool addEntreprise(Entreprise);
    QString updateEntreprise(Entreprise);
    QString deleteEntreprise(int);
    Entreprise getEntById(int);
private:
    Connexion& connexion;
    QSqlDatabase& db;
    QMessageBox reply;
};

#endif // DAO_ENTREPRISE_H
