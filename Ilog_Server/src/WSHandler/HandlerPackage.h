#pragma once
#include <Common/Network/WebSocket/HandlerPackage.h>

typedef std::function<void(ClientConnection,
                            Common::Network::WebSocket::IWebSocketServer*,
                            Common::Network::WebSocket::IServerBase*)> logHandler;

typedef std::function<void(ClientConnection, const Json::Value&,
                            Common::Network::WebSocket::IWebSocketServer*,
                            Common::Network::WebSocket::IServerBase*)> msgHandler;
namespace ILogServer
{
namespace WSHandler
{
class HandlerPackage : public Common::Network::WebSocket::HandlerPackage
{
public:
    void addConnectionHandler(logHandler hdl)
    {
        connectHandlers.push_back(hdl);
    }

    void addDisconnectionHandler(logHandler hdl)
    {
        disconnectHandlers.push_back(hdl);
    }

    void addMessageHandler(const std::string& type, msgHandler hdl)
    {
        messageHandlers[type].push_back(hdl);
    }
};
}
}