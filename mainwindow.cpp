// Classe principale de l'application, gère l'affichage des différentes pages
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Service/dao_entreprise.h"
#include "Service/dao_etudiant.h"
#include "Model/Etudiant.h"
#include "studentform.h"
#include "enterpriseform.h"
#include "stageform.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QProgressDialog>
#include <QTimer>

    // Constructeur de la classe MainWindow, initialise les DAO
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , daoEtudiant(new dao_etudiant)
        , daoRS(new dao_rs)
        , daoEntreprise(new dao_entreprise)
        , daoDomainne(new dao_domaine)
        , daoStage (new dao_stage)
    {
        ui->setupUi(this);
        // Connecte les signaux et les slots de l'interface utilisateur
        connectUI();
        checkDatabaseCallCount = 0;
        checkDatabase();
        loadBddCreditentials();

    }

// Connecte les signaux et les slots de l'interface utilisateur
void MainWindow::connectUI(){
    connect(ui->homeBTN, SIGNAL(clicked()), this, SLOT(navHome()));
    connect(ui->studentBTN, SIGNAL(clicked()), this, SLOT(navStudent()));
    connect(ui->compagnyBTN, SIGNAL(clicked()), this, SLOT(navCompagny()));
    connect(ui->settingsBTN, SIGNAL(clicked()), this, SLOT(navSettings()));
    connect(ui->stageBTN, SIGNAL(clicked()), this, SLOT(navInternship()));
    connect(ui->connexionBDD, SIGNAL(clicked()), this, SLOT(connexionBDD()));
    connect(ui->addStudent, SIGNAL(clicked()), this, SLOT(prepareAddStudent()));
    connect(ui->searchEtu, SIGNAL(clicked()), this, SLOT(refreshStudentList()));
    connect(ui->searchEnt, SIGNAL(clicked()), this, SLOT(refreshEntrepriseList()));
    connect(ui->addEntreprise, SIGNAL(clicked()), this, SLOT(prepareAddEntreprise()));
    connect(ui->stageBTN_2, SIGNAL(clicked()), this, SLOT(prepareAddStage()));

}
#pragma student {
// Affiche la liste des étudiants
void MainWindow::displayStudent() {
    // Configure les colonnes et les en-têtes du tableau
    ui->datagridStudent->setColumnCount(10);
    ui->datagridStudent->setHorizontalHeaderLabels(QStringList() << "Nom" << "Prénom" << "Email" << "Téléphone" << "Site Web" << "Promotion" << "Section" << "Réseaux Sociaux" << "Éditer" << "Supprimer");

    // Affiche la liste filtrée des étudiants
    refreshStudentList();

    // Ajuste automatiquement la largeur des colonnes en fonction du contenu
    ui->datagridStudent->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QList<Etudiant> MainWindow::getStudents(){
    // Récupérer tous les étudiants à partir de la base de données
    QList<Etudiant> etudiants = daoEtudiant->getAllStudents();

    // Initialiser une liste pour stocker les étudiants filtrés
    QList<Etudiant> filteredEtudiants;

    // Récupérer les paramètres de recherche de l'utilisateur
    QString filterBy = ui->filterByEtu->currentText();
    QString filterParam = ui->searchBarEtu->text();

    // Itérer sur chaque étudiant et filtrer en fonction des paramètres de recherche
    foreach(Etudiant etudiant, etudiants){
        bool match = false;

        if (filterBy == "nom") {
            match = etudiant.nom.contains(filterParam, Qt::CaseInsensitive);
        } else if (filterBy == "promo") {
            match = etudiant.classe.promo.contains(filterParam, Qt::CaseInsensitive);
        } else if (filterBy == "section") {
            match = etudiant.classe.section.contains(filterParam, Qt::CaseInsensitive);
        } else {
            match = true;
        }

        // Si l'étudiant correspond aux critères de recherche, l'ajouter à la liste filtrée
        if (match) {
            filteredEtudiants.append(etudiant);
        }
    }

    // Retourner la liste des étudiants filtrés
    return filteredEtudiants;
}

void MainWindow::refreshStudentList(){
    // Effacer toutes les lignes existantes
    ui->datagridStudent->setRowCount(0);

    // Obtenir la liste des étudiants
    QList<Etudiant> etudiants = getStudents();

    // Parcourir la liste des étudiants et les ajouter au tableau
    int row = 0;

    foreach(Etudiant etudiant, etudiants){

        ui->datagridStudent->insertRow(row);

        // Ajouter les informations de l'étudiant aux cellules du tableau
        ui->datagridStudent->setItem(row, 0, new QTableWidgetItem(etudiant.nom));
        ui->datagridStudent->setItem(row, 1, new QTableWidgetItem(etudiant.prenom));
        ui->datagridStudent->setItem(row, 2, new QTableWidgetItem(etudiant.email));
        ui->datagridStudent->setItem(row, 3, new QTableWidgetItem(etudiant.telephone));
        ui->datagridStudent->setItem(row, 4, new QTableWidgetItem(etudiant.site_web));
        ui->datagridStudent->setItem(row, 5, new QTableWidgetItem(etudiant.classe.promo));
        ui->datagridStudent->setItem(row, 6, new QTableWidgetItem(etudiant.classe.section));

        // Ajouter un QComboBox pour les réseaux sociaux
        QComboBox *reseauxSociauxCombo = new QComboBox(ui->datagridStudent);
        foreach(ReseauxSocial reseauSocial, etudiant.reseauxSociaux) {
            reseauxSociauxCombo->addItem(reseauSocial.nom  + " : " + reseauSocial.pseudo);
        }
        ui->datagridStudent->setCellWidget(row, 7, reseauxSociauxCombo);

        // Ajouter un bouton "Editer"
        QPushButton *editButton = new QPushButton("Éditer", ui->datagridStudent);
        ui->datagridStudent->setCellWidget(row, 8, editButton);
        connect(editButton, &QPushButton::clicked, this, [this, etudiant](){
            QDialog dialog(this);
            dialog.setWindowTitle("Modifier un étudiant");

            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            StudentForm *form = new StudentForm(nullptr);
            // Ajouter cette ligne pour pré-remplir les champs du formulaire
            form->setData(etudiant);

            layout->addWidget(form);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
            layout->addWidget(&buttonBox);

            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

            // Définir la taille minimale du dialogue en fonction de la taille du formulaire
            dialog.setMinimumSize(form->sizeHint());

            form->hideActionRS();

            if (dialog.exec() == QDialog::Accepted) {
                // Traiter les données du formulaire
                Etudiant modifiedEtudiant;
                modifiedEtudiant = form->getData();

                // pour éditer complètement
                daoRS->deleteRsStudent(modifiedEtudiant.id);
                daoRS->addListRSStudent(modifiedEtudiant.reseauxSociaux, modifiedEtudiant.id);
                daoEtudiant->updateStudent(modifiedEtudiant);
                refreshStudentList();
            } else {
                // L'utilisateur a annulé
            }
        });

        // Ajoutez un bouton "Supprimer"
        QPushButton *deleteButton = new QPushButton("Supr", ui->datagridStudent);
        ui->datagridStudent->setCellWidget(row, 9, deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, [this, etudiant](){
            // Code pour supprimer l'étudiant
            QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet étudiant ?",
                                              QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    // Supprimez l'étudiant de la base de données et mettez à jour l'affichage des étudiants
                    QMessageBox reply2;
                    reply2.setText(daoEtudiant->deleteStudent(etudiant.id));
                    reply2.exec();
                    refreshStudentList();
                }
        });
        // Passez à la ligne suivante
        row++;
    }
}

