#ifndef DAO_ETUDIANT_H
#define DAO_ETUDIANT_H

#include "Model/connexion.h"
#include "Model/Etudiant.h"
#include "Model/ReseauxSocial.h"
#include <QList>
#include <QSqlQuery>

class dao_etudiant
{


public:
    dao_etudiant();
    ~dao_etudiant();

    QList<Etudiant> getAllStudents();
    QList<ReseauxSocial> getAllRS(int idStudent);
    QString deleteStudent(int idStudent);
    QString updateStudent(Etudiant etudiant);
    bool addStudent(Etudiant etudiant);
    Etudiant getEtuById(int);
    QList<Etudiant> getStudentByEnt(int);
private:
    Connexion& connexion;
    QSqlDatabase& db;
};


#endif // DAO_ETUDIANT_H
