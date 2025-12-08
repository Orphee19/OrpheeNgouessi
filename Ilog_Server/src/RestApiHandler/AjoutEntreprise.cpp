#include <RestApiHandler/AjoutEntreprise.h>
#include <Common/Utils/Utils.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Database/IPostgreSQL.h>
#include <iostream>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Network;
using namespace Common::Utils;

HttpResponse AjoutEntreprise::process(HttpRequest* req)
{
    auto psql_ = getPSQL();
    if (psql_ == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible. Réessayez plus tard");
    }

    Json::Value requestBody = req->getBody();
    try
    {
        std::string denominationSociale = requestBody["denominationSociale"].asString();
        std::string numeroANPI = requestBody["numeroANPI"].asString();
        std::string dateCreation = requestBody["dateCreation"].asString();
        std::string formeJuridique = requestBody["formeJuridique"].asString();
        std::string numeroRMCC = requestBody["numeroRMCC"].asString();
        std::string numeroCNSS = requestBody["numeroCNSS"].asString();
        std::string numeroNIF = requestBody["numeroNIF"].asString();
        std::string numeroCNAMGS = requestBody["numeroCNAMGS"].asString();
        std::string capitalSocial = requestBody["capitalSocial"].asString();
        std::string adresseSiege = requestBody["adresseSiege"].asString();
        std::string numeroTel = requestBody["numeroTel"].asString();
        std::string email = requestBody["email"].asString();
        std::string ville = requestBody["ville"].asString();
        std::string pays = requestBody["pays"].asString();

        std::string query = "INSERT INTO entreprises (id, denomination_sociale, numero_anpi,"
                            "date_creation, forme_juridique, numero_rmcc, numero_cnss, numero_nif, "
                            " numero_cnamgs, capital_social, adresse_siege, numero_tel, email, ville, pays)"
                            " VALUES (0, '" + denominationSociale + "', '" + numeroANPI + "', '"
                + dateCreation + "', '" + formeJuridique + "', '" + numeroRMCC + "', '"
                + numeroCNSS + "', '" + numeroNIF + "', '" + numeroCNAMGS + "', '" 
                + capitalSocial + "', '" + adresseSiege + "', '" + numeroTel + "', '" 
                + email + "', '" + ville + "', '" + pays + "') ON CONFLICT DO NOTHING;";

        auto res = psql_->processQuery(query);
        if (res.empty())
        {
            auto ws = server_ != nullptr ? server_->getWSNotification() : nullptr;
            if (ws != nullptr)
            {
                requestBody["id"] = 0;
                ws->sendToAll("addEntreprise", Json::FastWriter().write(requestBody));
            }
            return HttpResponse(ResponseCode::OK, "{\"message\": \"Entreprise ajoutée/mise à jour avec succès\"}");
        }
        else
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, "Erreur lors de l'ajout/mise à jour de l'entreprise");
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Données invalides ou manquantes");
    }
}
}
}
