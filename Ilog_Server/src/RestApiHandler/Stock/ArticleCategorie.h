#pragma once

#include <Common/Utils/EnumStringifier.h>
#include <json/json.h>
#include <Common/Database/IPostgreSQL.h>

namespace ILogServer
{
namespace RestApiHandler
{
namespace Stock
{
struct ArticleCategorie
{
    int id;
    std::string nom;
    std::vector<ArticleCategorie> subcategories;

    Json::Value toJson() const
    {
        Json::Value obj = Json::objectValue;
        obj["id"] = id;
        obj["nom"] = nom.c_str();

        Json::Value arr = Json::arrayValue;
        for (auto& subcategorie : subcategories)
        {
            arr.append(subcategorie.toJson());
        }

        obj["subcategories"] = arr;
        return obj;
    }

    static ArticleCategorie fromJson(const Json::Value& obj)
    {
        ArticleCategorie categorie;
        categorie.id = obj.get("id", -1).asInt();
        categorie.nom = obj["nom"].asString();

        auto arr = obj["subcategories"];
        for (const auto& subcategorie : arr)
        {
            auto subcat = ArticleCategorie::fromJson(subcategorie);
            categorie.subcategories.push_back(subcat);
        }

        return categorie;
    }

    static void addArticleCategories(ArticleCategorie& categorie, Common::Database::IPostgreSQL*);
    static void linkArticleToCategories(const int idArticle, const std::vector<ArticleCategorie>& categories, Common::Database::IPostgreSQL*);
    static std::vector<ArticleCategorie> getArticleCategories(const int idArticle, Common::Database::IPostgreSQL*);
};   
}
}
}
