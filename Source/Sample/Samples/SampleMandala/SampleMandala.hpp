#pragma once

#include "../Sample.hpp"

class SampleMandala : 
    public Sample
{
public:
    SampleMandala();

private:
    std::unordered_map<std::string, Entity> m_entities;
};
