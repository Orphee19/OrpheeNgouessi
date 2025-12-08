#pragma once

#include <string>
#include <RestApiHandler/Salarie.h>
#include <RestApiHandler/Rh/Conge.h>

namespace ILogServer
{
namespace RestApiHandler
{
class TemplateEmail
{
public:
  static std::string demandeConge(
    const Salarie& demandeur,
    const Salarie& responsable,
    const Conge& conge,
    const std::string& sToken); 
};
}
}
