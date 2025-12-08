#pragma once

#include <Common/Network/WebSocket/IServerBase.h>
#include <Common/Network/WebSocket/IWebSocketServer.h>
#include <Common/Utils/Token.h>

namespace ILogServer
{
namespace WSHandler
{
void Authentication(ClientConnection conn, const Json::Value& json,
            Common::Network::WebSocket::IWebSocketServer* ws,
            Common::Network::WebSocket::IServerBase* server)
{
    std::cout << "authenticate" << std::endl;
    try
    {
        if (json.isMember("msg"))
        {
            auto msg = json["msg"];
            std::cout <<  msg.toStyledString() << std::endl; 
            const std::string username =  msg["username"].asString();
            const std::string sessionToken = msg["password"].asString();

            auto sessionMngr = ws->getSessionManager();
            auto decoded = sessionMngr->decodeAndVerifyToken(sessionToken);
            if (Common::Utils::Token::isTokenTimeout(decoded)) {
                server->sendMessageErr(conn, "/authenticate",
                    "{\"code\" : 1, \"msg\" : \"Token is expired\"}");
                return;
            }

            Json::Value body;
            Json::Reader reader;
            reader.parse(decoded.get_payload_claim("data").as_string(), body);

            auto username_ = body["username"].asString();

            if (username_ == username)
            {
                server->addClient(conn, sessionToken);
                ws->sendTo(sessionToken, "/authenticate",
                    "{\"code\" : 0, \"msg\" : \"Connection Success\"}");
                return;
            }
        }
        else
        {
            server->sendMessageErr(conn, "/authenticate",
                "{\"code\" : 1, \"msg\" : \"Invalid Json\"}");
        }
    }
    catch(...)
    {
        server->sendMessageErr(conn, "/authenticate",
                "{\"code\" : 1, \"msg\" : \"Failed during check token\"}");
                return;
    }

    server->sendMessageErr(conn, "/authenticate",
        "{\"code\" : 1, \"msg\" : \"Invalid token\"}");
}
}
}