void MainWindow::prepareAddStudent(){
    QDialog dialog(this);
    dialog.setWindowTitle("Ajouter un étudiant");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    StudentForm *form = new StudentForm(nullptr);
    layout->addWidget(form);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Définir la taille minimale du dialogue en fonction de la taille du formulaire
    dialog.setMinimumSize(form->sizeHint());

    form->hideActionRS();

    if (dialog.exec() == QDialog::Accepted) {
        // Traitez les données du formulaire
        Etudiant newEtudiant;
        newEtudiant = form->getData();

        // Ajoutez le nouvel étudiant à la base de données et mettez à jour l'affichage des étudiants
        daoEtudiant->addStudent(newEtudiant);
        refreshStudentList();
    } else {
        // L'utilisateur a annulé
    }
}


#pragma }

#pragma Enntreprise Page{

void MainWindow::displayEntreprise(){
    // Configurer les colonnes et les en-têtes du tableau
    ui->datagridEntreprise->setColumnCount(11);
    ui->datagridEntreprise->setHorizontalHeaderLabels(QStringList() << "Nom" << "Adresse" << "Ville" << "Téléphone" << "Site Web" << "Email" << "Domaine" << "Réseaux Sociaux" << "Étudiants" << "Éditer" << "Supr");

    // Afficher la liste des entreprises
    refreshEntrepriseList();

    // Ajuster automatiquement la largeur des colonnes en fonction du contenu
    ui->datagridEntreprise->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QList<Entreprise> MainWindow::getEntreprise(){
    // Obtenir la liste de toutes les entreprises de la base de données et filtrer en fonction des critères de recherche
    QList<Entreprise> entreprises = daoEntreprise->getAllEntreprises();
    QList<Entreprise> filteredEntreprise;
    QString filterBy = ui->filterByEnt->currentText();
    QString filterParam = ui->searchBarEnt->text();

    foreach(Entreprise entreprise, entreprises){
        bool match = false;

        // Vérifier si l'entreprise correspond aux critères de recherche
        if (filterBy == "ville") {
            match = entreprise.ville.contains(filterParam, Qt::CaseInsensitive);
        } else if (filterBy == "nom") {
            match = entreprise.nom.contains(filterParam, Qt::CaseInsensitive);
        } else {
            match = true;
        }

        // Ajouter l'entreprise filtrée à la liste
        if (match) {
            filteredEntreprise.append(entreprise);
        }
    }

    return filteredEntreprise;
}

void MainWindow::refreshEntrepriseList(){
    // Effacez toutes les lignes existantes
    ui->datagridEntreprise->setRowCount(0);

    // Obtenez la liste des étudiants
    QList<Entreprise> entreprises = getEntreprise();

    // Parcourez la liste des étudiants et ajoutez-les au tableau
    int row = 0;

    foreach(Entreprise entreprise, entreprises){

        ui->datagridEntreprise->insertRow(row);
        // Ajoutez les informations de l'étudiant aux cellules du tableau
        ui->datagridEntreprise->setItem(row, 0, new QTableWidgetItem(entreprise.nom));
        ui->datagridEntreprise->setItem(row, 1, new QTableWidgetItem(entreprise.adresse));
        ui->datagridEntreprise->setItem(row, 2, new QTableWidgetItem(entreprise.ville));
        ui->datagridEntreprise->setItem(row, 3, new QTableWidgetItem(entreprise.telephone));
        ui->datagridEntreprise->setItem(row, 4, new QTableWidgetItem(entreprise.siteWeb));
        ui->datagridEntreprise->setItem(row, 5, new QTableWidgetItem(entreprise.email));

        // Ajoutez un QComboBox pour les domaines
        QComboBox *domaineCombo = new QComboBox(ui->datagridEntreprise);
        foreach(Domaine domaine, entreprise.domaine) {
            domaineCombo->addItem(domaine.nom);
        }
        ui->datagridEntreprise->setCellWidget(row, 6, domaineCombo);

        // Ajoutez un QComboBox pour les réseaux sociaux
        QComboBox *reseauxSociauxCombo = new QComboBox(ui->datagridEntreprise);
        foreach(ReseauxSocial reseauSocial, entreprise.reseauxSociaux) {
            reseauxSociauxCombo->addItem(reseauSocial.nom  + " : " + reseauSocial.pseudo);
        }
        ui->datagridEntreprise->setCellWidget(row, 7, reseauxSociauxCombo);


        // Ajoutez un bouton "Voir les étudiants"
        QPushButton *viewStudentsButton = new QPushButton("Voir les étudiants", ui->datagridEntreprise);
        ui->datagridEntreprise->setCellWidget(row, 8, viewStudentsButton);
        connect(viewStudentsButton, &QPushButton::clicked, this, [this, entreprise](){
            QDialog dialog(this);
            dialog.setWindowTitle("Étudiants ayant effectué un stage dans l'entreprise");

            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            QList<Etudiant> students = daoEtudiant->getStudentByEnt(entreprise.id);

            // Créer un QTableWidget pour afficher les étudiants
            QTableWidget *studentTable = new QTableWidget(&dialog);
            studentTable->setColumnCount(4);
            studentTable->setHorizontalHeaderLabels(QStringList() << "Nom" << "Prénom" << "Email" << "Téléphone");
            studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            // Ajouter chaque étudiant dans le tableau
            int row = 0;
            foreach(Etudiant student, students){
                studentTable->insertRow(row);
                studentTable->setItem(row, 0, new QTableWidgetItem(student.nom));
                studentTable->setItem(row, 1, new QTableWidgetItem(student.prenom));
                studentTable->setItem(row, 2, new QTableWidgetItem(student.email));
                studentTable->setItem(row, 3, new QTableWidgetItem(student.telephone));
                row++;
            }
            // Ajouter le tableau à la boîte de dialogue
            layout->addWidget(studentTable);

            // Ajouter un bouton "OK" pour fermer la boîte de dialogue
            QDialogButtonBox buttonBox(QDialogButtonBox::Ok, &dialog);
            layout->addWidget(&buttonBox);
            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

            // Afficher la boîte de dialogue
            dialog.exec();
        });


        // Ajouter un bouton "Editer"
        QPushButton *editButton = new QPushButton("Éditer", ui->datagridEntreprise);
        ui->datagridEntreprise->setCellWidget(row, 9, editButton);
        connect(editButton, &QPushButton::clicked, this, [this, entreprise](){
            QDialog dialog(this);
            dialog.setWindowTitle("Modifier une entreprise");

            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            EnterpriseForm *form = new EnterpriseForm(nullptr);

            form->setData(entreprise);
            layout->addWidget(form);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
            layout->addWidget(&buttonBox);

            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

            // Définir la taille minimale du dialogue en fonction de la taille du formulaire
            dialog.setMinimumSize(form->sizeHint());

            form->hideActionRS();

            if (dialog.exec() == QDialog::Accepted) {
                // Traitez les données du formulaire
                Entreprise modifiedEntreprise = form->getData();

                daoRS->deleteRsEntreprise(modifiedEntreprise.id);
                daoRS->addListRSEntreprise(modifiedEntreprise.reseauxSociaux, modifiedEntreprise.id);

                daoDomainne->deleteDomaineEntreprise(modifiedEntreprise.id);
                daoDomainne->addListDomaine(modifiedEntreprise.domaine, modifiedEntreprise.id);

                daoEntreprise->updateEntreprise(modifiedEntreprise);
                refreshEntrepriseList();

            } else {
                // L'utilisateur a annulé
            }
        });

        // Ajoutez un bouton "Supprimer"
        QPushButton *deleteButton = new QPushButton("Supr", ui->datagridEntreprise);
        ui->datagridEntreprise->setCellWidget(row, 10, deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, [this, entreprise](){
            // Code pour supprimer l'étudiant
            QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cet enterprise ?",
                                              QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    // Supprimez l'étudiant de la base de données et mettez à jour l'affichage des entreprises
                    QMessageBox reply3;
                    reply3.setText(daoEntreprise->deleteEntreprise(entreprise.id));
                    reply3.exec();
                    refreshEntrepriseList();
                }
        });
        // Passez à la ligne suivante
        row++;
    }
}

