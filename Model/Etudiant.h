#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <QString>
#include <QList>

#include "Classe.h"
#include "ReseauxSocial.h"

class Etudiant
{

public:
    Etudiant(){

    }

    int id;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    QString site_web;
    Classe classe;
    QList<ReseauxSocial> reseauxSociaux;
};

#endif // ETUDIANT_H

