-- Création de la table "entreprise"
CREATE TABLE entreprise (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(200) NOT NULL,
    adresse VARCHAR(200) NOT NULL,
    ville VARCHAR(100) NOT NULL,
    telephone VARCHAR(20) NOT NULL,
    email VARCHAR(200) UNIQUE NOT NULL,
    site_web VARCHAR(255)
) ENGINE = InnoDB;

-- Création de la table "domaine"
CREATE TABLE domaine (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(200) NOT NULL UNIQUE
) ENGINE = InnoDB;

-- Création de la table "entreprise_domaine"
CREATE TABLE entreprise_domaine (
    entreprise_id INT NOT NULL,
    domaine_id INT NOT NULL,
    PRIMARY KEY (entreprise_id, domaine_id),
    FOREIGN KEY (entreprise_id) REFERENCES entreprise(id),
    FOREIGN KEY (domaine_id) REFERENCES domaine(id)
) ENGINE = InnoDB;

-- Création de la table "section"
CREATE TABLE section (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(200) NOT NULL
) ENGINE = InnoDB;

-- Création de la table "promo"
CREATE TABLE promo (
    id INT AUTO_INCREMENT PRIMARY KEY,
    annee VARCHAR(20) NOT NULL
) ENGINE = InnoDB;

-- Création de la table "classe"
CREATE TABLE classe (
    id INT AUTO_INCREMENT PRIMARY KEY,
    section_id INT NOT NULL,
    promo_id INT NOT NULL,
    FOREIGN KEY (section_id) REFERENCES section(id),
    FOREIGN KEY (promo_id) REFERENCES promo(id)
) ENGINE = InnoDB;
-- Création de la table "etudiant"
CREATE TABLE etudiant (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(200) NOT NULL,
    prenom VARCHAR(200) NOT NULL,
    email VARCHAR(200) UNIQUE NOT NULL,
    telephone VARCHAR(20) NOT NULL,
    classe_id INT NOT NULL,
    site_web VARCHAR(255),
    FOREIGN KEY (classe_id) REFERENCES classe(id)
) ENGINE = InnoDB;

-- Création de la table "reseau_social"
CREATE TABLE reseau_social (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(200) NOT NULL UNIQUE,
    url VARCHAR(255) NOT NULL UNIQUE
) ENGINE = InnoDB;

-- Création de la table "etudiant_reseau_social"
CREATE TABLE etudiant_reseau_social (
    etudiant_id INT NOT NULL,
    reseau_social_id INT NOT NULL,
    identifiant VARCHAR(200) NOT NULL,
    PRIMARY KEY (etudiant_id, reseau_social_id),
    FOREIGN KEY (etudiant_id) REFERENCES etudiant(id),
    FOREIGN KEY (reseau_social_id) REFERENCES reseau_social(id)
) ENGINE = InnoDB;

-- Création de la table "entreprise_reseau_social"
CREATE TABLE entreprise_reseau_social (
    entreprise_id INT NOT NULL,
    reseau_social_id INT NOT NULL,
    identifiant VARCHAR(200) NOT NULL,
    PRIMARY KEY (entreprise_id, reseau_social_id),
    FOREIGN KEY (entreprise_id) REFERENCES entreprise(id),
    FOREIGN KEY (reseau_social_id) REFERENCES reseau_social(id)
) ENGINE = InnoDB;

-- Création de la table "stage"
CREATE TABLE stage (
    id INT AUTO_INCREMENT PRIMARY KEY,
    etudiant_id INT NOT NULL,
    entreprise_id INT NOT NULL,
    date_debut DATE NOT NULL,
    date_fin DATE NOT NULL,
    poste VARCHAR(200) NOT NULL,
    description TEXT,
    FOREIGN KEY (etudiant_id) REFERENCES etudiant(id),
    FOREIGN KEY (entreprise_id) REFERENCES entreprise(id)
) ENGINE = InnoDB;


