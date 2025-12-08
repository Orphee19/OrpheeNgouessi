#include <Common/Network/RestApi/Server.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>

#include <src/RestApiHandler/RefreshToken.h>
#include <src/User.h>


namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse RefreshToken::process(Common::Network::HttpRequest* req)
{
    auto psql_= getPSQL();
    auto sessionMngr = getSessionMngr();
    auto serv = dynamic_cast<RestApi::Server*>(server_);

    if (psql_== nullptr || sessionMngr == nullptr || serv == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    // On récupère le token dans le header
    std::string accessToken = req->getHeader().bearer;
    if (accessToken.empty())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
    }

    // Verification du token et recuperation de l'identifiant
    std::string username;
    try
    {
        auto decoded = sessionMngr->decodeAndVerifyToken(accessToken);
        if (Token::isTokenTimeout(decoded))
             return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");
 
        User user(decoded.get_payload_claim("data").as_string());
        username = user.username_;
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    // vérification du status
    int status;
    int profil;
    try
    {
        std::string query = "SELECT status, profil FROM utilisateur where username ='" + username + "';";
        auto res = psql_->processQuery(query);

        if (res.size() == 0)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
        }

        status = res[0][0].as<int>();
        profil = (int)UserProfilStringifier.fromString(res[0][1].as<std::string>());

        if (status == (int)UserStatus::SUPPRESSED)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Account_Invalid);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
    }

    Json::Value json = Json::objectValue;
    json["username"] = username;
    json["profile"] = profil;
    json["status"] = status;

    std::string sToken = sessionMngr->createSessionToken(json.toStyledString());

    return HttpResponse(ResponseCode::OK, "{\"sessionToken\":  \"" + sToken + "\"}");
}
}
}