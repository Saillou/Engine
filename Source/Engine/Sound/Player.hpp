#pragma once

#include <memory>
#include <string>

struct ma_engine;
struct ma_sound;

class SoundPlayer {
    static SoundPlayer* s_player;

    // -- Singleton - No copy --
    SoundPlayer();

    SoundPlayer& operator=(const SoundPlayer&) = delete;
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer(SoundPlayer&&) = delete;

    void _playLoop(const std::string& pathFile);

    ma_engine* p_engine    = nullptr;
    ma_sound* p_curr_sound = nullptr;

    bool m_is_init    = false;
    bool m_is_playing = false;

public:
    ~SoundPlayer();

    static void Create();
    static void Destroy();

    static void PlayLoop(const std::string& pathFile);
};