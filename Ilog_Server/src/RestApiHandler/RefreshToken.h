#pragma once

#include <Common/Network/RestApi/IHandler.h> 

namespace ILogServer
{
namespace RestApiHandler
{
class RefreshToken : public Common::Network::RestApi::IHandler
{
public:
    virtual ~RefreshToken() {}
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}