#include <RestApiHandler/Stock/ArticleCategorie.h>
#include <Common/Utils/Query.h>

using namespace Common::Utils;
using namespace ILogServer::RestApiHandler::Stock;

void ArticleCategorie::addArticleCategories(ArticleCategorie& categorie, Common::Database::IPostgreSQL* psql)
{
    const auto insertCategorie = [&psql] (const ArticleCategorie& categorie, int id_parent = -1) -> int {
        Query query("INSERT INTO artCategories(nom, categorie_parent_id) VALUES (:categorie, :idParent) RETURNING id;");
        query.bindValue(":categorie", categorie.nom);
        if (id_parent != -1)
        {
            query.bindValueInt(":idParent", id_parent);
        }
        else
        {
            query.bindNullValue(":idParent");
        }

        auto res = psql->processQuery(query);

        return res[0][0].is_null() ? -1 : res[0][0].as<int>();
    };

    int idParent = categorie.id;

    if (idParent == -1)
    {
        idParent = insertCategorie(categorie);
        categorie.id = idParent;
    }

    for (ArticleCategorie& subcategorie : categorie.subcategories)
    {
        if (subcategorie.id == -1)
        {
            subcategorie.id = insertCategorie(subcategorie, idParent);
        }
    }
}

void ArticleCategorie::linkArticleToCategories(const int idArticle, const std::vector<ArticleCategorie>& categories, Common::Database::IPostgreSQL* psql)
{
    std::function<void(const int, const ArticleCategorie&)> link;

    link = [&link, &psql] (const int idArt, const ArticleCategorie& categorie) -> void {
        Query query("INSERT INTO articles_categories(id_article, id_categorie)"
                      " VALUES (:id_article, :id_categorie)");

        query.bindValueInt(":id_article", idArt);
        query.bindValueInt(":id_categorie", categorie.id);

        auto res = psql->processQuery(query);

        for (const auto& subcat : categorie.subcategories)
        {
            link(idArt, subcat);
        }
    };

    Query query("delete from articles_categories where id_article = :id ");
    query.bindValueInt(":id", idArticle);
    psql->processQuery(query);

    for (const ArticleCategorie& categorie : categories)
    {
        if (categorie.id == -1)
        {
            continue;
        }

        link(idArticle, categorie);
    }
}

std::vector<ArticleCategorie> ArticleCategorie::getArticleCategories(const int idArticle, Common::Database::IPostgreSQL* psql)
{
    std::vector<ArticleCategorie> categories;

    Query query("SELECT * FROM articles_categories FULL JOIN artCategories "
                  "ON articles_categories.id_categorie = artCategories.id "
                  "WHERE articles_categories.id_article = (:idArticle)");
    query.bindValueInt(":idArticle", idArticle);

    auto res = psql->processQuery(query);
    for (const auto & result : res)
    {
        ArticleCategorie categorie;
        categorie.id = result[0].as<int>();
        categorie.nom = result[1].as<std::string>();
        int idParentCat = result[2].is_null()? 0 : result[2].as<int>();
        if (idParentCat == 0)
        {
            categories.push_back(categorie);
            continue;
        }

        for(auto& cat : categories)
        {
            if (cat.id == idParentCat)
            {
                cat.subcategories.push_back(categorie);
            }
        }
    }

    return categories;
}