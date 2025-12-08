
CREATE EXTENSION IF NOT EXISTS pgcrypto;

CREATE OR REPLACE FUNCTION digestpassword(pwd varchar)
RETURNS varchar AS $$
declare pwd2 varchar;
BEGIN
    pwd2:= CONCAT(pwd,'I2TECH');
    RETURN crypt(pwd2, gen_salt('bf', 8));
END;
$$
LANGUAGE plpgsql;

--Fonction Add User
CREATE OR REPLACE FUNCTION Adduser(usernamei varchar, sexei integer, nomi varchar, prenomi varchar, emaili varchar, password varchar, profili varchar)
RETURNS TABLE ( status BOOLEAN, username varchar) AS $$
DECLARE
    status boolean;
    existe boolean;
BEGIN
   --verification du username email et telephone
   select exists ( select 1 from utilisateurs u where u.username = usernamei) into existe;
   if existe then 
    status:= false;
    return query select status, username; 
   else
    --insertion de l'utilisateur
    insert into utilisateurs (username, sexe, nom, prenom, email, psswrd, profil)  values (usernamei, sexei, nomi, prenomi, emaili, digestpassword(password), profili);
    status := true;
    return query select status, usernamei;
   end if;
END;
$$
LANGUAGE plpgsql;

SELECT * FROM Adduser('i2tech_dev', 1, 'i2tech', 'dev', 'contact@i2tech-ga.com', 'i2tech@IsaleDev', 'dev');

CREATE OR REPLACE FUNCTION UpdatePassword(usernamei varchar, password varchar)
RETURNS BOOLEAN AS $$
DECLARE
    status boolean;
    existe boolean;
    pwd2 varchar;
BEGIN
    pwd2:= CONCAT(password,'I2TECH');
    update utilisateurs  set psswrd = digestpassword(password) where username = usernamei;
    select exists ( select 1 from utilisateurs u where u.username = usernamei and psswrd = crypt(pwd2, u.psswrd)) into existe;
    if existe then
        UPDATE utilisateurs SET status = 2 WHERE username = usernamei;
        return true;
   else
     return false;
   end if;
END;
$$
LANGUAGE plpgsql;


--Fonction getUser
CREATE OR REPLACE FUNCTION GetUserByUsernamePassword(usern varchar(100), pssw varchar(100))
RETURNS table(username varchar, nom varchar, prenom varchar, email varchar, sexe smallint, profil varchar, status smallint) AS $$
DECLARE
    recordt record;
    pwd2 varchar;
BEGIN 
    pwd2:= CONCAT(pssw,'I2TECH');
    select * into recordt from utilisateurs u where u.username = usern and u.psswrd = crypt(pwd2, u.psswrd);
    RETURN query select recordt.username, recordt.nom, recordt.prenom, recordt.email, recordt.sexe, recordt.profil, recordt.status;
END;
$$
LANGUAGE plpgsql;

-- Fonction AddSalarie 
CREATE OR REPLACE FUNCTION AddSalarie(
    matricule_input VARCHAR,
    sexe_input INTEGER,
    nom_input VARCHAR,
    nom_marital_input VARCHAR,
    nom_usel_input VARCHAR,
    situation_familiale_input VARCHAR,
    regime_matrimonial_input VARCHAR,
    date_naissance_input DATE,
    ville_naissance_input VARCHAR,
    pays_naissance_input VARCHAR,
    pays_nationalite_input VARCHAR,
    cnamgs_input VARCHAR,
    cnss_input VARCHAR,
    ascoma_input VARCHAR,
    date_vm_embauche_input DATE,
    taille_blouse_input VARCHAR,
    taille_pantalon_input VARCHAR,
    taille_chemise_input VARCHAR,
    pointure_input REAL,
    niveau_etude_input VARCHAR,
    diplome_input VARCHAR,
    specialisation_input VARCHAR,
    diplome_etablissement_input VARCHAR,
    diplome_date_obtention_input DATE
)
RETURNS TABLE (status BOOLEAN) AS $$
DECLARE
    existe BOOLEAN;
