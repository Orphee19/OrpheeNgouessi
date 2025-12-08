#pragma once 

#include <Common/Utils/Token.h>
#include <Database/IDataBase.h>
#include <Common/Database/IPostgreSQL.h>
#include <Database/IPostgreSQL.h>
#include <Common/Network/Session/ISessionManager.h>
#include <Common/Network/RestApi/Server.h>


namespace ILogServer
{
namespace RestApiHandler
{
std::string getSessionUsername(Common::Network::Session::ISessionManager* sessionMngr, const std::string& sessionToken)
{
if (sessionToken.empty())
{
    throw std::runtime_error("L'entete de la requete invalide");
}

std::string username;

auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
if (Common::Utils::Token::isTokenTimeout(decoded))
    throw std::runtime_error("Token invalid: Token expiré");

Json::Value body;
Json::Reader reader;
reader.parse(decoded.get_payload_claim("data").as_string(), body);

username = body["username"].asString();
return username;
}

std::string checkUserSessionValide(Common::Database::IPostgreSQL* psql, const std::string& username)
{
std::string profilUser;
std::string query = "SELECT profil, status FROM utilisateur WHERE username = '" + username + "';";
    auto res = psql->processQuery(query);

    if (res.empty() || (profilUser = res[0][0].c_str()).empty())
    {
        throw std::runtime_error("Utilisateur inconnu");
    }

    UserStatus userStatus = static_cast<UserStatus>(res[0][1].as<int>());
    if (userStatus != UserStatus::VALIDATE)
    {
        throw std::runtime_error("Vous n'avez pas l'autorisation");
    }
    return profilUser;
}
}
}