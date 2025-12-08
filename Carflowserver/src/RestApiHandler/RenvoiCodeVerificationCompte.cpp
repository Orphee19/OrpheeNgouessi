#include <RestApiHandler/RenvoiCodeVerificationCompte.h>
#include<Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <User.h>

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse RenvoiCodeVerificationCompte::process(HttpRequest* req)
{
    auto psql_= getPSQL();
    if (psql_== nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    std::string email;

    try
    {
        const Json::Value requestBody = req->getBody();
        email = requestBody["email"].asString();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }
    
    bool ret =  (not email.empty() or str::isValidEmail(email));

    if (not ret)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    std::string query = "SELECT * FROM utilisateur WHERE email = '" +  email + "';";
    auto res = psql_->processQuery(query);

    if (res.size() != 1)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown );
    }

    int status = res[0][9].as<int>();
    if (status != UserStatus::CREATED)
    {
        return HttpResponse(ResponseErrorCode::Forbidden);
    }

    std::string id = res[0][0].as<std::string>();
    auto tok = Token::generateRandomToken(25); 
    auto code = Token::generateCode(6);

    auto emailMngr = getEmailMgnr();
    if (emailMngr != nullptr)
    {
        emailMngr->sendEmail(email, "Confirmation de compte", "code : " + code, [=, this](uint res){
            if (res == 0)
            {
                auto ssMngr = getSessionMngr();
                if (ssMngr != nullptr)
                    ssMngr->addCodeValidate(tok, {code, id});
            }
        });
    }

    return HttpResponse(ResponseCode::OK, "{\"token\" : \"" + tok + "\" }");
}
}


}
