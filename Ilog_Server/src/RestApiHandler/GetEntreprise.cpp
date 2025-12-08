#include <RestApiHandler/GetEntreprise.h>
#include <RestApiHandler/EntrepriseFetch.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <User.h>
#include <RestApiHandler/VerifySessionToken.h>
#include <Common/Database/IPostgreSQL.h>
#include <Common/Network/Session/SessionManager.h>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Network;
using namespace Common::Database; 

HttpResponse GetEntreprise::process(HttpRequest* req)
{
    auto psql_ = getPSQL();
    auto sessionMngr = getSessionMngr();

    if (!psql_ || !sessionMngr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible. Réessayez plus tard.");
    }

    std::string username;
    try
    {
        username = getSessionUsername(sessionMngr, req->getHeader().bearer);

    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    std::string profilUser;
     
    try
    { 
       profilUser = checkUserSessionValide(psql_, username);
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
    }

        int idEntreprise = req->getBody().get("id", -1).asInt();        
        Json::Value responseJson = fetchEntreprises(psql_, idEntreprise);

        return HttpResponse(ResponseCode::OK, responseJson.toStyledString());
}
}
}
