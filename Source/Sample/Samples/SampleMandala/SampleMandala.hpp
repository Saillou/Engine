#pragma once

#include "../Sample.hpp"

class SampleMandala : 
    public Sample
{
public:
    SampleMandala();

private:
    std::unordered_map<std::string, Model::Ref> m_models;
};
