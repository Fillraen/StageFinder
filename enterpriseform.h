#ifndef ENTERPRISEFORM_H
#define ENTERPRISEFORM_H

#include "Model/Enterprise.h"
#include "Service/dao_domaine.h"
#include "Service/dao_rs.h"

#include <QWidget>

namespace Ui {
class EnterpriseForm;
}

class EnterpriseForm : public QWidget
{
    Q_OBJECT

public:
    explicit EnterpriseForm(QWidget *parent = nullptr);
    ~EnterpriseForm();

private slots:
    void showActionRS();

    void prepareEditRS();
    void prepareAddRS();
    void deleteRS();
    void confirmActionRS();
    void addDomaine();
    void deleteDomaine();
public slots:
    void hideActionRS();

private:
    QString currentAction;
    Ui::EnterpriseForm *ui;
    dao_rs *daoRS;
    dao_domaine *daoDomaine;
    int idEntreprise;


public:
    Entreprise getData();
    void setData(Entreprise);
};

#endif // ENTERPRISEFORM_H
