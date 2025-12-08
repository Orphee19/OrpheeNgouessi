#pragma once

#include <string>

namespace ILogServer
{
namespace RestApiHandler
{
struct Salarie
{   
    int _id;
    std::string _matricule;
    std::string _civilite;
    std::string _nom;
    std::string _prenom;
    std::string _email;
    std::string _emailPro;
};
}
}
