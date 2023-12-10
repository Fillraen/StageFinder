#ifndef STAGEFORM_H
#define STAGEFORM_H

#include <QWidget>
#include "Service/dao_etudiant.h"
#include "Service/dao_entreprise.h"
#include "Model/Stage.h"
namespace Ui {
class StageForm;
}

class StageForm : public QWidget
{
    Q_OBJECT

public:
    explicit StageForm(QWidget *parent = nullptr);
    ~StageForm();

private:
    Ui::StageForm *ui;
    dao_entreprise *daoEntreprise;
    dao_etudiant *daoEtudiant;
    int idStage;
public :
    void setData(Stage);
    Stage getData();
    void hideDatagrid();
};

#endif // STAGEFORM_H
