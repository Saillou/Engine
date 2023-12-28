#include "ApplicationManager.hpp"

#include "Commanders/AppCommander.hpp"
#include "Commanders/FireCommander.hpp"

#include "Scenes/AppScene.hpp"
#include "Scenes/FireScene.hpp"
#include "Scenes/BloomScene.hpp"
#include "Scenes/LearnScene.hpp"

#include "../Engine/Sound/Player.hpp"


ApplicationManager* ApplicationManager::s_appManager = nullptr;


// Instance
ApplicationManager::ApplicationManager() 
{
	// Scene events
	_subscribe(&ApplicationManager::_on_scene_refresh);
	_subscribe(&ApplicationManager::_on_scene_ended);
}

ApplicationManager::~ApplicationManager() {
	// ..
}

void ApplicationManager::_on_scene_refresh(const CommonEvents::SceneRefresh& evt) {
	_sceneId.push(_currSceneId);
}

void ApplicationManager::_on_scene_ended(const CommonEvents::SceneEnded& evt) {
	_sceneId.push(SceneId::None);
}


// Statics
void ApplicationManager::Create() {
	SoundPlayer::Create();

	if(!s_appManager)
		s_appManager = new ApplicationManager();
}
void ApplicationManager::Destroy() {
	SoundPlayer::Destroy();

	if(s_appManager)
		delete s_appManager;
}

void ApplicationManager::SetScene(SceneId sceneId) {
	if (!s_appManager)
		return;

	s_appManager->_sceneId.push(sceneId);
}
void ApplicationManager::KeyPressed(unsigned int key) {
	if (!s_appManager)
		return;

	s_appManager->_keyPressed.push(key);
}
void ApplicationManager::MousePos(const glm::vec2& mousePos) {
	if (!s_appManager)
		return;

	s_appManager->_mousePos.push(mousePos);
}

ApplicationManager::ActionCode ApplicationManager::UpdateState() {
	// Unable to continue
	if (!s_appManager)
		return ActionCode::Close;

	// Need to refresh
	if (!s_appManager->_sceneId.empty())
		return ActionCode::Refresh;

	// Unqueue events
	Event::Emit(CommonEvents::StateUpdated());

	// Keyboards
	while (!s_appManager->_keyPressed.empty()) {
		Event::Emit(CommonEvents::KeyPressed(
			s_appManager->_keyPressed.front()
		));
		s_appManager->_keyPressed.pop();
	}

	// Mouse
	while (!s_appManager->_mousePos.empty()) {
		Event::Emit(CommonEvents::MouseMoved(
			(int)s_appManager->_mousePos.front().x, 
			(int)s_appManager->_mousePos.front().y
		));
		s_appManager->_mousePos.pop();
	}

	return ActionCode::Ok;
}

void ApplicationManager::Refresh(Window& window) {
	if (!s_appManager)
		return;

	SceneId sceneId = None;

	// Skip to last scene
	while (!s_appManager->_sceneId.empty()) {
		sceneId = s_appManager->_sceneId.front();
		s_appManager->_sceneId.pop();
	}

	// Change view
	window.scene(([&]() -> std::shared_ptr<BaseScene> {
		switch (sceneId)
		{
			case SceneId::App:
				return std::make_shared<AppScene>();

			case SceneId::Bloom:
				return std::make_unique<BloomScene>();

			case SceneId::Fire:
				return std::make_unique<FireScene>();

			case SceneId::Learn:
				return std::make_unique<LearnScene>();

			default: 
				return std::make_shared<BaseScene>();
		}
	})());

	// Change commander
	s_appManager->_commander = ([&]() -> std::unique_ptr<BaseCommander> {
		switch (sceneId)
		{
			case SceneId::App:
				return std::make_unique<AppCommander>(window.scene());

			case SceneId::Fire:
				return std::make_unique<FireCommander>(window.scene());

			default:
				return std::make_unique<BaseCommander>(window.scene());
		}
	})();

	// Change sound
	switch (sceneId)
	{
		case SceneId::App:  
			SoundPlayer::PlayLoop("Resources/Intro.wav"); 
			break;
	}

	// Update internals
	s_appManager->_currSceneId = sceneId;
}
