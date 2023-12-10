#ifndef DAO_RS_H
#define DAO_RS_H

#include "Model/connexion.h"
#include "Model/ReseauxSocial.h"

class dao_rs
{
private:
    Connexion& connexion;
    QSqlDatabase& db;

    QMessageBox reply;
    QString responceMsg;

public:
    dao_rs();
    ~dao_rs();

    QList<ReseauxSocial> getAllSocialNetwork();
    QString deleteRsStudent(int idStudent);
    QString deleteRsEntreprise(int idEntreprise);
    QString addListRSStudent(QList<ReseauxSocial>, int);
    QString addListRSEntreprise(QList<ReseauxSocial>, int);
    int getRSId(QString nom);
};

#endif // DAO_RS_H
