#pragma once

#include <Common/Package/ApiRestHandlerPackage.h>

namespace ILogServer
{
class RestHandlerPackage : public Common::Package::ApiRestHandlerPackage
{
public:
    std::map<std::string, std::shared_ptr<Common::Network::RestApi::IHandler>> getHandlers() override final;
};
}