#pragma once

#include <Common/Utils/EnumStringifier.h>

namespace ILogServer
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
    AGENT = 1,
    ADMIN = 2,
    DEV = 3
};

const Common::Utils::EnumStringifier<UserProfil> UserProfilStringifier
{
    {
        {UserProfil::USER, "user"},
        {UserProfil::AGENT, "agent"},
        {UserProfil::ADMIN, "admin"},
        {UserProfil::DEV, "dev"},
    }
};

class User
{
public:
    std::string nom_, prenom_, username_, email_;
    int sexe_;
    UserProfil profil_;
    UserStatus statut_;

public:
    User();
    User(int sexe, const std::string& nom,
            const std::string& prenom, const std::string& username,
            const std::string& email, int status,
            const std::string& profil);
    User(const std::string& userJson);

    UserStatus getStatus () const { return statut_; }

    bool isValidAccount();
    const std::string getRawJson() const;
};
    
} // namespace CarflowServer
