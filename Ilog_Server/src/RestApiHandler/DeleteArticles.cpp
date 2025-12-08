#include <RestApiHandler/DeleteArticles.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <User.h>
#include <json/json.h>

namespace ILogServer {
namespace RestApiHandler {

using namespace Common::Utils;
using namespace Common::Network;

HttpResponse DeleteArticles::process(HttpRequest* req) {
    auto psql_ = getPSQL();
    auto sessionMngr = getSessionMngr();

    if (psql_ == nullptr || sessionMngr == nullptr) {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible.");
    }

    std::string sessionToken = req->getHeader().bearer;
    if (sessionToken.empty()) {
        return HttpResponse(ResponseErrorCode::Forbidden, "Header Authorization manquant.");
    }

    try {
        const Json::Value body = req->getBody();
        std::cout << "JSON reçu :\n" << body.toStyledString() << std::endl;

        int article_id = body["id_article"].asInt();

        std::string query =
            "SELECT * FROM DeleteArticles(" + std::to_string(article_id) + ");";

        std::cout << "Requête SQL : [" << query << "]" << std::endl;

        auto res = psql_->processQuery(query);

        // Affichage du résultat
        std::cout << "Résultat de la requête PostgreSQL :" << std::endl;
        for (int i = 0; i < res.size(); ++i) {
            std::cout << "Ligne " << i << ": ";
            for (int j = 0; j < res[i].size(); ++j) {
                if (res[i][j].is_null()) {
                    std::cout << "NULL";
                } else {
                    std::cout << res[i][j].c_str();
                }
                if (j != res[i].size() - 1) std::cout << " | ";
            }
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Champ manquant ou invalide");
    }

    return HttpResponse(ResponseCode::OK, "Article supprimé avec succès");
}

} 
}
