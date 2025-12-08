#include <RestApiHandler/Rh/AddVacation.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <Common/Logger/Logger.h>
#include <RestApiHandler/TemplateEmail.h>
#include <User.h>

namespace ILogServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

 
 HttpResponse AddVacation::process(HttpRequest* req)
{
    try {
        LOG_INFO("Traitement de AddVacation::process lancé");

        auto psql_ = getPSQL();
        auto sessionMngr = getSessionMngr();

        if (psql_ == nullptr || sessionMngr == nullptr)
        {
            LOG_ERR("Service indisponible : PSQL ou SessionMngr est nul");
            return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible. Réessayez plus tard");
        }

        std::string sessionToken = req->getHeader().bearer;
        if (sessionToken.empty())
        {
            LOG_ERR("Token absent dans l'en-tête HTTP");
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
        }

         std::cout << " avant envoie de le resquestBody " << std::endl;
         std::cout << "le token"<<sessionToken<< std::endl;

        Json::Value requestBody;
        try
        {
            requestBody = req->getBody();
            LOG_INFO("Corps de la requête analysé avec succès");
        }
        catch (const std::exception& e)
        {
            LOG_ERR("Erreur lors de la lecture du corps de la requête : %s", e.what());
            return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
        }

        std::string username_;
        try
        {
            auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
            if (Token::isTokenTimeout(decoded))
            {
                LOG_ERR("Token expiré");
                return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, "Token expiré");
            }

            Json::Value body;
            Json::Reader reader;
            reader.parse(decoded.get_payload_claim("data").as_string(), body);
            username_ = body["username"].asString();

            LOG_INFO("Utilisateur identifié via token : %s", username_.c_str());
        }
        catch (const std::exception& e)
        {
            LOG_ERR("Erreur de décodage du token : %s", e.what());
            return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Token, e.what());
        }

        try
        {
            std::string query = "SELECT profil, status FROM utilisateur WHERE username = '" + username_ + "';";
            auto res = psql_->processQuery(query);

            if (res.empty() || res[0][0].as<std::string>().empty())
            {
                LOG_ERR("Utilisateur inconnu dans la base : %s", username_.c_str());
                return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Unknown);
            }

            auto userProfil = UserProfilStringifier.fromString(res[0][0].as<std::string>());
            UserStatus userStatus = UserStatus(res[0][1].as<int>());

            if (userProfil < UserProfil::AGENT || userStatus != UserStatus::VALIDATE)
            {
                LOG_ERR("Droits insuffisants pour l'utilisateur : %s", username_.c_str());
                return HttpResponse(ResponseErrorCode::Forbidden, "Vous n'avez pas l'autorisation");
            }

            LOG_INFO("Utilisateur %s autorisé à ajouter un congé", username_.c_str());
        }
        catch (const std::exception& e)
        {
            LOG_ERR("Erreur lors de la vérification des droits : %s", e.what());
            return HttpResponse(ResponseErrorCode::Forbidden, "Erreur lors de la vérification des autorisations");
        }

        try
        {
            for (auto& conge : requestBody["conges"])
            {
                int idSalarie = requestBody["idSalarie"].asInt();
                int idValidateur = requestBody["idValidateur"].asInt();
                std::string typeConge = conge["type"].asString();
                std::string dateDebut = conge["dateDebut"].asString();
                std::string finTheo = conge["dateRetourTheorique"].asString();
                int nbrJour = conge["nbrJour"].asInt();

                LOG_INFO("Ajout du congé : %s, %s, %d jours", typeConge.c_str(), dateDebut.c_str(), nbrJour);

                std::string query = "INSERT INTO absences(id_salarie, id_validateur, type, date_debut, nbre_jours, retour_theorique, date_retour, commentaire, status) VALUES ('" +
                    std::to_string(idSalarie) + "', '" +
                    std::to_string(idValidateur) + "', '" +
                    typeConge + "', '" +
                    dateDebut + "', " +
                    std::to_string(nbrJour) + ", '" +
                    finTheo + "', NULL, '" +
                    conge["commentaire"].asString() + "', 0) RETURNING id;";

                auto res = psql_->processQuery(query);
                if (res.empty())
                {
                    LOG_ERR("Échec de l'insertion du congé");
                    return HttpResponse(ResponseErrorCode::Forbidden, "Échec de l'ajout du congé");
                }

                conge["id"] = res[0][0].as<int>();
                LOG_INFO("Congé inséré avec succès, id : %d", conge["id"].asInt());

                if (idValidateur > 0)
                {
                    std::string queryValidateur = "SELECT id, nom, prenom, email FROM salarie WHERE id = " + std::to_string(idValidateur);
                    auto resValidateur = psql_->processQuery(queryValidateur);

                    if (resValidateur.empty())
                    {
                        LOG_ERR("Validateur non trouvé id : %d", idValidateur);
                        return HttpResponse(ResponseErrorCode::Forbidden, "Validateur non trouvé");
                    }

                    Salarie responsable;
                    responsable._id = resValidateur[0][0].as<int>();
                    responsable._nom = resValidateur[0][1].as<std::string>();
                    responsable._prenom = resValidateur[0][2].as<std::string>();
                    responsable._email = resValidateur[0][3].as<std::string>();

                    std::string queryDemandeur = "SELECT id, nom, prenom, email FROM salarie WHERE id = " + std::to_string(idSalarie);
                    auto resDemandeur = psql_->processQuery(queryDemandeur);

                    if (resDemandeur.empty())
                    {
                        LOG_ERR("Demandeur non trouvé id : %d", idSalarie);
                        return HttpResponse(ResponseErrorCode::Forbidden, "Demandeur non trouvé");
                    }

                    Salarie demandeur;
                    demandeur._id = resDemandeur[0][0].as<int>();
                    demandeur._nom = resDemandeur[0][1].as<std::string>();
                    demandeur._prenom = resDemandeur[0][2].as<std::string>();
                    demandeur._email = resDemandeur[0][3].as<std::string>();

                    Conge absence;
                    absence._id = res[0][0].as<int>();
                    absence._type = typeConge;
                    absence._debut = dateDebut;
                    absence._nbrJour = nbrJour;
                    absence._finTheorique = finTheo;

                    Json::Value sessionJson = Json::objectValue;
                    sessionJson["id"] = responsable._id;
                    std::string sToken = sessionMngr->createSessionToken(sessionJson.toStyledString());

                    auto emailMngr = getEmailMgnr();
                    if (emailMngr != nullptr)
                    {
                        std::string email = TemplateEmail::demandeConge(demandeur, responsable, absence, sToken);
                        emailMngr->sendEmail(responsable._emailPro.empty() ? responsable._email :
                            responsable._emailPro, "Demande de congés", email, [=, this](uint res)
                        {
                            if (res != 0)
                            {
                                LOG_ERR("Échec d'envoi de l'e-mail de congé %d à %s %s", absence._id, responsable._nom, responsable._prenom);
                            }
                            else
                            {
                                LOG_INFO("E-mail envoyé avec succès à %s %s pour congé %d", responsable._nom, responsable._prenom, absence._id);
                            }
                        });
                    }
                }
            }

            auto ws = server_ != nullptr ? server_->getWSNotification() : nullptr;
            if (ws != nullptr)
            {
                LOG_INFO("Envoi de la notification WebSocket 'addVacation'");
                ws->sendToAll("addVacation", Json::FastWriter().write(requestBody));
            }

            LOG_INFO("Ajout de congé terminé avec succès");
            return HttpResponse(ResponseCode::OK, "Demande ajoutée avec succès");
        }
        catch (const std::exception& e)
        {
            LOG_ERR("Erreur lors du traitement des congés : %s", e.what());
            return HttpResponse(ResponseErrorCode::internal_server_error, "Erreur inattendue: " + std::string(e.what()));
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERR("Erreur inattendue globale : %s", e.what());
        return HttpResponse(ResponseErrorCode::internal_server_error, "Erreur inattendue: " + std::string(e.what()));
    }
}
}
}
