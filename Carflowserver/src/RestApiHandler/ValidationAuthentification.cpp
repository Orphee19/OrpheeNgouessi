#include <src/RestApiHandler/ValidationAuthentification.h> 
#include <Common/Network/RestApi/Server.h>
#include <Common/Utils/Token.h>

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse ValidationAuthentification::process(Common::Network::HttpRequest* req)
{
    auto psql_= getPSQL();
    auto sessionMngr = getSessionMngr();
    if (psql_== nullptr || sessionMngr == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    std::string code;
    try
    {
        const Json::Value requestBody = req->getBody();
        code = requestBody["code"].asString();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    std::string token = req->getHeader().bearer;
    
    if (code.empty() || token.empty())
    {
        return HttpResponse(ResponseErrorCode::Bad_Request);
    }

    std::string userData;
    try
    {
        auto data = sessionMngr->getCodeAuthen(token);
        std::string servCode = std::get<0>(data);
        if (code != servCode)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Code not correct");
        }

        userData = std::get<1>(data);
    }
    catch(const std::exception& e)
    {
        HttpResponse(ResponseErrorCode::Forbidden, e.what());
    }
    
    Json::Value body;
    Json::Reader reader;

    if (not reader.parse(userData, body))
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
    }

    std::string id = body["identifiant"].asString();
    auto accessToken =  sessionMngr->createAccessToken(userData);
    std::string sToken = sessionMngr->createSessionToken(id);

    return HttpResponse(ResponseCode::OK, "{\"accessToken\" : \"" + accessToken + "\", \"sessionToken\":  \"" + sToken + "\"}");
}
}
}