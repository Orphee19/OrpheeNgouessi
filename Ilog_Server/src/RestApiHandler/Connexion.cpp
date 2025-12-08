#include <RestApiHandler/Connexion.h>
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

HttpResponse Connexion::process(HttpRequest* req)
{
    auto psql_= getPSQL();
    if (psql_== nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    const Json::Value requestBody = req->getBody();
    std::string username;
    std::string psswrd;
    std::string code_etablissement;
  
    try
    {
        username = requestBody["username"].asString();
        psswrd = requestBody["password"].asString();
        code_etablissement = requestBody["codeEtablissement"].asString();  

        bool ret =  str::isValidUsername(username)
                and (not psswrd.empty() and str::isValidPassword(psswrd));
          std::cout << " dans le try avant le if not ret" << std::endl;
          std::cout << " le ret est de : " << ret << std::endl;
          std::cout << "Username reçu : [" << username << "]" << std::endl;
          std::cout << "Mot de passe reçu : [" << psswrd << "]" << std::endl;
          std::cout << "Code reçu : [" << code_etablissement << "]" << std::endl;


        if (not ret)
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }
    std::cout << " recuperation du nom et mdp" << std::endl;
    std::string query = "SELECT * FROM getuserbyusernamepassword('" + username + "','" + psswrd + "');";
    std::cout << " verification du nom et mdp dans la base " << std::endl;
    auto res = psql_->processQuery(query);
    if (res.size() != 1)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Username_or_Password_Invalid);
    }

    std::string username_ = (res[0][0].is_null()) ? "" : res[0][0].as<std::string>();
    std::string nom = (res[0][1].is_null()) ? "" : res[0][1].as<std::string>();    
    std::string prenom = (res[0][2].is_null()) ? "" : res[0][2].as<std::string>();
    std::string email = (res[0][3].is_null()) ? "" : res[0][3].as<std::string>();
    int sexe = (res[0][4].is_null()) ? 0 : res[0][4].as<int>();
    std::string profil = (res[0][5].is_null()) ? "" : res[0][5].as<std::string>();
    int status = (res[0][6].is_null()) ? 0 : res[0][6].as<int>();

    try
    {
        User user(sexe, nom, prenom, username_, email, status, profil);

        if (user.getStatus() == UserStatus::SUPPRESSED)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Account_Invalid);
        }
  std::cout << " avant generation du token" << std::endl;
        auto tok = Token::generateRandomToken(25); 
        auto code = Token::generateCode(6);
        auto emailMngr = getEmailMgnr();
        
         // insertion du code_etablissement dans la table
        if (!code_etablissement.empty())
        {
            std::string insertCode = "INSERT INTO code_etablissement (code_etablissement) VALUES ('" + code_etablissement + "');";
            try {
                psql_->processQuery(insertCode);
                std::cout << "Code etablissement inséré dans la table : " << code_etablissement << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Erreur lors de l’insertion du code_etablissement : " << e.what() << std::endl;
            }
        }

        if (emailMngr != nullptr)
        {
            emailMngr->sendEmail(user.email_, "Code Authentification", "code : " + code, [=, this](uint res)
            {
                if (res == 0)
                {
                    auto ssMngr = getSessionMngr();
                    if (ssMngr != nullptr)
                    {
                        auto userTuple = std::make_tuple(code, user.getRawJson());
                        ssMngr->addCodeAuthen(tok, userTuple);
                    }
                }
            });
        }
  std::cout << " avant envoie de le reponse et token" << std::endl;
        return HttpResponse(ResponseCode::OK, "{\"token\" : \"" + tok + "\" }");
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::User_Unknown);
    }
}
}
}