#include <Common/Network/RestApi/Server.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>

#include <src/RestApiHandler/AddDevice.h>
#include <src/SerialNumber.h>
#include <src/User.h>


namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse AddDevice::process(Common::Network::HttpRequest* req)
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
    std::string deviceType;
    std::string name;
    try
    {
        const Json::Value requestBody = req->getBody();
        mcAddress = requestBody["mac_address"].asString();
        deviceType = requestBody["device_type"].asString();
        name = requestBody["nom"].asString();

        if (not str::isValidMacAddress(mcAddress) or deviceType.empty())
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    // Verification du token et recuperation de l'identifiant
    std::string username;
    try
    {
        auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
        if (Token::isTokenTimeout(decoded))
             return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");

        Json::Value body;
        Json::Reader reader;
        reader.parse(decoded.get_payload_claim("data").as_string(), body);

        username = body["username"].asString();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    // vérification du profil
    try
    {
        std::string query = "SELECT profil FROM utilisateur where username ='" + username + "';";
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

    std::string numeroSerie; 
    if (not SerialNumber::createSerialNumber(mcAddress, deviceType, numeroSerie))
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Erreur de création du numero de serie");
    }

    try
    {
        std::string query = "insert into scan_device(numero_serie, addresse_mac, type_device, name) values ('" + numeroSerie + "', '" + mcAddress + "' + '" + deviceType + "', '" + name + "');";
        auto res = psql_->processQuery(query);
        return HttpResponse(ResponseCode::OK, "{\"serialNumber\" : \"" + numeroSerie + "\" }");
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Failed to add device");
    }
}
}
}