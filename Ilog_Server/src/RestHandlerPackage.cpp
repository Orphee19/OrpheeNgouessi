#include "RestHandlerPackage.h"
#include <RestApiHandler/AddDevice.h>
#include <RestApiHandler/AddUser.h>
#include <RestApiHandler/Rh/AddSalarie.h>
#include <RestApiHandler/Stock/AddArticles.h>
#include <RestApiHandler/Rh/AddVacation.h>
#include <RestApiHandler/AddEtablissement.h>
#include <RestApiHandler/AjoutEntreprise.h>
#include <RestApiHandler/ConfirmUpdatePassword.h>
#include <RestApiHandler/Connexion.h>
#include <RestApiHandler/Rh/ConfirmVacation.h>
#include <RestApiHandler/DeleteAccount.h>
#include <RestApiHandler/DeleteUser.h>
#include <RestApiHandler/DeleteArticles.h>
#include <RestApiHandler/GetEntreprise.h>
#include <RestApiHandler/GetEtablissement.h>
#include <RestApiHandler/GetListeDevice.h>
#include <RestApiHandler/GetListeUser.h>
#include <RestApiHandler/RefreshToken.h>
#include <RestApiHandler/UpdatePassword.h>
#include <RestApiHandler/ValidationAuthentification.h>

namespace ILogServer
{
using namespace RestApiHandler;

std::map<std::string, std::shared_ptr<Common::Network::RestApi::IHandler>> RestHandlerPackage::getHandlers()
{
    std::map<std::string, std::shared_ptr<Common::Network::RestApi::IHandler>> handlers;

    handlers["/addUser"] = std::make_shared<ILogServer::RestApiHandler::AddUser>();
    handlers["/addSalarie"] = std::make_shared<ILogServer::RestApiHandler::AddSalarie>();
    handlers["/addArticles"] = std::make_shared<ILogServer::RestApiHandler::Stock::AddArticles>();
    handlers["/addEtablissement"] = std::make_shared<ILogServer::RestApiHandler::AddEtablissement>();
    handlers["/addVacation"] = std::make_shared<ILogServer::RestApiHandler::AddVacation>();
    handlers["/ajoutEntreprise"] = std::make_shared<ILogServer::RestApiHandler::AjoutEntreprise>();
    handlers["/confirmUpdatePassword"] = std::make_shared<ILogServer::RestApiHandler::ConfirmUpdatePassword>();
    handlers["/confirmVacation"] = std::make_shared<ILogServer::RestApiHandler::ConfirmVacation>();
    handlers["/connexion"] = std::make_shared<ILogServer::RestApiHandler::Connexion>();
    handlers["/deleteAccount"] = std::make_shared<ILogServer::RestApiHandler::DeleteAccount>();
    handlers["/deleteArticles"] = std::make_shared<ILogServer::RestApiHandler::DeleteArticles>();
    handlers["/getListeUser"] = std::make_shared<ILogServer::RestApiHandler::GetListeUser>();
    handlers["/getentreprise"] = std::make_shared<ILogServer::RestApiHandler::GetEntreprise>();
    handlers["/getetablissement"] = std::make_shared<ILogServer::RestApiHandler::GetEtablissement>();
    handlers["/refreshToken"] = std::make_shared<ILogServer::RestApiHandler::RefreshToken>();
    handlers["/updatePassword"] = std::make_shared<ILogServer::RestApiHandler::UpdatePassword>();
    handlers["/validationAuthentification"] = std::make_shared<ILogServer::RestApiHandler::ValidationAuthentification>();
    return handlers;
}
};