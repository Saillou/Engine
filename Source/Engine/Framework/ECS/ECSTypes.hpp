#pragma once

#include <bitset>
#include <cstdint>

using Entity                        = std::uint32_t;
const Entity MAX_ENTITIES           = 1000;

using ComponentType                 = std::uint8_t;
const ComponentType MAX_COMPONENTS  = 64;

using Signature                     = std::bitset<MAX_COMPONENTS>;