-- Insertion des entreprises
INSERT INTO entreprise (nom, adresse, ville, telephone, email, site_web) VALUES
('ACME Inc.', '123 rue de la Paix', 'Paris', '01 23 45 67 89', 'contact@acme.com', 'www.acme.com'),
('Globex Corporation', '456 avenue des Champs-Elysées', 'Paris', '01 34 56 78 90', 'contact@globex.com', 'www.globex.com'),
('Wayne Enterprises', '789 boulevard Haussmann', 'Paris', '01 45 67 89 01', 'contact@wayne.com', 'www.wayne.com'),
('Stark Industries', '1 place de la Concorde', 'Paris', '01 23 45 67 89', 'contact@stark.com', 'www.stark.com'),
('LexCorp', '42 rue de la Victoire', 'Paris', '01 34 56 78 90', 'contact@lexcorp.com', 'www.lexcorp.com'),
('Cyberdyne Systems', '100 rue des Robots', 'Lyon', '04 23 45 67 89', 'contact@cyberdyne.com', 'www.cyberdyne.com'),
('Oscorp Industries', '200 avenue des Gobelins', 'Marseille', '04 34 56 78 90', 'contact@oscorp.com', 'www.oscorp.com'),
('Umbrella Corporation', '300 boulevard du Virus', 'Toulouse', '05 45 67 89 01', 'contact@umbrella.com', 'www.umbrella.com'),
('Weyland-Yutani', '400 place de la Conquête', 'Nice', '04 23 45 67 89', 'contact@weyland.com', 'www.weyland.com'),
('Tyrell Corporation', '500 rue des Réplicants', 'Bordeaux', '05 34 56 78 90', 'contact@tyrell.com', 'www.tyrell.com');

-- Insertion des domaines
INSERT INTO domaine (nom) VALUES
('Informatique'),
('Electronique'),
('Automatique'),
('Réseaux et télécommunications'),
('Développement web'),
('Mécanique'),
('Génie civil'),
('Génie électrique'),
('Génie industriel'),
('Génie chimique');

-- Insertion des sections
INSERT INTO section (nom) VALUES
('BTS SIO'),
('BTS SNIR'),
('BTS CRSA'),
('BTS ATI');

-- Insertion des promotions
INSERT INTO promo (annee) VALUES
('2022-2023'),
('2023-2024'),
('2024-2025'),
('2025-2026');

-- Insertion des classes
INSERT INTO classe (section_id, promo_id) VALUES
(1, 1),
(2, 1),
(1, 2),
(2, 2),
(3, 1),
(4, 1),
(3, 2),
(4, 2),
(1, 3),
(2, 3),
(3, 3),
(4, 3);


-- Insertion des étudiants
INSERT INTO etudiant (nom, prenom, email, telephone, classe_id, site_web) VALUES
('Dupont', 'Jean', 'jean.dupont@example.com', '01 23 45 67 89', 1, NULL),
('Martin', 'Julie', 'julie.martin@example.com', '01 23 45 67 90', 2, NULL),
('Garcia', 'Pierre', 'pierre.garcia@example.com', '01 23 45 67 91', 1, NULL),
('Bernard', 'Sophie', 'sophie.bernard@example.com', '01 23 45 67 92', 2, NULL),
('Moreau', 'Luc', 'luc.moreau@example.com', '01 23 45 67 93', 3, NULL),
('Petit', 'Marie', 'marie.petit@example.com', '01 23 45 67 94', 4, NULL),
('Lefebvre', 'Antoine', 'antoine.lefebvre@example.com', '01 23 45 67 95', 3, NULL),
('Roux', 'Emilie', 'emilie.roux@example.com', '01 23 45 67 96', 4, NULL),
('Morel', 'Thibault', 'thibault.morel@example.com', '01 23 45 67 97', 1, NULL),
('Fournier', 'Hélène', 'helene.fournier@example.com', '01 23 45 67 98', 2, NULL),
('Doe', 'John', 'john.doe@example.com', '01 23 45 67 99', 1, NULL),
('Smith', 'Sarah', 'sarah.smith@example.com', '01 23 45 67 00', 2, NULL),
('Brown', 'Emily', 'emily.brown@example.com', '01 23 45 67 01', 1, NULL),
('Taylor', 'Michael', 'michael.taylor@example.com', '01 23 45 67 02', 2, NULL),
('Wilson', 'David', 'david.wilson@example.com', '01 23 45 67 03', 3, NULL),
('Johnson', 'Emma', 'emma.johnson@example.com', '01 23 45 67 04', 4, NULL),
('Lee', 'Oliver', 'oliver.lee@example.com', '01 23 45 67 05', 3, NULL),
('Harris', 'Isabella', 'isabella.harris@example.com', '01 23 45 67 06', 4, NULL),
('Clark', 'Charlotte', 'charlotte.clark@example.com', '01 23 45 67 07', 1, NULL),
('Lewis', 'Liam', 'liam.lewis@example.com', '01 23 45 67 08', 2, NULL);

-- Insertion des associations entreprise-domaine
INSERT INTO entreprise_domaine (entreprise_id, domaine_id) VALUES
(1, 1),
(1, 5),
(2, 1),
(2, 4),
(3, 2),
(4, 1),
(4, 2),
(4, 3),
(5, 3),
(5, 5),
(1, 6),
(2, 7),
(3, 8),
(4, 9),
(5, 10);

