#ifndef STAGE_H
#define STAGE_H

#include <QString>
#include <QDate>

#include "Etudiant.h"
#include "Enterprise.h"

class Stage{
    public:
        Stage(){}
        int id;
        Etudiant etudiant;
        Entreprise entreprise;
        QDate dateDebut;
        QDate dateFin;
        QString poste;
        QString desc;
};

#endif // STAGE_H
