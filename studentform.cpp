#include "studentform.h"
#include "ui_studentform.h"
#include "Model/ReseauxSocial.h"

StudentForm::StudentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentForm),
    daoRS(new dao_rs),
    daoClasse(new dao_classe)
{
    ui->setupUi(this);

    // Connexion des signaux aux slots correspondants
    connect(ui->addRS, SIGNAL(clicked(bool)), this, SLOT(prepareAddRS()));
    connect(ui->editRS, SIGNAL(clicked(bool)), this, SLOT(prepareEditRS()));
    connect(ui->confirmRS, SIGNAL(clicked(bool)), this, SLOT(confirmActionRS()));
    connect(ui->deleteRS, SIGNAL(clicked(bool)), this, SLOT(deleteRS()));

    // Récupération de la liste des classes depuis la base de données
    QList<Classe> classes = daoClasse->getAllClasse();

    // Ajout des classes à la comboBox correspondante
    foreach(Classe classe, classes){
        ui->studentEdit_Class_CB->addItem(QString::number(classe.classe_id) + " / " + classe.promo + " / " + classe.section);
    }

    // Récupération de la liste des réseaux sociaux depuis la base de données
    QList<ReseauxSocial> socialNetworks = daoRS->getAllSocialNetwork();

    // Ajout des réseaux sociaux à la liste correspondante
    foreach(ReseauxSocial rs, socialNetworks){
        ui->listRS->addItem(rs.nom);
    }
}

// Affichage des widgets d'action pour la modification d'un réseau social
void StudentForm::showActionRS(){
    for (int i = 0; i < ui->actionRS->count(); ++i) {
        QWidget *widget = ui->actionRS->itemAt(i)->widget();
        if (widget) {
            widget->show();
        }
    }
}

// Masquage des widgets d'action pour la modification d'un réseau social
void StudentForm::hideActionRS(){
    for (int i = 0; i < ui->actionRS->count(); ++i) {
        QWidget *widget = ui->actionRS->itemAt(i)->widget();
        if (widget) {
            widget->hide();
        }
    }
}

// Préparation de l'édition d'un réseau social sélectionné dans la liste
void StudentForm::prepareEditRS(){
    // Récupération de l'item sélectionné dans la liste des réseaux sociaux
    int selectedIndex = ui->studentEdit_RS_CB->currentIndex();
    currentAction = "edit";
    if (selectedIndex != -1) {
        // Récupération des données de l'item sélectionné
        QString selectedItemText = ui->studentEdit_RS_CB->currentText();

        // Extraction du nom et du pseudo du texte sélectionné
        QStringList splittedText = selectedItemText.split(" - ");

        if(splittedText.size() >= 2) {
            QString rsNom = splittedText[0].trimmed();
            QString rsPseudo = splittedText[1].trimmed();

            // Pré-remplissage de la comboBox et de la lineEdit avec les valeurs récupérées
            ui->listRS->setCurrentText(rsNom);
            ui->studentEdit_RsPseudo_TB->setText(rsPseudo);

            showActionRS();
        } else {
            // Gestion de l'erreur, par exemple en affichant un message d'erreur ou en ignorant l'action
        }
    }
}


// Réinitialise les champs pour ajouter un nouveau réseau social
void StudentForm::prepareAddRS(){
    ui->listRS->setCurrentIndex(-1);
    ui->studentEdit_RsPseudo_TB->clear();
    currentAction = "add";
    showActionRS();
}

// Supprime l'élément de réseau social sélectionné
void StudentForm::deleteRS(){
    ui->studentEdit_RS_CB->removeItem(ui->studentEdit_RS_CB->currentIndex());
}

// Ajoute/modifie un élément de réseau social dans la liste
void StudentForm::confirmActionRS(){
    QString selectedRS = ui->listRS->currentText();
    QString enteredPseudo = ui->studentEdit_RsPseudo_TB->text();

    if (currentAction == "add") {
        QString newRSItem = selectedRS + " - " + enteredPseudo;
        ui->studentEdit_RS_CB->addItem(newRSItem);
    } else if (currentAction == "edit") {
        int selectedIndex = ui->studentEdit_RS_CB->currentIndex();
        if (selectedIndex != -1) {
            QString updatedRSItem = selectedRS + " - " + enteredPseudo;
            ui->studentEdit_RS_CB->setItemText(selectedIndex, updatedRSItem);
        }
    }

    currentAction = "";
    hideActionRS();
}

// Remplit les champs avec les données d'un étudiant donné
void StudentForm::setData(Etudiant etudiant) {
    idStudent = etudiant.id;
    ui->studentEdit_Nom_TB->setText(etudiant.nom);
    ui->studentEdit_Prenom_TB->setText(etudiant.prenom);
    ui->studentEdit_Mail_TB->setText(etudiant.email);
    ui->studentEdit_Tel_TB->setText(etudiant.telephone);
    ui->studentEdit_WebSite_TB->setText(etudiant.site_web);

    QString actualClasse = QString::number(etudiant.classe.classe_id) + " / " + etudiant.classe.promo + " / " + etudiant.classe.section;
    ui->studentEdit_Class_CB->setCurrentText(actualClasse);

    foreach(ReseauxSocial rs, etudiant.reseauxSociaux){
        QString actualRS = rs.nom + " - " + rs.pseudo;
        ui->studentEdit_RS_CB->addItem(actualRS);
    }
}

// Récupère les données de l'interface utilisateur pour créer un objet Etudiant
Etudiant StudentForm::getData()
{
    Etudiant etudiant;

    // Récupère les données de chaque champ et les stocke dans l'objet Etudiant correspondant
    etudiant.id = idStudent;
    etudiant.nom = ui->studentEdit_Nom_TB->text();
    etudiant.prenom = ui->studentEdit_Prenom_TB->text();
    etudiant.email = ui->studentEdit_Mail_TB->text();
    etudiant.telephone = ui->studentEdit_Tel_TB->text();
    etudiant.site_web = ui->studentEdit_WebSite_TB->text();

    // Récupère les données de la liste déroulante Classe et les stocke dans l'objet Classe correspondant
    QStringList splittedTextClasse = ui->studentEdit_Class_CB->currentText().split(" / ");
    Classe classe;
    classe.classe_id = splittedTextClasse[0].toInt();
    classe.promo = splittedTextClasse[1];
    classe.section = splittedTextClasse[2];
    etudiant.classe = classe;

    // Récupère les données de la liste de réseaux sociaux et les stocke dans une liste de ReseauxSocial correspondante
    QList<ReseauxSocial> reseauxSociaux;
    for (int i = 0; i < ui->studentEdit_RS_CB->count(); ++i) {
        QString itemText = ui->studentEdit_RS_CB->itemText(i);
        QStringList splittedRS = itemText.split("-");
        ReseauxSocial rs;
        rs.nom = splittedRS[0].trimmed();
        rs.pseudo = splittedRS[1].trimmed();
        rs.id = daoRS->getRSId(rs.nom);
        reseauxSociaux.append(rs);
    }
    etudiant.reseauxSociaux = reseauxSociaux;

    return etudiant;
}

// Destructeur de la classe StudentForm, libère la mémoire allouée pour les objets UI, DaoRS et DaoClasse
StudentForm::~StudentForm()
{
    delete ui;
    delete daoRS;
    delete daoClasse;
}
