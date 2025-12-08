#pragma once

#include <Common/Utils/EnumStringifier.h>

namespace CarflowServer
{
enum UserStatus
{
    CREATED = 1,
    VALIDATE = 2,
    SUPRESSING = 3,
    SUPPRESSED = 4
};

enum UserProfil
{
    USER = 0,
    CONTROLEUR = 1,
    AGENT = 2,
    ADMIN = 3,
};

const Common::Utils::EnumStringifier<UserProfil> UserProfilStringifier
{
    {
        {UserProfil::USER, "user"},
        {UserProfil::CONTROLEUR, "controleur"},
        {UserProfil::AGENT, "agent"},
        {UserProfil::ADMIN, "admin"},
    }
};

class User
{
public:
    std::string identifiant_, nom_, prenom_, username_, naissance_, email_, phone_, contact_, carte_;
    int sexe_;
    UserProfil profil_;
    UserStatus statut_;
    bool hasPhoto_, hasIdentite_;

public:
    User();
    User(const std::string& identifiant, int sexe, const std::string& nom,
            const std::string& prenom, const std::string& username, const std::string& naissance,
            const std::string& email, const std::string& phone, const std::string& contact, int status,
            const std::string& profil, const std::string& carte, bool has_photo, bool has_identite);

    bool isValidAccount();
    const std::string getRawJson() const;
};
    
} // namespace CarflowServer
