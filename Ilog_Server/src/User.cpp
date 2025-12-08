#include <User.h>
#include <jsoncpp/json/json.h>

namespace ILogServer
{

User::User() {}

User::User(int sexe, const std::string& nom,
            const std::string& prenom, const std::string& username,
            const std::string& email, int statut,
            const std::string&  profil)
{
    nom_ = nom;
    prenom_ = prenom;
    username_ = username;
    email_ = email;
    sexe_ = sexe;
    profil_ = UserProfilStringifier.fromString(profil);
    statut_ = UserStatus(statut);
}

User::User(const std::string& userJson)
{
    Json::Value body;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(userJson, body);
    if ( !parsingSuccessful )
    {
        throw std::runtime_error("invalid user data json format");
    }

    Json::Value json = Json::objectValue;
    nom_ = body["nom"].asString();
    prenom_ = body["prenom"].asString();
    username_ = body["username"].asString();
    email_ = body["email"].asString();
    profil_ = (UserProfil)body["profil"].asUInt();
    sexe_ = body["sexe"].asUInt();
    statut_ = (UserStatus)body["status"].asUInt();
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
    json["nom"] = nom_;
    json["prenom"] = prenom_;
    json["username"] = username_;
    json["email"] = email_;
    json["profil"] = profil_;
    json["sexe"] = sexe_;
    json["status"] = statut_;
    return json.toStyledString();
}
 
} // namespace CarflowServer
