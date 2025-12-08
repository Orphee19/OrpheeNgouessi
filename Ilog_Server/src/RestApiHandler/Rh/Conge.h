#pragma once

#include <string>

namespace ILogServer
{
namespace RestApiHandler
{
struct Conge
{
    int _id;
    std::string _type;
    std::string _debut;
    int _nbrJour;
    std::string _finTheorique;
    std::string _fin;
    std::string _dateDebut;
    std::string _dateRetour;
    std::string _commentaire;
    std::string dateValidation;

};
}
}