BEGIN
    -- Vérification de l'existence du matricule
    SELECT EXISTS (
        SELECT 1 FROM salaries s WHERE s.matricule = matricule_input
    ) INTO existe;

    IF existe THEN
        RETURN QUERY SELECT FALSE;
    ELSE
        INSERT INTO salaries (
            matricule, sexe, nom, nom_marital, nom_usel, situation_familiale, regime_matrimonial,
            date_naissance, ville_naissance, pays_naissance, pays_nationalite,
            cnamgs, cnss, ascoma, date_vm_embauche,
            taille_blouse, taille_pantalon, taille_chemise, pointure,
            niveau_etude, diplome, specialisation, diplome_etablissement, diplome_date_obtention
        )
        VALUES (
            matricule_input, sexe_input, nom_input, nom_marital_input, nom_usel_input,
            situation_familiale_input, regime_matrimonial_input, date_naissance_input,
            ville_naissance_input, pays_naissance_input, pays_nationalite_input,
            cnamgs_input, cnss_input, ascoma_input, date_vm_embauche_input,
            taille_blouse_input, taille_pantalon_input, taille_chemise_input, pointure_input,
            niveau_etude_input, diplome_input, specialisation_input,
            diplome_etablissement_input, diplome_date_obtention_input
        );

        RETURN QUERY SELECT TRUE;
    END IF;
END;
$$ LANGUAGE plpgsql;

--Fonction categorie_salarie
CREATE OR REPLACE FUNCTION insert_categorie_salarie(p_id INTEGER, p_categorie TEXT)
RETURNS VOID AS $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM salarie_categorie WHERE id = p_id) THEN
        INSERT INTO salarie_categorie (id, categorie) VALUES (p_id, p_categorie);
    END IF;
END;
$$ LANGUAGE plpgsql;

--Fonction grille_salaire
CREATE OR REPLACE FUNCTION insert_grille_salaire(p_id INTEGER, p_salaire_min REAL, p_salaire_max REAL)
RETURNS VOID AS $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM grille_salaires WHERE id_categorie = p_id) THEN
        INSERT INTO grille_salaires (id_categorie, salaire_min, salaire_max)
        VALUES (p_id, p_salaire_min, p_salaire_max);
    END IF;
END;
$$ LANGUAGE plpgsql;

--Fonction departement
CREATE OR REPLACE FUNCTION insert_departement(p_nom VARCHAR)
RETURNS VOID AS $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM departements WHERE nom = p_nom) THEN
        INSERT INTO departements (nom) VALUES (p_nom);
    END IF;
END;
$$ LANGUAGE plpgsql;


--Fonction lier_departement_etablissement
CREATE OR REPLACE FUNCTION lier_departement_etablissement(p_nom_departement VARCHAR, p_id_etablissement INTEGER)
RETURNS VOID AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM etablissements_departement
        WHERE nom_departement = p_nom_departement AND id_etablissement = p_id_etablissement
    ) THEN
        INSERT INTO etablissements_departement (nom_departement, id_etablissement)
        VALUES (p_nom_departement, p_id_etablissement);
    END IF;
END;
$$ LANGUAGE plpgsql;


--Fonction insert_absence 
CREATE OR REPLACE FUNCTION insert_absence(
    p_id_salarie INTEGER,
    p_id_validateur INTEGER,
    p_id_responsable INTEGER,
    p_type VARCHAR,
    p_date_debut VARCHAR,
    p_date_validation VARCHAR,
    p_nbre_jours INTEGER,
    p_retour_theorique VARCHAR,
    p_date_retour VARCHAR,
    p_commentaire VARCHAR,
    p_status INTEGER
)
RETURNS VOID AS $$
BEGIN
    INSERT INTO absences(
        id_salarie,
        id_validateur,
        id_responsable,
        type,
        date_debut,
        date_validation,
        nbre_jours,
        retour_theorique,
        date_retour,
        commentaire,
        status
    )
    VALUES (
        p_id_salarie,
        p_id_validateur,
        p_id_responsable,
        p_type,
        p_date_debut,
        p_date_validation,
        p_nbre_jours,
        p_retour_theorique,
        p_date_retour,
        p_commentaire,
        p_status
    );
END;
$$ LANGUAGE plpgsql;

--Fonction AddEnfant
CREATE OR REPLACE FUNCTION AddEnfant(
    p_matricule VARCHAR,
    p_nom VARCHAR,
    p_prenom VARCHAR,
    p_naissance DATE,
    p_sexe INTEGER
)
RETURNS VOID AS $$
BEGIN
    INSERT INTO enfants(matricule, nom, prenom, naissance, sexe)
    VALUES (p_matricule, p_nom, p_prenom, p_naissance, p_sexe);