void MainWindow::prepareAddEntreprise(){
    QDialog dialog(this);
    dialog.setWindowTitle("Ajouter une entreprise");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    EnterpriseForm *form = new EnterpriseForm(nullptr);
    layout->addWidget(form);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Définir la taille minimale du dialogue en fonction de la taille du formulaire
    dialog.setMinimumSize(form->sizeHint());

    form->hideActionRS();
    if (dialog.exec() == QDialog::Accepted) {
        // Traitez les données du formulaire
        Entreprise newEntreprise;
        newEntreprise = form->getData();

        // Ajoutez la nouvel entreprise à la base de données et mettez à jour l'affichage des étudiants
        daoEntreprise->addEntreprise(newEntreprise);
        refreshEntrepriseList();
    } else {
        // L'utilisateur a annulé
    }
}

#pragma }

#pragma Stage Page{

void MainWindow::displayStage(){
    // Configurez les colonnes et les en-têtes du tableau
    ui->datagridStudent_2->setColumnCount(3);
    ui->datagridStudent_2->setHorizontalHeaderLabels(QStringList() << "Id" << "Nom" << "Prénom");
    // Ajustez automatiquement la largeur des colonnes en fonction du contenu
    ui->datagridStudent_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Configurez les colonnes et les en-têtes du tableau
    ui->datagridEntreprise_2->setColumnCount(3);
    ui->datagridEntreprise_2->setHorizontalHeaderLabels(QStringList() << "Id" << "Nom" << "Ville");
    // Ajustez automatiquement la largeur des colonnes en fonction du contenu
    ui->datagridEntreprise_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Configurez les colonnes et les en-têtes du tableau
    ui->datagridIntern->setColumnCount(8);
    ui->datagridIntern->setHorizontalHeaderLabels(QStringList() << "Etudiant" << "Entreprise" << "Date Début" << "Date Fin" << "Poste" << "description");
    // Ajustez automatiquement la largeur des colonnes en fonction du contenu
    ui->datagridIntern->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    refreshStageList();
}

