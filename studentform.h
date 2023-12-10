#ifndef STUDENTFORM_H
#define STUDENTFORM_H

#include <QWidget>
#include "Service/dao_rs.h"
#include "Service/dao_classe.h"
#include "Model/Etudiant.h"
namespace Ui {
class StudentForm;
}

class StudentForm : public QWidget
{
    Q_OBJECT

public:
    explicit StudentForm(QWidget *parent = nullptr);
    ~StudentForm();

private slots:
    void showActionRS();
    void prepareEditRS();
    void prepareAddRS();
    void deleteRS();
    void confirmActionRS();

public slots:
    void hideActionRS();

private:
    QString currentAction;
    Ui::StudentForm *ui;
    dao_rs *daoRS;
    dao_classe *daoClasse;
    int idStudent;

public:
    void setData(Etudiant);
    Etudiant getData();
};

#endif // STUDENTFORM_H