END;
$$ LANGUAGE plpgsql;

--Fonction addPermis
CREATE OR REPLACE FUNCTION AddPermis(
    p_matricule VARCHAR,
    p_type VARCHAR,
    p_numero VARCHAR,
    p_debut DATE,
    p_fin DATE
)
RETURNS VOID AS $$
BEGIN
    INSERT INTO permis(matricule, type, numero, debut, fin)
    VALUES (p_matricule, p_type, p_numero, p_debut, p_fin);
END;
$$ LANGUAGE plpgsql;

--Fonction AddOrUpdateCoordonnees
CREATE OR REPLACE FUNCTION AddOrUpdateCoordonnees(
    p_id_salarie INTEGER,
    p_adresse VARCHAR,
    p_complement VARCHAR DEFAULT NULL,
    p_bp INTEGER DEFAULT NULL,
    p_ville VARCHAR DEFAULT NULL,
    p_pays VARCHAR DEFAULT NULL,
    p_fixe_dureau VARCHAR DEFAULT NULL,
    p_fixe_domicile VARCHAR DEFAULT NULL,
    p_phone VARCHAR DEFAULT NULL,
    p_phone_pro VARCHAR DEFAULT NULL,
    p_email VARCHAR DEFAULT NULL,
    p_email_pro VARCHAR DEFAULT NULL,
    p_nomUrgence VARCHAR DEFAULT NULL,
    p_prenom_urgence VARCHAR DEFAULT NULL,
    p_phone_urgence VARCHAR DEFAULT NULL,
    p_email_urgence VARCHAR DEFAULT NULL
)

RETURNS BOOLEAN AS $$
BEGIN
    -- Vérifie si une entrée existe déjà pour ce salarié
    IF EXISTS (SELECT 1 FROM coordonnees WHERE id_salarie = p_id_salarie) THEN
        -- Mise à jour
        UPDATE coordonnees SET
            adresse = p_adresse,
            complement = p_complement,
            bp = p_bp,
            ville = p_ville,
            pays = p_pays,
            fixe_dureau = p_fixe_dureau,
            fixe_domicile = p_fixe_domicile,
            phone = p_phone,
            phone_pro = p_phone_pro,
            email = p_email,
            email_pro = p_email_pro,
            nomUrgence = p_nomUrgence,
            prenom_urgence = p_prenom_urgence,
            phone_urgence = p_phone_urgence,
            email_urgence = p_email_urgence
        WHERE id_salarie = p_id_salarie;
    ELSE
        -- Insertion
        INSERT INTO coordonnees (
            id_salarie, adresse, complement, bp, ville, pays,
            fixe_dureau, fixe_domicile, phone, phone_pro,
            email, email_pro, nomUrgence, prenom_urgence,
            phone_urgence, email_urgence
        ) VALUES (
            p_id_salarie, p_adresse, p_complement, p_bp, p_ville, p_pays,
            p_fixe_dureau, p_fixe_domicile, p_phone, p_phone_pro,
            p_email, p_email_pro, p_nomUrgence, p_prenom_urgence,
            p_phone_urgence, p_email_urgence
        );
    END IF;

    RETURN TRUE;
EXCEPTION WHEN OTHERS THEN
    RETURN FALSE;
END;
$$ LANGUAGE plpgsql;


--Fonction changeProfil
CREATE OR REPLACE FUNCTION ChangeProfil(usernamei varchar, profili varchar(20))
RETURNS boolean AS $$
DECLARE  cc int;
BEGIN select count(*) into cc from profil where pk_utilisateur = pk_utilisateuri;
    if cc=1 then
       update profil set profil = profili where pk_utilisateur = pk_utilisateuri;
       return true;
    else
       return false;
    end if;
END;
$$
LANGUAGE plpgsql;

--Supprimer utilisateur
CREATE OR REPLACE FUNCTION SupprimerUser(par_nom VARCHAR(100), par_prenom VARCHAR(100))
RETURNS VOID AS $$
DECLARE identifianti varchar;
BEGIN
    select identifiant into identifianti from utilisateurs where nom= par_nom and prenom= par_prenom; 
    delete from profil
    where pk_utilisateur = identifianti;
    delete from utilisateurs where nom = par_nom and prenom = par_prenom;
END;
$$ LANGUAGE plpgsql;

