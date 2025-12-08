#pragma once

#include <Common/Network/RestApi/IHandler.h> 


namespace ILogServer
{
namespace RestApiHandler
{
class GetEtablissement: public Common::Network::RestApi::IHandler
{
public:
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}