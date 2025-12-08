#include <RestApiHandler/Inscription.h>
#include<Common/Utils/Utils.h>
#include <Common/Utils/Token.h>
#include <Common/Network/RestApi/Server.h>

namespace CarflowServer
{
namespace RestApiHandler
{
using namespace Common::Utils;
using namespace Common::Network;

HttpResponse Inscription::process(HttpRequest* req)
{
    auto psql_= getPSQL();
    if (psql_== nullptr)
    {
        return HttpResponse(ResponseErrorCode::service_unavailable, "service non disponible. Reessayez plus tard");
    }

    std::string username, nom, prenom, email, phone, password, naissance, contact;
    int sexe;
    try
    {
        const Json::Value requestBody = req->getBody();
        username = requestBody["username"].asString();
        nom = requestBody["nom"].asString();
        prenom = requestBody["prenom"].asString();
        email = requestBody["email"].asString();
        phone = requestBody["phone"].asString();
        password = requestBody["password"].asString();
        naissance = requestBody["naissance"].asString();
        contact = requestBody["contact"].asString();
        sexe = requestBody["sexe"].asUInt();
    }
    catch(const std::exception& e)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }
    
    bool ret =  (not nom.empty() and nom.size() <= 20)
            and str::isValidUsername(username)
            and (not prenom.empty() and prenom.size() <= 20)
            and (not phone.empty() or not email.empty())
            and (not phone.empty() or not email.empty())
            and (email.empty() or str::isValidEmail(email))
            and (not password.empty() and str::isValidPassword(password))
            and (phone.empty() or str::isValidPhone(phone))
            and (not naissance.empty() and str::isValidBirthDate(naissance, "%d-%m-%Y"))
            and (contact.empty() or (contact == "phone" or contact == "email"))
            and (sexe == 1 or sexe == 2);

    if (not ret)
    {
        return HttpResponse(ResponseErrorCode::Bad_Request, InternalResponseCode::Missing_or_Wrong_Field);
    }
    
    if (contact.empty())
    {
        contact = "email";
    }

    std::string dateConverted = str::convertDate(naissance, "%d-%m-%Y", "%Y-%m-%d");
    std::string query = "SELECT * FROM addUser('" + username + "'," + std::to_string(sexe) + ", '" + nom + "','" + prenom + "','" + phone + "','" + email + "','" + password + "','" + dateConverted + "','" + contact + "');";   
    
    auto res = psql_->processQuery(query);
   
    if (res.size() == 0 or not res[0][0].as<bool>())
    {
        return HttpResponse(ResponseErrorCode::Forbidden, InternalResponseCode::User_Already_Exist);
    }

    std::string id = res[0][1].as<std::string>();
    auto tok =  Token::generateRandomToken(25);
    auto code = Token::generateCode(6);

    if (contact == "email")
    {
        auto emailMngr = getEmailMgnr();
        if (emailMngr != nullptr and str::isValidEmail(email))
        {
            emailMngr->sendEmail(email, "Confirmation de compte", "code : " + code, [=, this](uint res){
                if (res == 0)
                {
                    auto ssMngr = getSessionMngr();
                    if (ssMngr != nullptr)
                        ssMngr->addCodeValidate(tok, {code, id});
                }
            });
        }
    }
    
    return HttpResponse(ResponseCode::OK, "{\"token\" : \"" + tok + "\" }");
}
}

}
