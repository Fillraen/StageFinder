#ifndef DAO_CLASSE_H
#define DAO_CLASSE_H

#include "Model/connexion.h"
#include "Model/Classe.h"

class dao_classe
{
private:
    Connexion& connexion;
    QSqlDatabase& db;

    QMessageBox reply;
    QString responceMsg;

public:
    dao_classe();
    ~dao_classe();

    QList<Classe> getAllClasse();
};

#endif // DAO_CLASSE_H
