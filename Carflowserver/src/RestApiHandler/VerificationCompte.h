#pragma once

#include <Common/Network/RestApi/IHandler.h> 

namespace CarflowServer
{
namespace RestApiHandler
{
class VerificationCompte : public Common::Network::RestApi::IHandler
{
private:
    static constexpr int TOKEN_VALIDITE = 15; // minutes
public:
    virtual ~VerificationCompte() {}
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}