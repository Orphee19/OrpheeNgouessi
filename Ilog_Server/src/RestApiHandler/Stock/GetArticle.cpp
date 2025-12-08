#include <RestApiHandler/Stock/GetArticle.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <User.h>
#include <json/json.h>
#include <RestApiHandler/Stock/ArticleCategorie.h>
#include <Common/Utils/Query.h>

namespace ILogServer {
namespace RestApiHandler {
namespace Stock {

using namespace Common::Utils;
using namespace Common::Network;

HttpResponse GetArticle::process(HttpRequest* req) {
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
        int id = body.get("id", -1).asInt();
        
        Query query("SELECT * FROM articles " + (id != 0 ? "WHERE id = " + std::to_string(id) + "" : "") + ";" );

        auto res = psql_->processQuery(query);

        Json::Value arr = Json::arrayValue;
        for (const auto & result : res)
        {
            Json::Value obj = Json::objectValue;
            int id = result[0].as<int>();
            obj["id"] = id;
            obj["nom"] = result[1].as<std::string>();
            obj["description"] = result[1].as<std::string>();
            obj["code"] = result[1].as<std::string>();
            obj["quantite"] = result[1].as<int>();
            obj["unite"] = result[1].as<std::string>();
            obj["hasAlert"] = result[1].as<bool>();
            obj["seuilAlert"] = result[1].as<int>();
            obj["prix"] = result[1].as<double>();
            obj["tva"] = result[1].as<double>();
            
            const auto & categories = ArticleCategorie::getArticleCategories(id, psql_);
            Json::Value catArr = Json::arrayValue;
            for(const auto & categorie : categories)
            {
                catArr.append(categorie.toJson());
            }

            obj["categories"] = catArr;

            arr.append(obj);
        }
        
        return HttpResponse(ResponseCode::OK, arr.toStyledString());
    }
    catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Bad_Request, e.what());
    }
}
}
}
}
