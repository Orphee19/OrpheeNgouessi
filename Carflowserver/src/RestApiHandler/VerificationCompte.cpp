#include <src/RestApiHandler/VerificationCompte.h> 
#include <Common/Network/RestApi/Server.h>

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse VerificationCompte::process(Common::Network::HttpRequest* req)
{
    auto psql_= getPSQL();
    auto ssMngr = getSessionMngr();
    if (psql_== nullptr || ssMngr == nullptr)
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

    std::string identifiant;
    try
    {
        auto data = ssMngr->getCodeValidate(token);
        std::string servCode = std::get<0>(data);
        if (code != servCode)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Code not correct");
        }

        identifiant = std::get<1>(data);
    }
    catch(const std::exception& e)
    {
        HttpResponse(ResponseErrorCode::Forbidden, std::string(e.what()));
    }

    std::string query = "SELECT * FROM validUser('" + identifiant + "');";
    auto res = psql_->processQuery(query);

    if (res.size() == 0 or not res[0][0].as<bool>())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "can't validate user");
    }

    return HttpResponse(ResponseCode::OK,"");
}
}
}