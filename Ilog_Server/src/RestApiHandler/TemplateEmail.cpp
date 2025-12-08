#include <RestApiHandler/TemplateEmail.h>
#include <RestApiHandler/Salarie.h>  
#include <RestApiHandler/Rh/Conge.h>  
#include <Utils.h>  

std::string ILogServer::RestApiHandler::TemplateEmail::demandeConge(
    const Salarie& demandeur,
    const Salarie& responsable,
    const Conge& conge,
    const std::string& sToken)
{
    std::string email = 
        "<!DOCTYPE html><html><head></head><body>"
        "<p> Bonjour $civiliteResponsable $nomResponsable $prenomResponsable, <br/>"
        "$civiliteDemandeur $nomDemandeur $prenomDemandeur, matricule $matriculeDemandeur, "
        "sollicite un congé $typeConge, de $nbreJourConge jours à partir du $departConge "
        "pour un retour théorique le $retourTheoConge.<br/>"
        "$Commentaire <br/>"
        "Date de validation prévue : $dateValide <br/>"
        "</p> "
        "<input type=\"button\" value=\"Accepter\" onclick=\"accepted()\">"
        "<input type=\"button\" value=\"Refuser\" onclick=\"refused()\">"
        "<script type=\"text/javascript\">"
        "function accepted() {"
        "var xhr = new XMLHttpRequest();"
        "xhr.onreadystatechange = function () {"
        "if (xhr.readyState === 4) {"
        "alert(xhr.responseText);}}"
        "xhr.open('POST', '$url', true);"
        "xhr.setRequestHeader('Content-Type', 'application/json');"
        "xhr.setRequestHeader('User-Agent', '$appName $appVersion');"
        "xhr.setRequestHeader('Authorization', 'Bearer $token');"
        "xhr.send(JSON.stringify({\"id\": \"$id\", \"status\": \"accepted\", \"date_validation\": \"$dateValidation\"}));"
        "} "
        "function refused() {"
        "var xhr = new XMLHttpRequest();"
        "xhr.onreadystatechange = function () {"
        "if (xhr.readyState === 4) {"
        "alert(xhr.responseText);}}"
        "xhr.open('POST', '$url', true);"
        "xhr.setRequestHeader('Content-Type', 'application/json');"
        "xhr.setRequestHeader('User-Agent', '$appName $appVersion');"
        "xhr.setRequestHeader('Authorization', 'Bearer $token');"
        "xhr.send(JSON.stringify({\"id\": \"$id\", \"status\": \"refused\", \"date_validation\": \"$dateValidation\"}));"
        "} "
        "</script>"
        "</body></html>";

    ILogServer::replace(email, "$civiliteResponsable", responsable._civilite);
    ILogServer::replace(email, "$nomResponsable", responsable._nom);
    ILogServer::replace(email, "$prenomResponsable", responsable._prenom);
    
    ILogServer::replace(email, "$civiliteDemandeur", demandeur._civilite);
    ILogServer::replace(email, "$nomDemandeur", demandeur._nom);
    ILogServer::replace(email, "$prenomDemandeur", demandeur._prenom);
    ILogServer::replace(email, "$matriculeDemandeur", demandeur._matricule);
    
    ILogServer::replace(email, "$typeConge", conge._type);
    ILogServer::replace(email, "$nbreJourConge", std::to_string(conge._nbrJour));
    ILogServer::replace(email, "$departConge", conge._dateDebut);
    ILogServer::replace(email, "$retourTheoConge", conge._dateRetour);
    ILogServer::replace(email, "$Commentaire", conge._commentaire);

    ILogServer::replace(email, "$token", sToken); 

    return email;
}
