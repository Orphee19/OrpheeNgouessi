#include <src/RestApiHandler/ConfirmUpdatePassword.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Utils/Token.h>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse ConfirmUpdatePassword::process(Common::Network::HttpRequest* req)
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

    std::string pwd;
    try
    {
        auto data = sessionMngr->getCodeUpdatePwd(username);
        std::string servCode = std::get<0>(data);
        if (code != servCode)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Code not correct");
        }

        pwd = std::get<1>(data);
    }
    catch(const std::exception& e)
    {
        HttpResponse(ResponseErrorCode::Forbidden, e.what());
    }

    std::string query = "SELECT * from UpdatePassword('" + username + "', '" + pwd + "')";
    auto res = psql_->processQuery(query);

    if (res.size() == 0 or not res[0][0].as<bool>())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "can't update user password user");
    }

    return HttpResponse(ResponseCode::OK,"");
}
}
}