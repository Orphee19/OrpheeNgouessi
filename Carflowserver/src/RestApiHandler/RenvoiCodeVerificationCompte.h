#pragma once

#include <Common/Network/RestApi/IHandler.h> 

namespace CarflowServer
{
namespace RestApiHandler
{
class RenvoiCodeVerificationCompte : public Common::Network::RestApi::IHandler
{
public:
    virtual ~RenvoiCodeVerificationCompte() {}
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}