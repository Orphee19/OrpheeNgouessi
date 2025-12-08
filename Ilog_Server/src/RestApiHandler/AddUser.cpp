#include <RestApiHandler/AddUser.h>
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

HttpResponse AddUser::process(HttpRequest* req)
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

    // On récupère les datas du body

    std::string username, nom, prenom, email, password;
    UserProfil profil;
    int sexe;
    try
    {
        const Json::Value requestBody = req->getBody();
        username = requestBody["username"].asString();
        nom = requestBody["nom"].asString();
        prenom = requestBody["prenom"].asString();
        email = requestBody["email"].asString();
        password = requestBody["password"].asString();
        profil = UserProfilStringifier.fromString(requestBody["profil"].asString());
        sexe = requestBody["sexe"].asUInt();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    // Verification du token et recuperation du username
    std::string username_;
    try
    {
        auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
        if (Token::isTokenTimeout(decoded))
             return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");
 
        Json::Value body;
        Json::Reader reader;
        reader.parse(decoded.get_payload_claim("data").as_string(), body);

        username_ = body["username"].asString();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    try
    {
        std::string query = "SELECT profil, status FROM utilisateur where username ='" + username_ + "';";
        auto res = psql_->processQuery(query);
        std::string profilUser;
        UserStatus userStatus;

        if (res.size() == 0 or (profilUser = res[0][0].as<std::string>()).empty())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
        }

        auto userProfil = UserProfilStringifier.fromString(profilUser);
        userStatus = UserStatus(res[0][1].as<int>());
        if (userProfil  < UserProfil::AGENT or profil >= userProfil or userStatus != UserStatus::VALIDATE)
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
    }

    bool ret =  (not nom.empty() and nom.size() <= 20)
            and str::isValidUsername(username)
            and (not prenom.empty() and prenom.size() <= 20)
            and (email.empty() or str::isValidEmail(email))
            and (not password.empty() and str::isValidPassword(password))
            and (sexe == 1 or sexe == 2);

    if (not ret)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    try
    {
        std::string query = "SELECT * FROM Adduser('" + username + "'," + std::to_string(sexe) + ", \
                            '" + nom + "','" + prenom + "','" + email + "',\
                            '" + password + "', '" + UserProfilStringifier.toString(profil) + "');";
        auto res = psql_->processQuery(query);

        if (res.size() == 0 or not res[0][0].as<bool>())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Already_Exist);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Echec d'ajout de l'utilisateur");
    }
    
    return HttpResponse(ResponseCode::OK, "");
}
}

}
