#pragma once

#include <Common/Network/WebSocket/IServerBase.h>
#include <Common/Network/WebSocket/IWebSocketServer.h>

namespace ILogServer
{
namespace WSHandler
{
void disconnection(ClientConnection,
                    Common::Network::WebSocket::IWebSocketServer*,
                    Common::Network::WebSocket::IServerBase*)
{

}
}
}
