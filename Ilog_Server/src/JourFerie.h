#pragma once

#include <Common/Database/IPostgreSQL.h>
#include <Common/Logger/Logger.h>


namespace ILogServer
{
int addJourFeries(const std::string& nom, const std::string& jour,
                    Common::Database::IPostgreSQL* psql)
{
    std::string query = "INSERT INTO jours_feries(nom, jour)  VALUES('" + nom + "', '" + jour + "')"
                            "ON CONFLICT (nom, jour) DO UPDATE set nom = '" + nom + "' RETURNING id;";

    auto res = psql->processQuery(query);
    if (res.empty())
    {
        LOG_ERR("Failed to inser jour ferie %s, %s", nom, jour);
        return -1;
    }

    return res[0][0].as<int>();
}

void linkEtablissementJoursFerie(const int idEt, const int idJ, Common::Database::IPostgreSQL* psql)
{
    std::string query = "INSERT INTO etablissements_jours_feries(id_etablissement, id_jour) "
                        "VALUES(" + std::to_string(idEt) + ", " + std::to_string(idJ) + ");";

    auto res = psql->processQuery(query);
}
}



