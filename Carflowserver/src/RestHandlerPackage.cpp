#include "RestHandlerPackage.h"

#include <RestApiHandler/Connexion.h>
#include <RestApiHandler/Inscription.h>
#include <RestApiHandler/VerificationCompte.h>
#include <RestApiHandler/RenvoiCodeVerificationCompte.h>
#include <RestApiHandler/ValidationAuthentification.h>
#include <RestApiHandler/AddScannerProduct.h>
#include <RestApiHandler/ConfigurationScanner.h>
#include <RestApiHandler/ScannerConnexion.h>


namespace CarflowServer
{
using namespace RestApiHandler;

std::map<std::string, std::shared_ptr<Common::Network::RestApi::IHandler>> RestHandlerPackage::getHandlers()
{
    std::map<std::string, std::shared_ptr<Common::Network::RestApi::IHandler>> handlers;

    handlers["/connexion"] = std::make_shared<Connexion>();
    handlers["/inscription"] = std::make_shared<Inscription>();
    handlers["/verifyCompte"] = std::make_shared<VerificationCompte>();
    handlers["/renvoiCodeVerificationCompte"] = std::make_shared<RenvoiCodeVerificationCompte>();
    handlers["/validationAuthentification"] = std::make_shared<ValidationAuthentification>();
    handlers["/addScannerProduct"] = std::make_shared<AddScannerProduct>();
    handlers["/configurationScanner"] = std::make_shared<ConfigurationScanner>();
    handlers["/scannerConnexion"] = std::make_shared<ScannerConnexion>();
  
    return handlers;
}
};