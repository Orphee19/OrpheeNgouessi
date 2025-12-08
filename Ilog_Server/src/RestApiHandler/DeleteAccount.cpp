#include <RestApiHandler/DeleteAccount.h>
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

HttpResponse DeleteAccount::process(HttpRequest* req)
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

    // Verification du token et recuperation du username
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

    try
    {
        std::string query = "UPDATE utilisateur SET status = 3 WHERE username = '" + username + "';";
        auto res = psql_->processQuery(query);
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Supression échouée");
    }
    
    return HttpResponse(ResponseCode::OK, "");
}
}

}
