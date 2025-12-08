#include <RestApiHandler/GetEtablissement.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <User.h>

namespace ILogServer {
namespace RestApiHandler {

using namespace Common::Utils;
using namespace Common::Network;

Json::Value fetchEtablissements(Common::Database::IPostgreSQL* psql, const std::string& idEtablissement)
{
    Json::Value arr = Json::arrayValue;
    std::string query;

    if (!idEtablissement.empty()) {
        query = "SELECT * FROM etablissements WHERE id = '" + idEtablissement + "';";
    } else {
        query = "SELECT * FROM etablissements;";
    }

    auto res = psql->processQuery(query);

    for (const auto& item : res) {
        Json::Value json = Json::objectValue;
        json["id"] = item[0].is_null() ? "" : item[0].c_str();
        json["id_entreprise"] = item[1].is_null() ? "" : item[1].c_str();
        json["forme_juridique"] = item[2].is_null() ? "" : item[2].c_str();
        json["date_creation"] = item[3].is_null() ? "" : item[3].c_str();
        json["numero_anpi"] = item[4].is_null() ? "" : item[4].c_str();
        json["numero_cnamgs"] = item[5].is_null() ? "" : item[5].c_str();
        json["numero_rmcc"] = item[6].is_null() ? "" : item[6].c_str();
        json["numero_cnss"] = item[7].is_null() ? "" : item[7].c_str();
        json["numero_nif"] = item[8].is_null() ? "" : item[8].c_str();
        json["adresse"] = item[9].is_null() ? "" : item[9].c_str();
        json["ville"] = item[10].is_null() ? "" : item[10].c_str();
        json["pays"] = item[11].is_null() ? "" : item[11].c_str();
        json["telephone"] = item[12].is_null() ? "" : item[12].c_str();
        json["email"] = item[13].is_null() ? "" : item[13].c_str();
        arr.append(json);
    }

    return arr;
}

// Fonction principale pour traiter la requête
HttpResponse GetEtablissement::process(HttpRequest* req)
{
    auto psql = getPSQL();
    auto sessionMngr = getSessionMngr();

    if (psql == nullptr || sessionMngr == nullptr) {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible. Réessayez plus tard.");
    }

    std::string sessionToken = req->getHeader().bearer;
    if (sessionToken.empty()) {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
    }

    std::string username;
    try {
        auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
        if (Token::isTokenTimeout(decoded)) {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");
        }

        Json::Value tokenPayload;
        Json::Reader reader;
        if (!reader.parse(decoded.get_payload_claim("data").as_string(), tokenPayload)) {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Erreur de parsing du token");
        }

        username = tokenPayload["username"].asString();
    } catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
    }

    std::string profilUser;
    try {
        std::string query = "SELECT profil, status FROM utilisateur WHERE username ='" + username + "';";
        auto res = psql->processQuery(query);

        if (res.empty() || (profilUser = res[0][0].as<std::string>()).empty()) {
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
        }

        UserStatus userStatus = UserStatus(res[0][1].as<int>());
        if (userStatus != UserStatus::VALIDATE) {
            return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
        }
    } catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
    }

    try {
        UserProfil prof = UserProfilStringifier.fromString(profilUser);
        if (prof == UserProfil::USER) {
            return HttpResponse(ResponseErrorCode::Forbidden);
        }

        Json::Value body = req->getBody();
        std::string idEtablissement = body.get("id", "").asString();

        Json::Value responseJson = fetchEtablissements(psql, idEtablissement);
        return HttpResponse(ResponseCode::OK, responseJson.toStyledString());

    } catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Forbidden, "Échec de renvoi des établissements");
    }
}

}
} 
