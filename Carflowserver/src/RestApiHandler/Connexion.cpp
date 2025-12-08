#include <RestApiHandler/Connexion.h>
#include <Common/Utils/Utils.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Database/IPostgreSQL.h>
#include <Common/Utils/Token.h>
#include <User.h>
#include <iostream>
#include <algorithm>
#include <cctype>    

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Network;
using namespace Common::Utils;
HttpResponse Connexion::process(HttpRequest* req)
{
    auto psql_= getPSQL();
    if (psql_== nullptr)
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

        bool ret =  str::isValidUsername(username)
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

    std::string query = "SELECT * FROM Connexion('" + username + "','" + password + "');";   
    
    auto res = psql_->processQuery(query);
    if (res.size() != 1)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Username_or_Password_Invalid);
    }

    std::string identifiant = (res[0][0].is_null()) ? "" : res[0][0].as<std::string>();
    int sexe = (res[0][1].is_null()) ? 0 : res[0][1].as<int>();
    std::string nom = (res[0][2].is_null()) ? "" : res[0][2].as<std::string>();
    std::string prenom = (res[0][3].is_null()) ? "" : res[0][3].as<std::string>();
    std::string Username = (res[0][4].is_null()) ? "" : res[0][4].as<std::string>();
    std::string naissance = (res[0][5].is_null()) ? "" : res[0][5].as<std::string>();
    std::string email = (res[0][6].is_null()) ? "" : res[0][6].as<std::string>();
    std::string phone = (res[0][7].is_null()) ? "" : res[0][7].as<std::string>();
    std::string contact = (res[0][8].is_null()) ? "" : res[0][8].as<std::string>();
    int status = (res[0][9].is_null()) ? 0 : res[0][9].as<int>();
    std::string profil = (res[0][10].is_null()) ? "" : res[0][10].as<std::string>();
    std::string carte = (res[0][11].is_null()) ? "" : res[0][11].as<std::string>();
    bool hasPhoto = (res[0][12].is_null()) ? false : res[0][12].as<bool>();
    bool hasEntite = (res[0][13].is_null()) ? false : res[0][13].as<bool>();

    User user(identifiant,sexe,nom,prenom,username,naissance,email,phone,contact,status,profil,carte,hasPhoto,hasEntite);

    if (not user.isValidAccount())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Account_Invalid);
    }

    auto tok = Token::generateRandomToken(25); 
    auto code = Token::generateCode(6);
    
    if (user.contact_ == "email")
    {
        auto emailMngr = getEmailMgnr();
        if (emailMngr != nullptr)
        {
            emailMngr->sendEmail(user.email_, "Code Authentification", "code : " + code, [=, this](uint res)
            {
                if (res == 0)
                {
                    auto ssMngr = getSessionMngr();
                    if (ssMngr != nullptr)
                        ssMngr->addCodeAuthen(tok, {code, user.getRawJson()});
                }
            });
        }
    }

    return HttpResponse(ResponseCode::OK, "{\"token\" : \"" + tok + "\" }");
}
}
}
