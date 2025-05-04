#pragma once

struct ModelIdle {
    enum class CultivationStage {
        Mortal,            // 0: Ordinary being
        Neophyte,          // 1: Beginner cultivator
        QiPractitioner,    // 2: Gathering and refining Qi
        FoundationBuilder, // 3: Establishing a strong internal base
        CoreBuilder,       // 4: Condensing a golden core
        NascentSoul,       // 5: Forming a nascent consciousness
        SpiritAwakening,   // 6: Awakening higher spiritual power
        NirvanaRebirth,    // 7: Dying and being reborn stronger
        Ascendant,         // 8: Starting to transcend mortality
        Immortal,          // 9: Achieved immortality
        CelestialSage,     // 10: Master of the heavens
        DaoLord,           // 11: True understanding of universal principles
        WorldCreator,      // 12: Able to create small worlds
        EternalOne         // 13: Beyond space and time
    };

    static std::string StageToString(CultivationStage s) {
        switch (s)
        {
        case ModelIdle::CultivationStage::Mortal:               return "Mortal";
        case ModelIdle::CultivationStage::Neophyte:             return "Neophyte";
        case ModelIdle::CultivationStage::QiPractitioner:       return "QiPractitioner";
        case ModelIdle::CultivationStage::FoundationBuilder:    return "FoundationBuilder";
        case ModelIdle::CultivationStage::CoreBuilder:          return "CoreBuilder";
        case ModelIdle::CultivationStage::NascentSoul:          return "NascentSoul";
        case ModelIdle::CultivationStage::SpiritAwakening:      return "SpiritAwakening";
        case ModelIdle::CultivationStage::NirvanaRebirth:       return "NirvanaRebirth";
        case ModelIdle::CultivationStage::Ascendant:            return "Ascendant";
        case ModelIdle::CultivationStage::Immortal:             return "Immortal";
        case ModelIdle::CultivationStage::CelestialSage:        return "CelestialSage";
        case ModelIdle::CultivationStage::DaoLord:              return "DaoLord";
        case ModelIdle::CultivationStage::WorldCreator:         return "WorldCreator";
        case ModelIdle::CultivationStage::EternalOne:           return "EternalOne";
        }
        return "<unknown>";
    };

    CultivationStage stage = CultivationStage::Mortal;
    int level              = 0;
    float base_qi          = 10.f;
    float curr_qi          = 0.f;
    float focus            = 0.f;
    float speed            = 0.f;
    float quantity         = 0.f;
};