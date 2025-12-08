#pragma once

#include <Common/Network/RestApi/IHandler.h> 

namespace CarflowServer
{
namespace RestApiHandler
{
class AddScannerProduct : public Common::Network::RestApi::IHandler
{
public:
    virtual ~AddScannerProduct() {}
    Common::Network::HttpResponse process(Common::Network::HttpRequest*) final override;
};
}
}