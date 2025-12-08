#include <src/RestApiHandler/ConfigurationScanner.h>
#include <src/Scanner.h>
#include <Common/Network/Key/KeyManager.h>

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse ConfigurationScanner::process(Common::Network::HttpRequest* req)
{
    auto psql_= getPSQL();
    auto keyMngr = getKeyMngr();
    if (psql_== nullptr || keyMngr == nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    // On récupère l'adresse mac dans le body
    std::string mcAddress;
    try
    {
        const Json::Value requestBody = req->getBody();
        mcAddress = requestBody["mac_address"].asString();

        if (not str::isValidMacAddress(mcAddress))
        {
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
        }
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }

    // vérification si ce scanner est déja enregistré et recuperation des infos 
    std::string query = "SELECT status, numero_serie FROM scan_device WHERE addresse_mac = '" + mcAddress + "';";
    auto res = psql_->processQuery(query);

    if (res.size() == 0) {
        return HttpResponse(ResponseErrorCode::Forbidden, "Scanner inconnu");
    }
 
    auto status = DeviceStatusStringifier.fromString(res[0][0].as<std::string>());
    if (status != DeviceStatus::CREATED)
    {
        return HttpResponse(ResponseErrorCode::Forbidden, "Scanner already configured");
    }

    auto numeroSerie = res[0][1].as<std::string>();
    
    if (not keyMngr->createAndSignKeyPair(1024, 365, numeroSerie))
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, "Failed to generate keys");
    }

    std::string privatekey;
    std::string publicKey;
    keyMngr->getKeys(privatekey, publicKey, numeroSerie);

    // Mise à jour du statut à "configuré" dans la base de données
    query = "UPDATE scan_device SET status = 'configured' WHERE addresse_mac = '" + mcAddress + "';";
    psql_->processQuery(query);

    return HttpResponse(ResponseCode::OK,
        "{\"serialNumber\" : \"" + numeroSerie + "\", \"certificate\":  \"" +  publicKey + "\", \"key\":  \"" +  privatekey + "\" }");
}
}
}