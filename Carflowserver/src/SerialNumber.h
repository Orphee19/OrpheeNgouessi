#pragma once

#include <string>

namespace CarflowServer
{
class SerialNumber
{
public:
    static bool createSerialNumber(const std::string& macAddress, const std::string& deviceType, std::string& output);
};
}