#include "enterpriseform.h"
#include "ui_enterpriseform.h"

// Constructeur
EnterpriseForm::EnterpriseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterpriseForm),
    daoRS(new dao_rs),
    daoDomaine(new dao_domaine)
{
    ui->setupUi(this);

    // Connecter les signaux aux slots
    connect(ui->addRS, SIGNAL(clicked(bool)), this, SLOT(prepareAddRS()));
    connect(ui->editRS, SIGNAL(clicked(bool)), this, SLOT(prepareEditRS()));
    connect(ui->confirmRS, SIGNAL(clicked(bool)), this, SLOT(confirmActionRS()));
    connect(ui->deleteRS, SIGNAL(clicked(bool)), this, SLOT(deleteRS()));
    connect(ui->deleteDomaine,SIGNAL(clicked(bool)), this, SLOT(deleteDomaine()));
    connect(ui->addDomaine,SIGNAL(clicked(bool)), this, SLOT(addDomaine()));

    // Charger les données initiales
    QList<ReseauxSocial> socialNetworks = daoRS->getAllSocialNetwork();
    foreach(ReseauxSocial rs, socialNetworks){
        ui->listRS->addItem(rs.nom);
    }

    QList<Domaine> domaines = daoDomaine->getAllDomains();
    foreach(Domaine domaine, domaines){
        ui->listDomaines->addItem(QString::number(domaine.id) + " - " + domaine.nom);
    }
}

// Ajouter un domaine à la liste des domaines de l'entreprise
void EnterpriseForm::addDomaine(){
    QString newDomain = ui->listDomaines->currentText();
    bool addDomain = true;
    for(int i = 0; i < ui->listDomaineEntreprise->count(); i++){
        if(ui->listDomaineEntreprise->itemText(i) == newDomain){
            addDomain = false;
            break;
        }
    }

    if(addDomain){
        ui->listDomaineEntreprise->addItem(newDomain);
    }
}

// Supprimer un domaine de la liste des domaines de l'entreprise
void EnterpriseForm::deleteDomaine(){
    int selectedIndex = ui->listDomaineEntreprise->currentIndex();
    if (selectedIndex != -1) {
        ui->listDomaineEntreprise->removeItem(selectedIndex);
    }
}

// Afficher les actions pour modifier les réseaux sociaux
void EnterpriseForm::showActionRS(){
    for (int i = 0; i < ui->actionRS->count(); ++i) {
        QWidget *widget = ui->actionRS->itemAt(i)->widget();
        if (widget) {
            widget->show();
        }
    }
}

// Masquer les actions pour modifier les réseaux sociaux
void EnterpriseForm::hideActionRS(){
    for (int i = 0; i < ui->actionRS->count(); ++i) {
        QWidget *widget = ui->actionRS->itemAt(i)->widget();
        if (widget) {
            widget->hide();
        }
    }
}

// Préparer la modification d'un réseau social
void EnterpriseForm::prepareEditRS(){
    // Récupérez l'item sélectionné dans la liste des réseaux sociaux
    int selectedIndex = ui->entrepriseEdit_RS_CB->currentIndex();
    currentAction = "edit";
    if (selectedIndex != -1) {
        // Récupérez les données de l'item sélectionné
        QString selectedItemText = ui->entrepriseEdit_RS_CB->currentText();

        // Extrait le nom et le pseudo du texte sélectionné
        QStringList splittedText = selectedItemText.split(" - ");
        if(splittedText.size() >= 2)
        {
               QString rsNom = splittedText[0].trimmed();
               QString rsPseudo = splittedText[1].trimmed();

               // Pré-remplir la comboBox et la lineEdit avec les valeurs récupérées
               ui->listRS->setCurrentText(rsNom);
               ui->entrepriseEdit_RsPseudo_TB->setText(rsPseudo);

               showActionRS();

        } else {
               // Gérer l'erreur, par exemple en affichant un message d'erreur ou en ignorant l'action
        }
    }
}

// Préparer l'ajout d'un réseau social
void EnterpriseForm::prepareAddRS(){
    ui->listRS->setCurrentIndex(-1);
    ui->entrepriseEdit_RsPseudo_TB->clear();
    currentAction = "add";
    showActionRS();
}