-- Insertion des réseaux sociaux
INSERT INTO reseau_social (nom, url) VALUES
('LinkedIn', 'https://www.linkedin.com/'),
('Facebook', 'https://www.facebook.com/'),
('Twitter', 'https://twitter.com/'),
('Instagram', 'https://www.instagram.com/'),
('GitHub', 'https://github.com/'),
('Pinterest', 'https://www.pinterest.com/'),
('Snapchat', 'https://www.snapchat.com/'),
('TikTok', 'https://www.tiktok.com/'),
('YouTube', 'https://www.youtube.com/'),
('Dribbble', 'https://dribbble.com/');

-- Insertion des associations étudiant-réseau social
INSERT INTO etudiant_reseau_social (etudiant_id, reseau_social_id, identifiant) VALUES
(1, 1, 'jean.dupont'),
(1, 2, 'jean.dupont'),
(2, 3, 'julie.martin'),
(2, 4, 'julie.martin'),
(3, 1, 'pierre.garcia'),
(4, 3, 'sophie.bernard'),
(5, 1, 'luc.moreau'),
(5, 5, 'lucmoreau'),
(6, 2, 'marie.petit'),
(7, 4, 'antoine.lefebvre'),
(8, 1, 'emilie.roux'),
(8, 2, 'emilie.roux'),
(9, 4, 'thibault.morel'),
(10, 5, 'helene.fournier');

-- Insertion des associations entreprise-réseau social
INSERT INTO entreprise_reseau_social (entreprise_id, reseau_social_id, identifiant) VALUES
(1, 1, 'acme'),
(2, 3, 'globex'),
(3, 1, 'wayne'),
(4, 4, 'stark'),
(5, 2, 'lexcorp');

-- Insertion des stages
INSERT INTO stage (etudiant_id, entreprise_id, date_debut, date_fin, poste, description) VALUES
(1, 1, '2022-01-01', '2022-06-30', 'Développeur web', 'Stage de développement web'),
(2, 2, '2022-01-01', '2022-06-30', 'Développeur logiciel', 'Stage de développement logiciel'),
(3, 3, '2022-01-01', '2022-06-30', 'Ingénieur électronique', 'Stage en électronique'),
(4, 4, '2022-01-01', '2022-06-30', 'Ingénieur réseau', 'Stage en réseaux et télécommunications'),
(5, 5, '2022-01-01', '2022-06-30', 'Ingénieur en automatique', 'Stage en automatique');

-- 1. Sélectionner tous les étudiants
SELECT * FROM etudiant;

-- 2. Sélectionner toutes les entreprises
SELECT * FROM entreprise;

-- 3. Sélectionner les étudiants et leurs classes et tout leurs reseaux
SELECT e.* , s.nom , p.annee , rs.nom , ers.identifiant
FROM etudiant e
JOIN classe c ON e.classe_id = c.id
JOIN section s ON c.section_id = s.id
JOIN promo p ON c.promo_id = p.id
JOIN etudiant_reseau_social ers ON e.id = ers.etudiant_id
JOIN reseau_social rs ON ers.reseau_social_id = rs.id;

-- 4. Sélectionner les entreprises et leurs domaines
SELECT e.id AS entreprise_id, e.nom AS entreprise_nom, d.id AS domaine_id, d.nom AS domaine_nom
FROM entreprise e
JOIN entreprise_domaine ed ON e.id = ed.entreprise_id
JOIN domaine d ON ed.domaine_id = d.id;

-- 5. Sélectionner les étudiants et leurs réseaux sociaux
SELECT et.id AS etudiant_id, et.nom AS etudiant_nom, et.prenom AS etudiant_prenom, rs.id AS reseau_social_id, rs.nom AS reseau_social_nom, ers.identifiant AS identifiant
FROM etudiant et
JOIN etudiant_reseau_social ers ON et.id = ers.etudiant_id
JOIN reseau_social rs ON ers.reseau_social_id = rs.id;

-- 6. Sélectionner les entreprises et leurs réseaux sociaux
SELECT en.id AS entreprise_id, en.nom AS entreprise_nom, rs.id AS reseau_social_id, rs.nom AS reseau_social_nom, ers.identifiant AS identifiant
FROM entreprise en
JOIN entreprise_reseau_social ers ON en.id = ers.entreprise_id
JOIN reseau_social rs ON ers.reseau_social_id = rs.id;

-- 7. Sélectionner les stages, étudiants et entreprises
SELECT st.id AS stage_id, st.poste AS stage_poste, st.date_debut AS stage_date_debut, st.date_fin AS stage_date_fin, et.id AS etudiant_id, et.nom AS etudiant_nom, et.prenom AS etudiant_prenom, en.id AS entreprise_id, en.nom AS entreprise_nom
FROM stage st
JOIN etudiant et ON st.etudiant_id = et.id
JOIN entreprise en ON st.entreprise_id = en.id;