void MainWindow::refreshStageList(){

    // Effacer toutes les lignes existantes dans le tableau des étudiants
    ui->datagridStudent_2->setRowCount(0);

    // Récupérer la liste de tous les étudiants de la base de données
    QList<Etudiant> etudiants = daoEtudiant->getAllStudents();

    // Parcourir la liste d'étudiants et ajouter leurs informations dans le tableau
    int row = 0;
    foreach(Etudiant etudiant, etudiants){
        ui->datagridStudent_2->insertRow(row);
        ui->datagridStudent_2->setItem(row, 0, new QTableWidgetItem(QString::number(etudiant.id)));
        ui->datagridStudent_2->setItem(row, 1, new QTableWidgetItem(etudiant.nom));
        ui->datagridStudent_2->setItem(row, 2, new QTableWidgetItem(etudiant.prenom));
        row++;
    }

    // Effacer toutes les lignes existantes dans le tableau des entreprises
    ui->datagridEntreprise_2->setRowCount(0);

    // Récupérer la liste de toutes les entreprises de la base de données
    QList<Entreprise> entreprises = daoEntreprise->getAllEntreprises();

    // Parcourir la liste des entreprises et ajouter leurs informations dans le tableau
    int row2 = 0;
    foreach(Entreprise entreprise, entreprises){
        ui->datagridEntreprise_2->insertRow(row2);
        ui->datagridEntreprise_2->setItem(row2, 0, new QTableWidgetItem(QString::number(entreprise.id)));
        ui->datagridEntreprise_2->setItem(row2, 1, new QTableWidgetItem(entreprise.nom));
        ui->datagridEntreprise_2->setItem(row2, 2, new QTableWidgetItem(entreprise.ville));
              row2++;
    }

    // Effacer toutes les lignes existantes dans le tableau des stages
    ui->datagridIntern->setRowCount(0);

    // Récupérer la liste de tous les stages de la base de données
    QList<Stage> stages = daoStage->getAllStages();

    // Parcourir la liste des stages et ajouter leurs informations dans le tableau
    int row3 = 0;
    foreach (Stage stage, stages) {
        ui->datagridIntern->insertRow(row3);
        ui->datagridIntern->setItem(row3, 0, new QTableWidgetItem(stage.etudiant.nom + " " + stage.etudiant.prenom));
        ui->datagridIntern->setItem(row3, 1, new QTableWidgetItem(stage.entreprise.nom));
        ui->datagridIntern->setItem(row3, 2, new QTableWidgetItem(stage.dateDebut.toString())); // Convertir QDate en QString
        ui->datagridIntern->setItem(row3, 3, new QTableWidgetItem(stage.dateFin.toString())); // Convertir QDate en QString
        ui->datagridIntern->setItem(row3, 4, new QTableWidgetItem(stage.poste));
        ui->datagridIntern->setItem(row3, 5, new QTableWidgetItem(stage.desc));


    QPushButton *editButton = new QPushButton("Éditer", ui->datagridIntern);
        ui->datagridIntern->setCellWidget(row3, 6, editButton);
        connect(editButton, &QPushButton::clicked, this, [this, stage](){
            QDialog dialog(this);
            dialog.setWindowTitle("Modifier un stage");

            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            StageForm *form = new StageForm(nullptr);

            // Récupérer les données existantes du stage
            form->setData(stage);

            layout->addWidget(form);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
            layout->addWidget(&buttonBox);

            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

            // Définir la taille minimale du dialogue en fonction de la taille du formulaire
            dialog.setMinimumSize(form->sizeHint());

            if (dialog.exec() == QDialog::Accepted) {
                // Traitez les données du formulaire

                Stage modifiedStage;
                modifiedStage = form->getData();

                daoStage->updateStage(modifiedStage);
                refreshStageList();

            } else {
                // L'utilisateur a annulé
            }
        });

        // Ajoutez un bouton "Supprimer"
        QPushButton *deleteButton = new QPushButton("Supr", ui->datagridIntern);
        ui->datagridIntern->setCellWidget(row3, 7, deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, [this, stage](){
            // Code pour supprimer le stage
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce stage ?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                // Supprimez le stage de la base de données et mettez à jour l'affichage des stages
                QMessageBox reply4;
                reply4.setText(daoStage->deleteStage(stage.id));
                reply4.exec();
                refreshStageList();
            }
        });

        row3++;
    }
}

