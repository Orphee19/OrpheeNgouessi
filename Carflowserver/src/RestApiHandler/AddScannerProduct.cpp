#include <Common/Network/RestApi/Server.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>

#include <src/RestApiHandler/AddScannerProduct.h>
#include <src/SerialNumber.h>
#include <src/User.h>


namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse AddScannerProduct::process(Common::Network::HttpRequest* req)
{
    auto psql_= getPSQL();
    auto sessionMngr = getSessionMngr();
    auto serv = dynamic_cast<RestApi::Server*>(server_);

    if (psql_== nullptr || sessionMngr == nullptr || serv == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    // On récupère le token dans le header
    std::string sessionToken = req->getHeader().bearer;
    if (sessionToken.empty())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
    }

    // On récupère l'adresse mac dans le body
    std::string mcAddress;
    try
    {
        const Json::Value requestBody = req->getBody();
        mcAddress = requestBody["mac_address"].asString();

        if (not str::isValidMacAddress(mcAddress))
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    // Verification du token et recuperation de l'identifiant
    std::string identifiant;
    try
    {
        auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
        if (Token::isTokenTimeout(decoded))
             return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");
 
        identifiant = decoded.get_payload_claim("identifiant").as_string();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    // vérification du profil
    try
    {
        std::string query = "SELECT profil FROM profil where pk_utilisateur ='" + identifiant + "';";
        auto res = psql_->processQuery(query);
        std::string profilUser;

        if (res.size() == 0 or (profilUser = res[0][0].as<std::string>()).empty())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
        }

        auto userProfil = UserProfilStringifier.fromString(profilUser);
        if (userProfil  < UserProfil::AGENT)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
    }
    
    // vérification si ce scanner est déja enregistré
    std::string query = "SELECT numero_serie FROM scan_device where addresse_mac ='" + mcAddress +"';";
    auto res = psql_->processQuery(query);
    if (res.size() != 0)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Scanner existe déja avec cette addresse");
    }

    std::string numeroSerie; 
    if (not SerialNumber::createSerialNumber(mcAddress, "scanner", numeroSerie))
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Erreur de création du numero de serie");
    }

    //ajout du scanner dans la table
    query = "insert into scan_device(numero_serie, addresse_mac) values ('" + numeroSerie + "', '" + mcAddress + "');";
    res = psql_->processQuery(query);

    return HttpResponse(ResponseCode::OK,"");
}
}
}