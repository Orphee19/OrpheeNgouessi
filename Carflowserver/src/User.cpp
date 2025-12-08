#include <User.h>
#include <jsoncpp/json/json.h>

namespace CarflowServer
{

User::User() {}

User::User(const std::string& identifiant, int sexe, const std::string& nom,
            const std::string& prenom, const std::string& username, const std::string& naissance,
            const std::string& email, const std::string& phone, const std::string& contact, int statut,
            const std::string&  profil, const std::string& carte, bool has_photo, bool has_identite)
{
    identifiant_ = identifiant;
    nom_ = nom;
    prenom_ = prenom;
    username_ = username;
    naissance_ = naissance;
    email_ = email;
    phone_ = phone;
    contact_ = contact;
    carte_ = carte;
    sexe_ = sexe;
    profil_ = UserProfilStringifier.fromString(profil);
    statut_ = UserStatus(statut);
    hasPhoto_ = has_photo;
    hasIdentite_ = has_identite;
}

bool User::isValidAccount()
{
    if (statut_ == UserStatus::VALIDATE)
    {
        return true;
    }

    return false;
}

const std::string User::getRawJson() const
{
    Json::Value json = Json::objectValue;
    json["identifiant"] = identifiant_;
    json["nom"] = nom_;
    json["prenom"] = prenom_;
    json["username"] = username_;
    json["naissance"] = naissance_;
    json["email"] = email_;
    json["phone"] = phone_;
    json["contact"] = contact_;
    json["carte"] = carte_;
    json["profil"] = profil_;
    json["sexe"] = sexe_;
    json["has_photo"] = hasPhoto_;
    json["has_identite"] = hasIdentite_;
    return json.toStyledString();
}
 
} // namespace CarflowServer
