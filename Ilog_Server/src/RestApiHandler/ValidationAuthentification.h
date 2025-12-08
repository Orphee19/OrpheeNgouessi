#pragma once

#include <Common/Network/RestApi/IHandler.h> 

namespace ILogServer
{
namespace RestApiHandler
{
class ValidationAuthentification : public Common::Network::RestApi::IHandler
{
private:
    static constexpr int TOKEN_VALIDITE = 1; // minutes
public:
    virtual ~ValidationAuthentification() {}
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}