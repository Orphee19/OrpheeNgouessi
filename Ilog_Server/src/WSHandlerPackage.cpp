#include "WSHandlerPackage.h"
#include "WSHandler/Authentication.h"
#include "WSHandler/Connection.h"
#include "WSHandler/Disconnection.h"
#include "WSHandler/HandlerPackage.h"
#include "WSHandler/Home.h"

namespace ILogServer
{
using namespace std::placeholders;

std::shared_ptr<Common::Network::WebSocket::HandlerPackage> WSHandlerPackage::getHandler()
{
    std::shared_ptr<Common::Network::WebSocket::HandlerPackage> handlers = 
            std::make_shared<WSHandler::HandlerPackage>();
    auto hdlr = dynamic_cast<WSHandler::HandlerPackage*>(handlers.get());
    if (hdlr)
    {
        hdlr->addConnectionHandler(WSHandler::connection);
        hdlr->addDisconnectionHandler(WSHandler::disconnection);
        hdlr->addMessageHandler("/", WSHandler::home);
        hdlr->addMessageHandler("/authenticate", WSHandler::Authentication);
    }
    return handlers;
}
};