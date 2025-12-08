#pragma once
#include <Common/Database/IPostgreSQL.h>
#include <json/json.h>


namespace ILogServer
{
namespace RestApiHandler
{
static inline Json::Value fetchEntreprises(Common::Database::IPostgreSQL* psql, const int idEntreprise = -1)
{
    Json::Value arr = Json::arrayValue;
    std::string query;

    if (idEntreprise == -1)
        query = "SELECT * FROM entreprises;";
    else
        query = "SELECT * FROM entreprises WHERE id = '" + std::to_string(idEntreprise) + "';";


    auto res = psql->processQuery(query);
    
    for (const auto& item : res)
    {
        Json::Value json;
        json["id"] = item[0].is_null() ? "" : item[0].c_str();
        json["denomination_sociale"] = item[1].is_null() ? "" : item[1].c_str();
        json["numero_anpi"] = item[2].is_null() ? "" : item[2].c_str();
        json["date_creation"] = item[3].is_null() ? "" : item[3].c_str();
        json["forme_juridique"] = item[4].is_null() ? "" : item[4].c_str();
        json["numero_rmcc"] = item[5].is_null() ? "" : item[5].c_str();
        json["numero_cnss"] = item[6].is_null() ? "" : item[6].c_str();
        json["numero_nif"] = item[7].is_null() ? "" : item[7].c_str();
        json["numero_cnamgs"] = item[8].is_null() ? "" : item[8].c_str();
        json["capital_social"] = item[9].is_null() ? "" : item[9].c_str();
        json["adresse_siege"] = item[10].is_null() ? "" : item[10].c_str();
        json["numero_tel"] = item[11].is_null() ? "" : item[11].c_str();
        json["email"] = item[12].is_null() ? "" : item[12].c_str();
        arr.append(json);
    }
    return arr;
}

}
}