#include <RestApiHandler/UpdatePassword.h>
#include <Common/Utils/Utils.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Database/IPostgreSQL.h>
#include <Common/Utils/Token.h>
#include <User.h>
#include <iostream>
#include <algorithm>
#include <cctype>    

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Network;
using namespace Common::Utils;

HttpResponse UpdatePassword::process(HttpRequest* req)
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

    const Json::Value requestBody = req->getBody();
    std::string oldPwd;
    std::string password;

    try
    {
        oldPwd = requestBody["old_password"].asString();
        password = requestBody["password"].asString();

        bool ret =  (not oldPwd.empty() and str::isValidPassword(oldPwd))
                and (not password.empty() and str::isValidPassword(password));

        if (not ret)
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
        }
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

    std::string email;
    try
    {
        std::string query = "SELECT * FROM GetUserByUsernamePassword('" + username + "','" + oldPwd + "');";
        auto res = psql_->processQuery(query);

        if (res.size() != 1)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Mot de passe encours invalide");
        }

        email = res[0][3].as<std::string>();
    }
    catch(const std::exception&)
    {
         return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Failed_During_Process);
    }

    auto code = Token::generateCode(6);
    auto emailMngr = getEmailMgnr();
    if (emailMngr != nullptr)
    {
        emailMngr->sendEmail(email, "Code de Confirmation : Mise à jour de Mot de passe", "code : " + code, [=, this](uint res)
        {
            if (res == 0)
            {
                auto ssMngr = getSessionMngr();
                if (ssMngr != nullptr)
                {
                    auto pwdTuple = std::make_tuple(code, password);
                    ssMngr->addCodeUpdatePwd(username, pwdTuple);
                }
            }
        });
    }

    return HttpResponse(ResponseCode::OK, "");
}
}
}
