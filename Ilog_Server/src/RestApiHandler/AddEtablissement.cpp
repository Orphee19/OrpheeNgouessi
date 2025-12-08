#include <RestApiHandler/AddEtablissement.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Logger/Logger.h>
#include <Departement.h>
#include <JourFerie.h>
#include <User.h>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse AddEtablissement::process(HttpRequest* req)
{
    auto psql_ = getPSQL();
    auto sessionMngr = getSessionMngr();

    if (psql_ == nullptr || sessionMngr == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible. Réessayez plus tard");
    }

    std::string sessionToken = req->getHeader().bearer;
    if (sessionToken.empty())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
    }

    Json::Value requestBody;
    try
    {
        requestBody = req->getBody();
    }
    catch (const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    std::string username_;
    try
    {
        auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
        if (Token::isTokenTimeout(decoded))
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");

        Json::Value body;
        Json::Reader reader;
        reader.parse(decoded.get_payload_claim("data").as_string(), body);

        username_ = body["username"].asString();
    }
    catch (const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    try
    {
        std::string query = "SELECT profil, status FROM utilisateur WHERE username = '" + username_ + "';";
        auto res = psql_->processQuery(query);

        if (res.empty() || res[0][0].as<std::string>().empty())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
        }

        auto userProfil = UserProfilStringifier.fromString(res[0][0].as<std::string>());
        UserStatus userStatus = UserStatus(res[0][1].as<int>());

        if (userProfil < UserProfil::AGENT || userStatus != UserStatus::VALIDATE)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
        }
    }
    catch (const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Erreur lors de la vérification des autorisations");
    }

    try
    {

        std::string query = "INSERT INTO etablissements(id_entreprise, forme_juridique, "
                "date_creation, numero_anpi, numero_cnamgs, numero_rmcc, numero_cnss, numero_nif"
                "adresse, ville, pays, telephone, email) VALUES ('" +
                std::to_string(requestBody["idEntreprise"].asInt()) + ", '" +
                requestBody["nom"].asString() + "', " +
                requestBody["formeJuridique"].asString() + "', '" +
                requestBody["dateCreation"].asString() + "', '" +
                requestBody["numeroAnpi"].asString() + "', '" +
                requestBody["numeroCnamgs"].asString() + "', '" +
                requestBody["numeroRmcc"].asString() + "', '" +
                requestBody["numeroCnss"].asString() + "', '" +
                requestBody["numeroNif"].asString() + "', '" +
                requestBody["adresse"].asString() + "', '" +
                requestBody["ville"].asString() + "', '" +
                requestBody["pays"].asString() + "', '" +
                requestBody["numeroTel"].asString() + "', '" +
                requestBody["email"].asString() + "') RETURNING id;";

        auto res = psql_->processQuery(query);
        if (res.empty())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Échec de l'ajout de l'établissement");
        }

        int id = res[0][0].as<int>();

        for (const auto& dep : requestBody["departements"])
        {
            const std::string departement = dep.asString();
            addDepartement(departement, psql_);
            linkEtablissementDepartement(id , departement, psql_);
        }
        
        for (const auto& jf : requestBody["joursFerie"])
        {
            int idJ = addJourFeries(jf["nom"].asString(), jf["jour"].asString(), psql_);
            linkEtablissementJoursFerie(id, idJ, psql_);
        }
        
        requestBody["id"] = id;
        auto ws = server_ != nullptr ? server_->getWSNotification() : nullptr;
        if (ws != nullptr)
        {
            ws->sendToAll("addEtablissement", Json::FastWriter().write(requestBody));
        }

        return HttpResponse(ResponseCode::OK, "{\"id\": " + std::to_string(id) + "}");
    }
    catch (const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Erreur lors de l'exécution de la requête");
    }

    return HttpResponse(ResponseCode::OK, "Établissement ajouté avec succès");
}
}
}
