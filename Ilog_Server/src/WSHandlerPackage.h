#pragma once

#include <Common/Package/WSHandlerPackage.h>

namespace ILogServer
{
class WSHandlerPackage : public Common::Package::WSHandlerPackage
{
public:
    std::shared_ptr<Common::Network::WebSocket::HandlerPackage> getHandler() override final;
};
}