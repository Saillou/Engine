#include "ShaderManager.hpp"

#include "Shaders/ShaderGeometry.hpp"
#include "Shaders/ShaderParticle.hpp"
#include "Shaders/ShaderShadow.hpp"
#include "Shaders/ShaderShape.hpp"
#include "Shaders/ShaderSkybox.hpp"
#include "Shaders/ShaderSolid.hpp"
#include "Shaders/ShaderText.hpp"

#include <iostream>

std::shared_ptr<ShaderManager> ShaderManager::_s_manager = nullptr;

// Statics
void ShaderManager::Init() {
	if (_s_manager)
		return;

	struct _ShaderManager_ : public ShaderManager {
		_ShaderManager_() : ShaderManager() {};
	};

	_s_manager = std::make_shared<_ShaderManager_>();
	_s_manager->_create_default_shader();
}

void ShaderManager::Provide(const std::string& identifier, std::shared_ptr<Shader> shader) {
	if (Has(identifier)) {
		std::cerr << "Warning replacing already existing shader" << std::endl;
	}

	if (!shader->linked()) {
		shader->link();
	}

	_s_manager->_shaders[identifier] = shader;
}

void ShaderManager::Release(const std::string& identifier) {
	if (!Has(identifier))
		return;

	_s_manager->_shaders.erase(identifier);
}

bool ShaderManager::Has(const std::string& identifier)
{
	return _s_manager->_shaders.find(identifier) != _s_manager->_shaders.cend();
}

void ShaderManager::Edit(CookType identifier, ShaderSource::Type sourceType, const ShaderSource& new_source)
{
	return Edit(_getUUID(identifier), sourceType, new_source);
}

void ShaderManager::Edit(const std::string& identifier, ShaderSource::Type sourceType, const ShaderSource& new_source)
{
	if (!Has(identifier))
		return;

	Shader::Release();

    std::shared_ptr<Shader> old_shader = _s_manager->_shaders[identifier];
	std::shared_ptr<Shader> new_shader = std::make_shared<Shader>();

    for (const auto& old_source: old_shader->m_sources) {
        new_shader->attachSource(old_source.first,
            old_source.first == sourceType ?
            ShaderSource::Edit(old_source.second, new_source) :
            old_source.second
        );
    }

    new_shader->link();
	_s_manager->_shaders[identifier].swap(new_shader);
}

Shader& ShaderManager::Get(CookType type) {
	return Get(_getUUID(type));
}

Shader& ShaderManager::Get(const std::string& identifier) {
	assert(Has(identifier) && "Shader not available");

	return *_s_manager->_shaders.at(identifier);
}

// Instances
std::string ShaderManager::_getUUID(CookType type)
{
	return "#DEFAULT#" + std::to_string((int)type);
}

void ShaderManager::_create_default_shader() 
{
	// Create, compile and link default shaders
	Provide(_getUUID(CookType::Geometry),	ShaderGeometry::Create());
	Provide(_getUUID(CookType::Particle),	ShaderParticle::Create());
	Provide(_getUUID(CookType::Shadow),		ShaderShadow::Create());
	Provide(_getUUID(CookType::Shape),		ShaderShape::Create());
	Provide(_getUUID(CookType::Solid),		ShaderSolid::Create());
	Provide(_getUUID(CookType::Skybox),		ShaderSkybox::Create());
	Provide(_getUUID(CookType::Text),		ShaderText::Create());
}
