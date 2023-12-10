#ifndef ENTREPRISE_H
#define ENTREPRISE_H

#include <QString>
#include <QList>

#include "Domaine.h"
#include "ReseauxSocial.h"

class Entreprise{

public:
    Entreprise(){

    }

    int id;
    QString nom;
    QString adresse;
    QString ville;
    QString telephone;
    QString email;
    QString siteWeb;
    QList<Domaine> domaine;
    QList<ReseauxSocial> reseauxSociaux;

};

#endif // ENTREPRISE_H
