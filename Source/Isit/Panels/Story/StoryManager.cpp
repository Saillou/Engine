#include "StoryManager.hpp"

#include "Scenes/Scene_debug.hpp"
#include "Scenes/Scene_intro.hpp"
#include "Scenes/Scene_dead.hpp"
#include "Scenes/Scene_root.hpp"
#include "Scenes/Scene_newGame.hpp"

StoryManager::StoryManager()
{
    Scene_debug::Write(_story);
    Scene_root::Write(_story);
    Scene_intro::Write(_story);
    Scene_newGame::Write(_story);
    Scene_dead::Write(_story);
}
