#include <RestApiHandler/ScannerConnexion.h>
#include <Common/Network/RestApi/Server.h>
#include <Scanner.h>
#include <iostream>

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Network;
using namespace Common::Utils;
HttpResponse ScannerConnexion::process(HttpRequest* req)
{
    auto psql_= getPSQL();
    auto keyMngr = getKeyMngr();
    auto sessionMngr = getSessionMngr();

    if (psql_== nullptr || keyMngr == nullptr || sessionMngr == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }
 
    const Json::Value requestBody = req->getBody();
    std::string username;
    std::string password;

    try
    {
        username = requestBody["username"].asString();
        password = requestBody["password"].asString();

        if (username.empty() or password.empty())
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Username_or_Password_Invalid);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    std::string query = "SELECT status, nom_ligne FROM scan_device WHERE numero_serie = '" + username + "';";
    auto res = psql_->processQuery(query);
    
    if (res.size() == 0) {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Username_or_Password_Invalid);
    }

    auto status = DeviceStatusStringifier.fromString(res[0][0].as<std::string>());
    if (status != DeviceStatus::CONFIGURED)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Scanner not in configured mode");
    }

    auto sha1 = keyMngr->getShaPrivateKey(username);
    if (sha1 != password)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Username_or_Password_Invalid);
    }

    std::string ligne = res[0][1].as<std::string>();
    std::string sToken = sessionMngr->createSessionToken(username);

    return HttpResponse(ResponseCode::OK, "{\"ligne\" : \"" + ligne + "\", \"sessionToken\":  \"" + sToken + "\"}");
}
}
}
