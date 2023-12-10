#ifndef DAO_DOMAINE_H
#define DAO_DOMAINE_H

#include "Model/Domaine.h"
#include "Model/connexion.h"

#include <QList>
#include <QSqlQuery>
class dao_domaine
{
public:
    dao_domaine();
    ~dao_domaine();

    QList<Domaine> getAllDomains();
    QString deleteDomaineEntreprise(int);
    QString addListDomaine(QList<Domaine>, int);

private:
    Connexion& connexion;
    QSqlDatabase& db;
};

#endif // DAO_DOMAINE_H