void MainWindow::prepareAddStage(){
    int entrepriseRow = ui->datagridEntreprise_2->currentRow();
    int studentRow = ui->datagridStudent_2->currentRow();

    // Vérifier si un étudiant et une entreprise sont sélectionnés
    if (entrepriseRow >= 0 && studentRow >= 0) {
        QDialog dialog(this);
        dialog.setWindowTitle("Ajouter un stage");

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        StageForm *form = new StageForm(nullptr);
        layout->addWidget(form);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        layout->addWidget(&buttonBox);

        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        // Définir la taille minimale du dialogue en fonction de la taille du formulaire
        dialog.setMinimumSize(form->sizeHint());
        form->hideDatagrid();

        if (dialog.exec() == QDialog::Accepted) {
            // Traitez les données du formulaire
            Stage stage;
            stage = form->getData();

            int entrepriseRow = ui->datagridEntreprise_2->currentRow();
            if (entrepriseRow >= 0) {
                QTableWidgetItem* entrepriseIdItem = ui->datagridEntreprise_2->item(entrepriseRow, 0);
                if (entrepriseIdItem != nullptr) {
                    int entrepriseId = entrepriseIdItem->text().toInt();
                    stage.entreprise = daoEntreprise->getEntById(entrepriseId);
                }
            }

            // Récupérer l'étudiant sélectionné dans le tableau
            int studentRow = ui->datagridStudent_2->currentRow();
            if (studentRow >= 0) {
                QTableWidgetItem* studentIdItem = ui->datagridStudent_2->item(studentRow, 0);
                if (studentIdItem != nullptr) {
                    int studentId = studentIdItem->text().toInt();
                    stage.etudiant = daoEtudiant->getEtuById(studentId);
                }
            }

            daoStage->addStage(stage);
            refreshStageList();
        } else {
            // L'utilisateur a annulé
        }
    }else{
        // Afficher un message d'erreur indiquant que l'utilisateur doit sélectionner un étudiant et une entreprise
        QMessageBox::warning(this, "Sélection manquante", "Veuillez sélectionner un étudiant et une entreprise avant d'ajouter un stage.");
    }

}

