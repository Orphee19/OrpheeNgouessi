#include <RestApiHandler/Rh/ConfirmVacation.h>
#include <Common/Utils/Utils.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Logger/Logger.h>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse ConfirmVacation::process(HttpRequest* req)
{
    auto psql_ = getPSQL();

    if (psql_ == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible. Réessayez plus tard");
    }

    Json::Value requestBody;
    try
    {
        requestBody = req->getBody();
    }
    catch (const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    try
    {
        int idAbsence = requestBody["id"].asInt();
        std::string status = requestBody["status"].asString();

        if (idAbsence == 0 || (status != "accepted" && status != "refused"))
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, "Paramètres invalides");
        }
        int statut = (status == "accepted") ? 1 : 0;
        std::string query = "UPDATE absences SET status = " + std::to_string(statut) + " WHERE id = " + std::to_string(idAbsence) + ";";

        auto res = psql_->processQuery(query);
        if (res.empty())
        {
            return HttpResponse(ResponseErrorCode::Forbidden, "Échec de la mise à jour");
        }

        return HttpResponse(ResponseCode::OK, "Merci pour votre retour");
    }
    catch (const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Erreur lors de l'exécution de la requête");
    }
}
}
}
