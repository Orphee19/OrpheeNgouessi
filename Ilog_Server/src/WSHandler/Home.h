#pragma once

#include <Common/Network/WebSocket/IServerBase.h>
#include <Common/Network/WebSocket/IWebSocketServer.h>

namespace ILogServer
{
namespace WSHandler
{
void home(ClientConnection, const Json::Value&,
            Common::Network::WebSocket::IWebSocketServer*,
            Common::Network::WebSocket::IServerBase*)
{

}
}
}