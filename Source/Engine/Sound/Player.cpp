#include "Player.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

SoundPlayer* SoundPlayer::s_player = nullptr;


SoundPlayer::SoundPlayer() {
	p_engine = new ma_engine();
	p_curr_sound = new ma_sound();

	m_is_init = ma_engine_init(NULL, p_engine) == MA_SUCCESS;
}

SoundPlayer::~SoundPlayer() {
	if (m_is_playing) {
		ma_sound_stop(p_curr_sound);
		ma_sound_uninit(p_curr_sound);
	}
	if (p_curr_sound) {
		delete p_curr_sound;
	}

	if (m_is_init) {
		ma_engine_uninit(p_engine);
	}
	if(p_engine)
		delete p_engine;
}

void SoundPlayer::_playLoop(const std::string& pathFile) {
	if (!m_is_init)
		return;

	if (m_is_playing) {
		ma_sound_stop(p_curr_sound);
		ma_sound_uninit(p_curr_sound);
		m_is_playing = false;
	}

	if (ma_sound_init_from_file(p_engine, pathFile.c_str(), 0, NULL, NULL, p_curr_sound) == MA_SUCCESS) {
		m_is_playing = true;
		ma_sound_set_looping(p_curr_sound, 1);
		ma_sound_start(p_curr_sound);
	}
}

// -----
void SoundPlayer::Create() {
	if (s_player)
		return;

	s_player = new SoundPlayer();
}

void SoundPlayer::Destroy() {
	if (!s_player)
		return;

	delete s_player;
}

void SoundPlayer::PlayLoop(const std::string& pathFile) {
	if (!s_player || !s_player->m_is_init)
		return;

	s_player->_playLoop(pathFile);
}
