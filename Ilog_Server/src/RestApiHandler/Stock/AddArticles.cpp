#include <RestApiHandler/Stock/AddArticles.h>
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

HttpResponse AddArticles::process(HttpRequest* req) {
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
        // Extraction des champs
        std::string nom = body["nom"].asString();
        std::string description = body["description"].asString();
        std::string codeBarre = body["code_barre"].asString();
        std::string unite = body["unite"].asString();
        double prixHt = body["prixHt"].asDouble();
        double tva = body["tva"].asDouble();
        int quantite = body["quantite"].asInt();
        int hasAlert = body.get("hasAlert", false).asBool();
        int seuilAlert = body.get("seuilAlert", 0).asInt();

        std::vector<ArticleCategorie> categories;
        if (body.isMember("categories"))
        {
            const auto & categoriesJson = body["categories"];
            for (const auto & categorie : categoriesJson)
            {
                categories.push_back(ArticleCategorie::fromJson(categorie));
            }
        }

        for (auto & categorie : categories)
        {
            ArticleCategorie::addArticleCategories(categorie, psql_);
        }

        // Préparer la requête SQL
        Query query("INSERT INTO articles(nom, description, code, quantite, unite, prix, tva, has_alert, seuil_alert)"
                  " VALUES (:nom, :description, :code, :quantite, :unite, :prix, :tva, :has_alert, :seuil_alert) RETURNING id;");
        
        query.bindValue(":nom", nom);
        query.bindValue(":description", description);
        query.bindValue(":code", codeBarre);
        query.bindValueInt(":quantite", quantite);
        query.bindValue(":unite", unite);
        query.bindValueDouble(":prix", prixHt);
        query.bindValueDouble(":tva", tva);
        query.bindValueBool(":has_alert", hasAlert);
        query.bindValueInt(":seuil_alert", seuilAlert);
        auto res = psql_->processQuery(query);

        // récupérer l'id
        int idArticle = res[0][0].as<int>();

        ArticleCategorie::linkArticleToCategories(idArticle, categories, psql_);
        
        return HttpResponse(ResponseCode::OK, "{\"id\":  " + std::to_string(idArticle) + "}");

    }
    catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Bad_Request, e.what());
    }
}
}
}
}
