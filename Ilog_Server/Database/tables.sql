DROP TABLE IF EXISTS utilisateurs;
DROP TABLE IF EXISTS utilisateurs_connexion;
DROP TABLE IF EXISTS pages_utilisateur;
DROP TABLE IF EXISTS entreprises;
DROP TABLE IF EXISTS etablissements;
DROP TABLE IF EXISTS departements;
DROP TABLE IF EXISTS formations_suivies;
DROP TABLE IF EXISTS salarie_categorie;
DROP TABLE IF EXISTS grille_salaires;
DROP TABLE IF EXISTS permis;
DROP TABLE IF EXISTS tokens;
DROP TABLE IF EXISTS emplois;
DROP TABLE IF EXISTS visites_medicales;
DROP TABLE IF EXISTS offline_request;
DROP TABLE IF EXISTS etablissements_departement;
DROP TABLE IF EXISTS jours_feries;
DROP TABLE IF EXISTS etablissements_jours_feries;
DROP TABLE IF EXISTS salaries;
DROP TABLE IF EXISTS coordonnees;
DROP TABLE IF EXISTS absences;
DROP TABLE IF EXISTS enfants;
DROP TABLE IF EXISTS articles;
DROP TABLE IF EXISTS articles_categories;
DROP TABLE IF EXISTS artCategories;
DROP TABLE IF EXISTS articles_fournisseurs;
DROP TABLE IF EXISTS achats;
DROP TABLE IF EXISTS lignes_achats;
DROP TABLE IF EXISTS flow_achat;
DROP TABLE IF EXISTS inventaires;
DROP TABLE IF EXISTS lignes_inventaires;
DROP TABLE IF EXISTS fournisseurs;
DROP TABLE IF EXISTS fournisseurs_categories;
DROP TABLE IF EXISTS commandes;
DROP TABLE IF EXISTS flow_commande;
DROP TABLE IF EXISTS lignes_commandes;
DROP TABLE IF EXISTS produits;
DROP TABLE IF EXISTS produits_categories;
DROP TABLE IF EXISTS fourCategories;
DROP TABLE IF EXISTS prodCategories;
DROP TABLE IF EXISTS fournisseurs_categories;
DROP TABLE IF EXISTS produits_categories;
DROP TABLE IF EXISTS salles;
DROP TABLE IF EXISTS tables;
DROP TABLE IF EXISTS commandes_ventes;
DROP TABLE IF EXISTS code_etablissement;

--creation de la table utilisateur
CREATE TABLE IF NOT EXISTS utilisateurs (
    username VARCHAR(20) PRIMARY KEY UNIQUE,
    sexe smallint NOT NULL,
    nom VARCHAR(30) NOT NULL,
    prenom VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE,
    psswrd VARCHAR(100) NOT NULL,
    status smallint default 1,
    profil VARCHAR(20),
    CONSTRAINT check_status CHECK(status IN (1, 2, 3, 4)),
    CONSTRAINT check_sexe CHECK(sexe IN (1, 2)),
    CONSTRAINT check_profil CHECK(profil IN ('dev', 'admin', 'agent', 'user'))
);

CREATE TABLE IF NOT EXISTS pages_utilisateur(
    id SERIAL PRIMARY KEY,
    username VARCHAR(20) NOT NULL,
    section VARCHAR(50) NOT NULL,
    parent_id INTEGER,
    readable int NOT NULL default 0,
    writable int NOT NULL default 0,
    FOREIGN KEY(username) REFERENCES utilisateurs(username),
    FOREIGN KEY(parent_id) REFERENCES pages_utilisateur(id)
);

CREATE TABLE IF NOT EXISTS utilisateurs_connexion (
    username VARCHAR(20) NOT NULL,
    date DATE DEFAULT CURRENT_TIMESTAMP NOT NULL,
    FOREIGN KEY(username) REFERENCES utilisateurs(username)
);

CREATE TABLE IF NOT EXISTS entreprises (
    id INTEGER PRIMARY KEY UNIQUE CHECK (id = 0),
    denomination_sociale VARCHAR(30) NOT NULL,
    numero_anpi VARCHAR(20) NOT NULL,
    date_creation DATE NOT NULL,
    forme_juridique VARCHAR(20) NOT NULL,
    numero_rmcc VARCHAR(20) NOT NULL,
    numero_cnss VARCHAR(20) NOT NULL,
    numero_nif VARCHAR(20) NOT NULL,
    numero_cnamgs VARCHAR(20) NOT NULL,
    capital_social NUMERIC NOT NULL,
    adresse_siege VARCHAR(20) NOT NULL,
    numero_tel VARCHAR(20) NOT NULL,
    email VARCHAR(20) NOT NULL
);