// Supprimer un réseau social de la liste
void EnterpriseForm::deleteRS(){
    int selectedIndex = ui->entrepriseEdit_RS_CB->currentIndex();
    if (selectedIndex != -1) {
        ui->entrepriseEdit_RS_CB->removeItem(selectedIndex);
    }
}

// Confirmer l'action en cours (ajouter ou modifier un réseau social)
void EnterpriseForm::confirmActionRS(){
    QString selectedRS = ui->listRS->currentText();
    QString enteredPseudo = ui->entrepriseEdit_RsPseudo_TB->text();

    if (currentAction == "add") {
        // Ajoutez le nouveau réseau social à la liste
        // Vous pouvez également vérifier si le réseau social existe déjà dans la liste et éviter les doublons
        QString newRSItem = selectedRS + " - " + enteredPseudo;
        ui->entrepriseEdit_RS_CB->addItem(newRSItem);
    } else if (currentAction == "edit") {
        // Mettez à jour le réseau social sélectionné dans la liste
        int selectedIndex = ui->entrepriseEdit_RS_CB->currentIndex();
        if (selectedIndex != -1) {
            QString updatedRSItem = selectedRS + " - " + enteredPseudo;
            ui->entrepriseEdit_RS_CB->setItemText(selectedIndex, updatedRSItem);
        }
    }

    // Réinitialisez l'action en cours et masquez les widgets d'action
    currentAction = "";
    hideActionRS();
}

// Définir les données de l'entreprise
void EnterpriseForm::setData(Entreprise entreprise){
    idEntreprise = entreprise.id;

    ui->entrepriseEdit_Nom_TB->setText(entreprise.nom);
    ui->entrepriseEdit_Ville_TB->setText(entreprise.ville);
    ui->entrepriseEdit_Email_TB->setText(entreprise.email);
    ui->entrepriseEdit_Tel_TB->setText(entreprise.telephone);
    ui->entreprise_adresse_TB->setText(entreprise.adresse);
    ui->entrepriseEdit_WebSite_TB->setText(entreprise.siteWeb);

    foreach(ReseauxSocial rs, entreprise.reseauxSociaux){
        QString actualRS = rs.nom + " - " + rs.pseudo;
        ui->entrepriseEdit_RS_CB->addItem(actualRS);
    }

    foreach(Domaine domaine, entreprise.domaine){
        QString actualDomaine = QString::number(domaine.id) + " - " + domaine.nom;
        ui->listDomaineEntreprise->addItem(actualDomaine);
    }
}

// Récupérer les données de l'entreprise
Entreprise EnterpriseForm::getData(){
    Entreprise entreprise;

    entreprise.id = idEntreprise;
    entreprise.nom = ui->entrepriseEdit_Nom_TB->text();
    entreprise.ville = ui->entrepriseEdit_Ville_TB->text();
    entreprise.email = ui->entrepriseEdit_Email_TB->text();
    entreprise.adresse = ui->entreprise_adresse_TB->text();
    entreprise.siteWeb = ui->entrepriseEdit_WebSite_TB->text();
    entreprise.telephone = ui->entrepriseEdit_Tel_TB->text();

    int itemCount = ui->entrepriseEdit_RS_CB->count();
    for (int i = 0; i < itemCount; ++i) {
        ReseauxSocial rs;

        QString itemText = ui->entrepriseEdit_RS_CB->itemText(i);
        QStringList splittedRS = itemText.split("-");
        rs.nom = splittedRS[0].trimmed();
        rs.pseudo = splittedRS[1].trimmed();
        rs.id = daoRS->getRSId(rs.nom);
        entreprise.reseauxSociaux.append(rs);
    }

    int itemCount2 = ui->listDomaineEntreprise->count();
    for (int i = 0; i < itemCount2; ++i) {
        Domaine domaine;

        QString itemText = ui->listDomaineEntreprise->itemText(i);
        QStringList splittedDomaine = itemText.split("-");
        domaine.nom = splittedDomaine[1].trimmed();
        domaine.id = splittedDomaine[0].toInt();
        entreprise.domaine.append(domaine);
    }

    return entreprise;
}

EnterpriseForm::~EnterpriseForm()
{
    delete ui;
    delete daoRS;
    delete daoDomaine;
}
