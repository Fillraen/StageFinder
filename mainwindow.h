#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Service/dao_etudiant.h"
#include "Service/dao_rs.h"
#include "Service/dao_entreprise.h"
#include "Service/dao_domaine.h"
#include "Service/dao_stage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void navHome();
    void navStudent();
    void navCompagny();
    void navInternship();
    void navSettings();
    void prepareAddStudent();
    void refreshStudentList();
    void refreshEntrepriseList();
    void connexionBDD();
    void prepareAddEntreprise();
    void prepareAddStage();
    void refreshStageList();

private:
    Ui::MainWindow *ui;
    dao_etudiant *daoEtudiant;
    dao_rs *daoRS;
    dao_stage *daoStage;
    dao_entreprise *daoEntreprise;
    dao_domaine *daoDomainne;
    void connectUI();
    QList<Etudiant> getStudents();
    void displayStudent();
    void displayStage();
    void loadBddCreditentials();
    bool checkDatabase();
    void reloadActualConnection();
    QString filter;

    void displayEntreprise();
    QList<Entreprise> getEntreprise();
    int checkDatabaseCallCount;
    QMessageBox reply;
};
#endif // MAINWINDOW_H