CREATE TABLE IF NOT EXISTS etablissements (
    id SERIAL PRIMARY KEY,
    id_entreprise INTEGER NOT NULL,
    forme_juridique VARCHAR(20) NOT NULL,
    date_creation DATE NOT NULL,
    numero_anpi VARCHAR(20) NOT NULL,
    numero_cnamgs VARCHAR(20) NOT NULL,
    numero_rmcc VARCHAR(20) NOT NULL,
    numero_cnss VARCHAR(20) NOT NULL,
    numero_nif VARCHAR(20) NOT NULL,
    adresse VARCHAR(20) NOT NULL,
    ville VARCHAR(20) NOT NULL,
    pays VARCHAR(20) NOT NULL,
    telephone VARCHAR(20) NOT NULL,
    email VARCHAR(20) NOT NULL
);

CREATE TABLE IF NOT EXISTS departements (
    nom VARCHAR(20) PRIMARY KEY NOT NULL
);

CREATE TABLE IF NOT EXISTS formations_suivies (
    id INTEGER PRIMARY KEY,
    id_salarie INTEGER,
    intitule TEXT NOT NULL,
    description TEXT,
    date_debut DATE,
    date_fin DATE,
    type TEXT,
    FOREIGN KEY(id_salarie) REFERENCES salaries(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS salarie_categorie (
    id INTEGER PRIMARY KEY,
    categorie TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS grille_salaires (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_categorie INTEGER NOT NULL UNIQUE,
    salaire_min REAL,
    salaire_max REAL,
    FOREIGN KEY(id_categorie) REFERENCES salarie_categorie(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS permis (
    numero TEXT NOT NULL,
    type TEXT NOT NULL,
    delivrance DATE NOT NULL,
    expiration DATE,
    id_salarie TEXT NOT NULL,
    FOREIGN KEY(id_salarie) REFERENCES salaries(matricule) ON DELETE CASCADE,
    CONSTRAINT UC_Permis UNIQUE (numero, type)
);

CREATE TABLE IF NOT EXISTS tokens (
    id INTEGER PRIMARY KEY CHECK (id = 0),
    access TEXT NOT NULL,
    session TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS emplois
(
    id INTEGER PRIMARY KEY,
    id_salarie INTEGER NOT NULL,
    poste TEXT NOT NULL,
    id_etablissement INTEGER NOT NULL,
    departement TEXT,
    id_responsable INTEGER,
    id_convention INTEGER,
    statut TEXT,
    categorie TEXT,
    date_signature DATE NOT NULL,
    date_debut DATE NOT NULL,
    date_fin DATE,
    salaire_categoriel NUMERIC,
    total_gain NUMERIC,
    type_salaire TEXT,
    type_stage TEXT,
    prestataire TEXT,
    motif_cdd TEXT,
    date_prev_fin INTEGER,
    motif_fin TEXT,
    nbr_jour_conges_mois INTEGER,
    jours_travail TEXT NOT NULL,
    heures_jour TEXT NOT NULL,
    heures_nuit TEXT NOT NULL,
    FOREIGN KEY(id_salarie) REFERENCES salaries(id) ON DELETE CASCADE,
    FOREIGN KEY(id_responsable) REFERENCES salaries(id) ON DELETE CASCADE,
    FOREIGN KEY(id_etablissement) REFERENCES etablissements(id) ON DELETE CASCADE,
    FOREIGN KEY(departement) REFERENCES departements(nom) ON DELETE CASCADE,
);

CREATE TABLE IF NOT EXISTS visites_medicales
(
    id INTEGER PRIMARY KEY,
    id_salarie INTEGER NOT NULL,
    type TEXT,
    periode INTEGER,
    date_visite DATE,
    FOREIGN KEY(id_salarie) REFERENCES salaries(id)
);

CREATE TABLE IF NOT EXISTS offline_request
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    req_time INTEGER DEFAULT CURRENT_TIMESTAMP NOT NULL,
    ressource TEXT NOT NULL,
    data TEXT
);

CREATE TABLE IF NOT EXISTS etablissements_departement(
    nom_departement VARCHAR(20) NOT NULL,
    id_etablissement INTEGER NOT NULL,
    FOREIGN KEY(id_etablissement) REFERENCES etablissements(id) ON DELETE CASCADE,
    FOREIGN KEY(nom_departement) REFERENCES departements(nom) ON DELETE CASCADE,
    CONSTRAINT UC_UM UNIQUE (id_etablissement, nom_departement)
);

CREATE TABLE IF NOT EXISTS jours_feries (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(20) NOT NULL,
    jour VARCHAR(20),
    CONSTRAINT UC_JS UNIQUE(nom, jour)
);

CREATE TABLE IF NOT EXISTS etablissements_jours_feries(
    id_etablissement INTEGER NOT NULL,
    id_jour INTEGER NOT NULL,
    FOREIGN KEY(id_etablissement) REFERENCES etablissements(id) ON DELETE CASCADE,
    FOREIGN KEY(id_jour) REFERENCES jours_feries(id) ON DELETE CASCADE,
    CONSTRAINT UC_UM UNIQUE (id_etablissement, id_jour)
);

CREATE TABLE IF NOT EXISTS salaries (
    id SERIAL PRIMARY KEY,
    matricule VARCHAR(20) NOT NULL UNIQUE,
    sexe INTEGER NOT NULL,
    nom VARCHAR(30),
    nom_marital VARCHAR(30),
    nom_usel VARCHAR(30),
    situation_familiale VARCHAR(30) NOT NULL,
    regime_matrimonial VARCHAR(30) NOT NULL,
    date_naissance DATE NOT NULL,
    ville_naissance VARCHAR(30) NOT NULL,
    pays_naissance VARCHAR(30) NOT NULL,
    pays_nationalite VARCHAR(30) NOT NULL,
    cnamgs VARCHAR(30),
    cnss VARCHAR(30),
    ascoma VARCHAR(30),
    date_vm_embauche DATE,
    taille_blouse VARCHAR(30),
    taille_pantalon VARCHAR(30),
    taille_chemise VARCHAR(30),
    pointure real,
    niveau_etude VARCHAR(30),
    diplome VARCHAR(30),
    specialisation VARCHAR(30),
    diplome_etablissement VARCHAR(30),
    diplome_date_obtention DATE
);

CREATE TABLE IF NOT EXISTS coordonnees
(
    id SERIAL PRIMARY KEY,
    id_salarie INTEGER NOT NULL UNIQUE,
    adresse VARCHAR(50),
    complement VARCHAR(50),
    bp INTEGER,
    ville VARCHAR(20),
    pays VARCHAR(20),
    fixe_dureau VARCHAR(20),
    fixe_domicile VARCHAR(20),
    phone VARCHAR(20),
    phone_pro VARCHAR(20),
    email VARCHAR(30),
    email_pro VARCHAR(30),
    nomUrgence VARCHAR(30),
    prenom_urgence VARCHAR(30),
    phone_urgence VARCHAR(20),
    email_urgence VARCHAR(30),
    FOREIGN KEY(id_salarie) REFERENCES salaries(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS absences
(
    id SERIAL PRIMARY KEY,
    id_salarie INTEGER NOT NULL,
    id_validateur INTEGER,
    id_responsable INTEGER,
    type VARCHAR(20) NOT NULL,
    date_debut VARCHAR(20) NOT NULL,
    date_validation VARCHAR(20),
    nbre_jours INTEGER NOT NULL,
    retour_theorique VARCHAR(20) NOT NULL,
    date_retour VARCHAR(20),
    commentaire VARCHAR(50),
    status INTEGER default 0,
    FOREIGN KEY(id_salarie) REFERENCES salaries(id),
    FOREIGN KEY(id_validateur) REFERENCES salaries(id)
); 

CREATE TABLE IF NOT EXISTS enfants (
    id SERIAL PRIMARY KEY,
    parent_id INTEGER NOT NULL,
    nom VARCHAR(30) NOT NULL,
    prenom VARCHAR(30) NOT NULL,
    naissance DATE NOT NULL,
    sexe INTEGER NOT NULL CHECK (sexe IN (1, 2)), -- 1: Homme, 2: Femme
    FOREIGN KEY(parent_id) REFERENCES salaries(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS articles (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL,
    description VARCHAR(100) NOT NULL,
    code VARCHAR(20),
    quantite INTEGER,
    unite VARCHAR(20),
    has_alert boolean,
    seuil_alert INTEGER,
    prix NUMERIC NOT NULL,
    tva NUMERIC NOT NULL
);

CREATE TABLE IF NOT EXISTS articles_categories(
    id_article INTEGER NOT NULL,
    id_categorie INTEGER NOT NULL,
    FOREIGN KEY(id_article) REFERENCES articles(id) ON DELETE CASCADE,
    FOREIGN KEY(id_categorie) REFERENCES artCategories(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS artCategories(
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL,
    categorie_parent_id INTEGER,
    FOREIGN KEY(categorie_parent_id) REFERENCES artCategories(id)
);

CREATE TABLE IF NOT EXISTS articles_fournisseurs(
    id SERIAL PRIMARY KEY,
    id_article INTEGER NOT NULL,
    id_fournisseur INTEGER NOT NULL,
    price DECIMAL,
    tva DECIMAL NOT NULL,
    FOREIGN KEY(id_article) REFERENCES articles(id) ON DELETE CASCADE,
    FOREIGN KEY(id_fournisseur) REFERENCES fournisseurs(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS achats (
    id SERIAL PRIMARY KEY,
    libelle VARCHAR(30),
    date_creation DATE NOT NULL,
    editeur VARCHAR(30) NOT NULL,
    statut INTEGER NOT NULL,
    id_fournisseur INTEGER NOT NULL,
    facture VARCHAR(100),
    FOREIGN KEY(editeur) REFERENCES utilisateurs(username) ON DELETE CASCADE,
    FOREIGN KEY(id_fournisseur) REFERENCES fournisseurs(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS lignes_achats (
    id SERIAL PRIMARY KEY,
    id_achat INTEGER NOT NULL,
    id_article INTEGER NOT NULL,
    quantite INTEGER NOT NULL,
    prix_unitaire NUMERIC NOT NULL,
    tva NUMERIC NOT NULL,
    FOREIGN KEY(id_achat) REFERENCES achats(id) ON DELETE CASCADE,
    FOREIGN KEY(id_article) REFERENCES articles(id) ON DELETE CASCADE,
    UNIQUE(id_achat, id_article)
);

CREATE TABLE IF NOT EXISTS flow_achat(
    id_achat INTEGER NOT NULL,
    statut INTEGER NOT NULL,
    update_date INTEGER NOT NULL,
    editeur VARCHAR(30) NOT NULL,
    FOREIGN KEY(id_achat) REFERENCES achats(id) ON DELETE CASCADE,
    FOREIGN KEY(editeur) REFERENCES utilisateurs(username) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS inventaires (
    id SERIAL PRIMARY KEY,
    libelle VARCHAR(30),
    date_creation DATE NOT NULL,
    editeurVARCHAR(30) NOT NULL,
    validateur VARCHAR(30),
    date_validation INTEGER,
    statut INTEGER NOT NULL,
    FOREIGN KEY(editeur) REFERENCES utilisateurs(username) ON DELETE CASCADE,
    FOREIGN KEY(validateur) REFERENCES utilisateurs(username) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS lignes_inventaires(
    id_inventaire INTEGER NOT NULL,
    id_article INTEGER NOT NULL,
    quantite INTEGER NOT NULL,
    FOREIGN KEY(id_inventaire) REFERENCES inventaires(id),
    FOREIGN KEY(id_article) REFERENCES articles(id) ON DELETE CASCADE,
);

CREATE TABLE IF NOT EXISTS fournisseurs (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL,
    adresse VARCHAR(30),
    quartier VARCHAR(30),
    ville VARCHAR(30),
    pays VARCHAR(30),
    telephone NUMERIC NOT NULL,
    email TEXT UNIQUE,
    statut INTEGER NOT NULL,
    devise VARCHAR(30) NOT NULL
);

CREATE TABLE IF NOT EXISTS fournisseurs_categories(
    id_fournisseur INTEGER NOT NULL,
    id_categorie INTEGER NOT NULL,
    FOREIGN KEY(id_fournisseur) REFERENCES fournisseurs(id) ON DELETE CASCADE,
    FOREIGN KEY(id_categorie) REFERENCES FourCategories(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS commandes (
    id SERIAL PRIMARY KEY,
    libelle VARCHAR(30),
    date_creation DATE NOT NULL,
    editeur  VARCHAR(30) NOT NULL,
    statut INTEGER NOT NULL,
    id_client INTEGER NOT NULL,
    rabais NUMERIC,
    facture VARCHAR(100),
    adresse  VARCHAR(30),
    ville VARCHAR(30),
    pays VARCHAR(30),
    email VARCHAR(30),
    tel VARCHAR(30),
    FOREIGN KEY(editeur) REFERENCES utilisateurs(username) ON DELETE CASCADE,
    FOREIGN KEY(id_client) REFERENCES clients(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS flow_commande(
    id_commande INTEGER NOT NULL,
    statut INTEGER NOT NULL,
    update_date INTEGER NOT NULL,
    editeur VARCHAR(30) NOT NULL,
    FOREIGN KEY(id_commande) REFERENCES commandes(id) ON DELETE CASCADE,
    FOREIGN KEY(editeur) REFERENCES utilisateurs(username) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS lignes_commandes (
    id SERIAL PRIMARY KEY,
    id_commande INTEGER NOT NULL,
    id_article INTEGER NOT NULL,
    quantite INTEGER NOT NULL,
    prix_unitaire NUMERIC NOT NULL,
    tva NUMERIC NOT NULL,
    FOREIGN KEY(id_commande) REFERENCES commandes(id) ON DELETE CASCADE,
    FOREIGN KEY(id_article) REFERENCES articles(id) ON DELETE CASCADE,
    UNIQUE(id_commande, id_article)
);

CREATE TABLE IF NOT EXISTS produits (
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL,
    description VARCHAR(100),
    quantite INTEGER,
    prix NUMERIC NOT NULL,
    tva NUMERIC NOT NULL,
    id_article INTEGER,
    FOREIGN KEY(id_article) REFERENCES articles(id) ON DELETE CASCADE
);

XISTS produits_categories(
    id_produit INTEGER NOT NULL,
    id_categorie INTEGER NOT NULL,
    FOREIGN KEY(id_produit) REFERENCES produits(id),
    FOREIGN KEY(id_cat
    egorie) REFERENCES prodCategories(id)
);

CREATE TABLE IF NOT EXISTS fourCategories(
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL
);

CREATE TABLE IF NOT EXISTS prodCategories(
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL,
    categorie_parent_id INTEGER,
    FOREIGN KEY(categorie_parent_id) REFERENCES prodCategories(id)
);


CREATE TABLE IF NOT EXISTS fournisseurs_categories(
    id_fournisseur INTEGER NOT NULL,
    id_categorie INTEGER NOT NULL,
    FOREIGN KEY(id_fournisseur) REFERENCES fournisseurs(id) ON DELETE CASCADE,
    FOREIGN KEY(id_categorie) REFERENCES FourCategories(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS produits_categories(
    id_produit INTEGER NOT NULL,
    id_categorie INTEGER NOT NULL,
    FOREIGN KEY(id_produit) REFERENCES produits(id),
    FOREIGN KEY(id_categorie) REFERENCES prodCategories(id)
);

CREATE TABLE IF NOT EXISTS salles(
    id SERIAL PRIMARY KEY,
    nom VARCHAR(30) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS tables(
    id SERIAL PRIMARY KEY,
    id_salle INTEGER NOT NULL,
    nom VARCHAR(30) NOT NULL,
    nbr_place INTEGER NOT NULL,
    FOREIGN KEY(id_salle) REFERENCES salles(id),
    CONSTRAINT c_tables UNIQUE (id_salle, nom)
);

CREATE TABLE IF NOT EXISTS commandes_ventes(
    id SERIAL PRIMARY KEY,
    id_salle INTEGER NOT NULL,
    nom VARCHAR(30) NOT NULL,
    nbr_place INTEGER NOT NULL,
    FOREIGN KEY(id_salle) REFERENCES salles(id),
    CONSTRAINT c_tables UNIQUE (id_salle, nom)
);


CREATE TABLE code_etablissement (
    id SERIAL PRIMARY KEY,
    code_etablissement VARCHAR(255) NOT NULL
);