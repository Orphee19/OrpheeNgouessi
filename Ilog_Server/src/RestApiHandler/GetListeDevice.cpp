#include <RestApiHandler/GetListeDevice.h>
#include<Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>

#include <User.h>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse GetListeDevice::process(HttpRequest* req)
{
    auto psql_= getPSQL();
    auto sessionMngr = getSessionMngr();

    if (psql_== nullptr || sessionMngr == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    std::string sessionToken = req->getHeader().bearer;
    if (sessionToken.empty())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
    }

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

    std::string profilUser;
    try
    {
        std::string query = "SELECT profil, status FROM utilisateur where username ='" + username + "';";
        auto res = psql_->processQuery(query);

        if (res.size() == 0 or (profilUser = res[0][0].as<std::string>()).empty())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
        }

        UserStatus userStatus = UserStatus(res[0][1].as<int>());
        if (userStatus != UserStatus::VALIDATE)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
    }

    try
    {
        UserProfil prof = UserProfilStringifier.fromString(profilUser);
        if (prof == UserProfil::USER)
        {
            return  HttpResponse(ResponseErrorCode::Forbidden);
        }

        std::string query = "SELECT * FROM device;";
        auto res = psql_->processQuery(query);

        Json::Value arr = Json::arrayValue;
        for (const auto item : res)
        {
            Json::Value json = Json::objectValue;
            json["numero_serie"] = (res[0][0].is_null()) ? "" : res[0][0].as<std::string>();
            json["address_mac"] = (res[0][1].is_null()) ? 0 : res[0][1].as<std::string>();
            json["type_device"] = (res[0][2].is_null()) ? "" : res[0][2].as<std::string>();
            json["nom"] = (res[0][3].is_null()) ? "" : res[0][3].as<std::string>();
            json["status"] = (res[0][4].is_null()) ? "" : res[0][4].as<std::string>();
            arr.append(json);
        }

        return HttpResponse(ResponseCode::OK, arr.toStyledString());
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Echec de renvoi de la liste des appareils");
    }
}
}
}
