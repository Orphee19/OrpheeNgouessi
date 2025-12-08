#include <RestApiHandler/Rh/AddSalarie.h>
#include <Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>
#include <User.h>

namespace ILogServer {
namespace RestApiHandler {

using namespace Common::Utils;
using namespace Common::Network;

HttpResponse AddSalarie::process(HttpRequest* req) {
    auto psql_ = getPSQL();
    auto sessionMngr = getSessionMngr();

    if (psql_ == nullptr || sessionMngr == nullptr) {
        return HttpResponse(ResponseErrorCode::service_unavailable, "Service non disponible.");
    }

    std::string sessionToken = req->getHeader().bearer;
    if (sessionToken.empty()) {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::Invalid_Header);
    }

    try {
        const Json::Value body = req->getBody();
        std::cout << "le JSON reçu :\n" << body.toStyledString() << std::endl;
        std::string matricule = body["matricule"].asString();
        int sexe = body["sexe"].asInt();
        std::string nom = body["nom"].asString();
        std::string nomMarital = body["nomMarital"].asString();
        std::cout << " nomMarital  : [" << nomMarital << "]" << std::endl;
        std::string nomUsuel = body["nomUsuel"].asString();
        std::cout << "nomUsuel  : [" << nomUsuel << "]" << std::endl;
        std::string situationFamiliale = body["situationFamiliale"].asString();
        std::string regimeMatrimonial = body["regimeMatrimonial"].asString();
        std::string dateNaissance = body["dateNaissance"].asString();
        std::string villeNaissance = body["villeNaissance"].asString();
        std::string paysNaissance = body["paysNaissance"].asString();
        std::string paysNationalite = body["paysNationalite"].asString();
        // Couverture santé
        const auto& couvertureSante = body["couvertureSante"];
        std::string cnamgs = couvertureSante["cnamgs"].asString();
        std::string cnss = couvertureSante["cnss"].asString();
        std::string ascoma = couvertureSante["ascoma"].asString();
        std::string dateVmEmbauche = couvertureSante["dateVmEmbauche"].asString();
        // Tenue
        const auto& tenue = body["tenue"];
        std::string tailleBlouse = tenue["tailleBlouse"].asString();
        std::string taillePantalon = tenue["taillePantalon"].asString();
        std::string tailleChemise = tenue["tailleChemise"].asString();
        std::string pointure = tenue["pointure"].asString();
        //etude
        const auto& etude = body["etude"];
        std::string niveauEtude = etude["niveauEtude"].asString();
        std::string diplome = etude["diplome"].asString();
        std::string specialisation = etude["specialisation"].asString();
        std::string diplomeEtablissement = etude["diplomeEtablissement"].asString();
        std::string diplomeDateObtention = etude["diplomeDateObtention"].asString();

        // Coordonnées
        const Json::Value coord = body["coordonnees"];
        std::string adresse = coord["adresse"].asString();
        std::string complement = coord["complement"].asString();
        int bp = coord["bp"].asInt();
        std::string ville = coord["ville"].asString();
        std::string pays = coord["pays"].asString();
        std::string fixe_dureau = coord["fixe_dureau"].asString();
        std::string fixe_domicile = coord["fixe_domicile"].asString();
        std::string phone = coord["phone"].asString();
        std::string phonePro = coord["phonePro"].asString();
        std::string email = coord["email"].asString();
        std::string emailPro = coord["emailPro"].asString();
        std::string prenom_urgence = coord["prenom_urgence"].asString();
        std::string phone_urgence = coord["phone_urgence"].asString();
        std::string email_urgence = coord["email_urgence"].asString();


        // Emploi
        const Json::Value emploi = body["emploi"];
        std::string contrat = emploi["contrat"].asString();
        std::string dateDebut = emploi["dateDebut"].asString();
        std::string statut = emploi["statut"].asString();

        // Paiement
        const Json::Value paiement = body["paiementInfo"];
        std::string preference = paiement["preference"].asString();

    // fonction SQL stockée pour addsalarie
  auto formatDate = [](const std::string& dateStr) -> std::string {
    return dateStr.empty() ? "NULL" : "'" + dateStr + "'";
};

std::string query =
"SELECT * FROM AddSalarie('" + matricule + "', " + std::to_string(sexe) + ", '" + nom + "', '" +
nomMarital + "', '" + nomUsuel + "', '" + situationFamiliale + "', '" + regimeMatrimonial + "', " +
formatDate(dateNaissance) + ", '" + villeNaissance + "', '" + paysNaissance + "', '" + paysNationalite + "', '" +
cnamgs + "', '" + cnss + "', '" + ascoma + "', " + formatDate(dateVmEmbauche) + ", '" + tailleBlouse + "', '" +
taillePantalon + "', '" + tailleChemise + "', '" + pointure + "', '" + niveauEtude + "', '" +
diplome + "', '" + specialisation + "', '" + diplomeEtablissement + "', " + formatDate(diplomeDateObtention) + ");";

         std::cout << "le resultat  : [" << query << "]" << std::endl;
        auto res = psql_->processQuery(query);

std::cout << "Résultat de la requête PostgreSQL :" << std::endl;

for (int i = 0; i < res.size(); ++i) {
    std::cout << "Ligne " << i << ": ";
    for (int j = 0; j < res[i].size(); ++j) {
        if (res[i][j].is_null()) {
            std::cout << "NULL";
        } else {
            std::cout << res[i][j].c_str();
        }

        if (j != res[i].size() - 1) {
            std::cout << " | ";
        }
    }
    std::cout << std::endl;
}


                std::string queryCoord =
                    "SELECT * FROM AddCoordonnees('" + matricule + "', '" + adresse + "', '" + complement + "', " +
                    std::to_string(bp) + ", '" + ville + "', '" + pays + "', '" + fixe_dureau + "', '" + fixe_domicile +
                    "', '" + phone + "', '" + phonePro + "', '" + email + "', '" + emailPro + "', '" + prenom_urgence +
                    "', '" + phone_urgence + "', '" + email_urgence + "');";

        // enfants           
        const Json::Value enfants = body["enfants"];
        if (enfants.isArray()) {
            for (const auto& enfant : enfants) {
                std::string matriculeE = enfant["matricule"].asString();
                std::string nomE = enfant["nom"].asString();
                std::string prenomE = enfant["prenom"].asString();
                std::string naissanceE = enfant["naissance"].asString();
                int sexeE = enfant["sexe"].asInt();

        // fonction SQL stockée pour addenfant
                std::string queryEnfant =
                    "SELECT * FROM AddEnfant('" + matriculeE + "', '" + nomE + "', '" + prenomE + "', '" + naissanceE + "', " + std::to_string(sexeE) + ");";
                psql_->processQuery(queryEnfant);
            }
        }

       // Permis
        const Json::Value permis = body["permis"];
        if (permis.isArray()) {
            for (const auto& p : permis) {
                std::string type = p["type"].asString();
                std::string numero = p["numero"].asString();
                std::string debut = p["debut"].asString();
                std::string fin = p["fin"].asString();
            // fonction SQL stockée pour addpermis
                std::string queryPermis =
                    "SELECT * FROM AddPermis('" + matricule + "', '" + type + "', '" + numero + "', '" + debut + "', '" + fin + "');";
                psql_->processQuery(queryPermis);
            }
        }

    } catch (const std::exception& e) {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field, e.what());
    }

    return HttpResponse(ResponseCode::OK, "Salarié ajouté avec succès");
}

}
}
