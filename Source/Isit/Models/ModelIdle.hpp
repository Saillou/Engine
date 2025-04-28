#pragma once

struct ModelIdle {
    enum CultivationStage {
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

    CultivationStage stage = CultivationStage::Mortal;
    int level              = 0;
    float base_qi          = 10.f;
    float curr_qi          = 0.f;
    float focus            = 0.f;
    float speed            = 0.f;
    float quantity         = 0.f;
};