#pragma once

#include <Common/Database/IPostgreSQL.h> 


namespace ILogServer
{
void addDepartement(const std::string& dep, Common::Database::IPostgreSQL* psql)
{
    std::string query = "INSERT INTO departements(nom)  VALUES('" + dep + "') ON CONFLICT DO NOTHING;";
    
    auto res = psql->processQuery(query);
}

void linkEtablissementDepartement(const int id, const std::string& dep, Common::Database::IPostgreSQL* psql)
{
    std::string query = "INSERT INTO etablissements_departement(nom_departement, id_etablissement) "
              "VALUES ('" + dep + "', '" + std::to_string(id) + "') ON CONFLICT DO NOTHING;";

    auto res = psql->processQuery(query);
}
}