#pragma once

#include <Common/Network/RestApi/IHandler.h> 

namespace CarflowServer
{
namespace RestApiHandler
{
class ConfigurationScanner : public Common::Network::RestApi::IHandler
{
public:
    virtual ~ConfigurationScanner() {}
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}