#pragma }

#pragma Navigation {
void MainWindow::navCompagny(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::navStudent(){
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::navHome(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::navSettings(){
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::navInternship(){
    ui->stackedWidget->setCurrentIndex(4);
}
#pragma }

#pragma BDD Connnexion{
void MainWindow::connexionBDD() {
    // Création d'un objet QSettings pour stocker les données de configuration
    QSettings settings("get", "getBdd");

    settings.setValue("HostName", ui->hostnameBDD->text());
    settings.setValue("UserName", ui->usernameBDD->text());
    settings.setValue("Password", ui->pwdBDD->text());
    settings.setValue("DatabaseName", ui->bddname->text());
    settings.setValue("Port", ui->portBDD->text());

    // Crée un QProgressDialog pour indiquer la tentative de connexion
    QProgressDialog progressDialog(this);
    progressDialog.setLabelText(tr("Connexion à la base de données..."));
    progressDialog.setRange(0, 0); // Mode indéterminé
    progressDialog.setModal(true);

    // QTimer pour garantir que le QProgressDialog reste visible pendant au moins 2 secondes
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(1500);

    progressDialog.show();

    // Relance la connexion à la base de données avec les nouvelles valeurs
    Connexion::getInstance().reloadActualConnection();

    // Attend la fin du QTimer
    while (timer.isActive()) {
        QApplication::processEvents();
    }

    progressDialog.cancel();

    if (checkDatabase()) {
        reloadActualConnection();
    }
}

void MainWindow::reloadActualConnection(){

    // Affiche la liste des étudiants
    displayStudent();

    // Affiche la liste des entreprises
    displayEntreprise();

    // Affiche la liste des stages
    displayStage();

    //Affiche les information de connexion a la bdd
    loadBddCreditentials();
}

void MainWindow::loadBddCreditentials(){
    // Charge les paramètres de connexion
    QSettings settings("get", "getBdd");

    ui->hostnameBDD->setText(settings.value("HostName").toString());
    ui->usernameBDD->setText(settings.value("UserName").toString());
    ui->pwdBDD->setText(settings.value("Password").toString());
    ui->bddname->setText(settings.value("DatabaseName").toString());
    ui->portBDD->setText(settings.value("Port").toString());
}

bool MainWindow::checkDatabase() {
    // Vérifie si la base de données est connectée
    if (!Connexion::getInstance().isConnected()) {
        // Si la connexion a échoué, va sur la page 3 et désactive les boutons de navigation
        ui->stackedWidget->setCurrentIndex(3);

        ui->studentBTN->setEnabled(false);
        ui->compagnyBTN->setEnabled(false);
        ui->stageBTN->setEnabled(false);

        // Affiche un message d'erreur
        QMessageBox::critical(this, tr("Erreur de connexion"), tr("La connexion à la base de données a échoué.\n") +
                              Connexion::getInstance().getLastError().text());

        return false;
    } else {
        ui->stackedWidget->setCurrentIndex(0);

        ui->studentBTN->setEnabled(true);
        ui->compagnyBTN->setEnabled(true);
        ui->stageBTN->setEnabled(true);

        reloadActualConnection();

        checkDatabaseCallCount++; // Incrémente le compteur

        // Affiche un message de succès seulement si le compteur est supérieur à 1
        if (checkDatabaseCallCount > 1) {
            QMessageBox::information(this, tr("Connexion réussie"), tr("La connexion à la base de données a réussi."));
        }

        return true;
    }
}



#pragma }


MainWindow::~MainWindow()
{
    delete ui;

    delete daoEtudiant;

    delete daoRS;

    delete daoStage;

    delete daoEntreprise;

    delete daoDomainne;
}

