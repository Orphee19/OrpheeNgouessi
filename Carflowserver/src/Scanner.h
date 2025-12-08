#pragma once

#include <Common/Utils/EnumStringifier.h>

namespace CarflowServer
{
enum DeviceStatus
{
    CREATED = 1,
    CONFIGURED = 2,
    BLOCKED = 3,
};


const Common::Utils::EnumStringifier<DeviceStatus> DeviceStatusStringifier
{
    {
        {DeviceStatus::CREATED, "create"},
        {DeviceStatus::CONFIGURED, "configured"},
        {DeviceStatus::BLOCKED, "blocked"},
    }
};
    
} // namespace CarflowServer
