#include "stageform.h"
#include "ui_stageform.h"

StageForm::StageForm(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::StageForm)
    , daoEntreprise(new dao_entreprise)
    , daoEtudiant(new dao_etudiant)
{
    ui->setupUi(this);

    // Configurez les colonnes et les en-têtes du tableau
    ui->EditStudentStage->setColumnCount(3);
    ui->EditStudentStage->setHorizontalHeaderLabels(QStringList()<< "Id" << "Nom" << "Prénom");
    // Ajustez automatiquement la largeur des colonnes en fonction du contenu
    ui->EditStudentStage->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Configurez les colonnes et les en-têtes du tableau
    ui->EditEntrepriseStage->setColumnCount(3);
    ui->EditEntrepriseStage->setHorizontalHeaderLabels(QStringList()<< "Id" << "Nom" << "Ville");
    // Ajustez automatiquement la largeur des colonnes en fonction du contenu
    ui->EditEntrepriseStage->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Effacer toutes les lignes existantes dans le tableau des étudiants
    ui->EditStudentStage->setRowCount(0);

    // Récupérer la liste de tous les étudiants de la base de données
    QList<Etudiant> etudiants = daoEtudiant->getAllStudents();

    // Parcourir la liste d'étudiants et ajouter leurs informations dans le tableau
    int row = 0;
    foreach(Etudiant etudiant, etudiants){
        ui->EditStudentStage->insertRow(row);
        ui->EditStudentStage->setItem(row, 0, new QTableWidgetItem(QString::number(etudiant.id)));
        ui->EditStudentStage->setItem(row, 1, new QTableWidgetItem(etudiant.nom));
        ui->EditStudentStage->setItem(row, 2, new QTableWidgetItem(etudiant.prenom));
        row++;
    }

    // Effacer toutes les lignes existantes dans le tableau des entreprises
    ui->EditEntrepriseStage->setRowCount(0);

    // Récupérer la liste de toutes les entreprises de la base de données
    QList<Entreprise> entreprises = daoEntreprise->getAllEntreprises();

    // Parcourir la liste des entreprises et ajouter leurs informations dans le tableau
    int row2 = 0;
    foreach(Entreprise entreprise, entreprises){
        ui->EditEntrepriseStage->insertRow(row2);
        ui->EditEntrepriseStage->setItem(row2, 0, new QTableWidgetItem(QString::number(entreprise.id)));
        ui->EditEntrepriseStage->setItem(row2, 1, new QTableWidgetItem(entreprise.nom));
        ui->EditEntrepriseStage->setItem(row2, 2, new QTableWidgetItem(entreprise.ville));
        row2++;
    }
}

void StageForm::hideDatagrid(){
    ui->EditEntrepriseStage->hide();
    ui->EditStudentStage->hide();
}


void StageForm::setData(Stage stage){


    // Sélectionner l'entreprise dans le tableau
    for(int row = 0; row < ui->EditEntrepriseStage->rowCount(); row++) {
        int entrepriseId = ui->EditEntrepriseStage->item(row, 0)->text().toInt();
        if(entrepriseId == stage.entreprise.id) {
            ui->EditEntrepriseStage->selectRow(row);
            break;
        }
    }

    // Sélectionner l'étudiant dans le tableau
    for(int row = 0; row < ui->EditStudentStage->rowCount(); row++) {
        int studentId = ui->EditStudentStage->item(row, 0)->text().toInt();
        if(studentId == stage.etudiant.id) {
            ui->EditStudentStage->selectRow(row);
            break;
        }
    }

    //Sauvegarder l'id du stage
    idStage = stage.id;

    // Mettre à jour les champs de texte
    ui->editDateDebut->setDate(stage.dateDebut);
    ui->editDateFin->setDate(stage.dateFin);
    ui->EditPoste->setText(stage.poste);
    ui->editDesc->setPlainText(stage.desc);

}

Stage StageForm::getData(){
    Stage stage;
    stage.id = idStage;

    // Récupérer l'entreprise sélectionnée dans le tableau
    int entrepriseRow = ui->EditEntrepriseStage->currentRow();
    if (entrepriseRow >= 0) {
        QTableWidgetItem* entrepriseIdItem = ui->EditEntrepriseStage->item(entrepriseRow, 0);
        if (entrepriseIdItem != nullptr) {
            int entrepriseId = entrepriseIdItem->text().toInt();
            stage.entreprise = daoEntreprise->getEntById(entrepriseId);
        }
    }

    // Récupérer l'étudiant sélectionné dans le tableau
    int studentRow = ui->EditStudentStage->currentRow();
    if (studentRow >= 0) {
        QTableWidgetItem* studentIdItem = ui->EditStudentStage->item(studentRow, 0);
        if (studentIdItem != nullptr) {
            int studentId = studentIdItem->text().toInt();
            stage.etudiant = daoEtudiant->getEtuById(studentId);
        }
    }

    // Récupérer les champs de texte
    stage.dateDebut = ui->editDateDebut->date();
    stage.dateFin = ui->editDateFin->date();
    stage.poste = ui->EditPoste->text();
    stage.desc = ui->editDesc->toPlainText();

    return stage;
}


StageForm::~StageForm()
{
    delete ui;
    delete daoEntreprise;
    delete daoEtudiant;
}
