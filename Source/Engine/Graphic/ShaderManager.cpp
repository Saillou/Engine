#include "ShaderManager.hpp"

#include "Shaders/ShaderGeometry.hpp"
#include "Shaders/ShaderParticle.hpp"
#include "Shaders/ShaderShadow.hpp"
#include "Shaders/ShaderShape.hpp"
#include "Shaders/ShaderSurface.hpp"
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
	_s_manager->_create_default_shaders();
}

void ShaderManager::Provide(CookType::_tag_ identifier, std::shared_ptr<Shader> shader) {
	if (Has(identifier)) {
		std::cerr << "Warning replacing already existing shader" << std::endl;
	}

	if (!shader->linked()) {
		shader->link();
	}

	_s_manager->_shaders[identifier] = shader;
}

void ShaderManager::Provide(CookType::_tag_ identifier, std::shared_ptr<Shader> shader, UniformSetter setter)
{
	Provide(identifier, shader);
	_s_manager->_shadersSetters[identifier] = setter;
}

void ShaderManager::Release(CookType::_tag_ identifier) {
	if (_s_manager->_shaders.find(identifier) != _s_manager->_shaders.cend())
		_s_manager->_shaders.erase(identifier);

	if (_s_manager->_shadersSetters.find(identifier) != _s_manager->_shadersSetters.cend())
		_s_manager->_shadersSetters.erase(identifier);
}

bool ShaderManager::Has(CookType::_tag_ identifier)
{
	return _s_manager->_shaders.find(identifier) != _s_manager->_shaders.cend();
}

void ShaderManager::Edit(CookType::_tag_ identifier, ShaderSource::Type sourceType, const ShaderSource& new_source)
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

void ShaderManager::Apply(CookType::_tag_ identifier)
{
	if (_s_manager->_shadersSetters.find(identifier) == _s_manager->_shadersSetters.cend())
		return;

	return _s_manager->_shadersSetters.at(identifier)();
}

Shader& ShaderManager::Get(CookType::_tag_ identifier) {
	assert(Has(identifier) && "Shader not available");

	return *_s_manager->_shaders.at(identifier);
}

// Instances
void ShaderManager::_create_default_shaders() 
{
	// Create, compile and link default shaders
	Provide(CookType::Geometry,	ShaderGeometry::Create());
	Provide(CookType::Particle,	ShaderParticle::Create());
	Provide(CookType::Shadow,	ShaderShadow::Create());
	Provide(CookType::Surface,	ShaderSurface::Create());
	Provide(CookType::Shape,	ShaderShape::Create());
	Provide(CookType::Solid,	ShaderSolid::Create());
	Provide(CookType::Skybox,	ShaderSkybox::Create());
	Provide(CookType::Text,		ShaderText::Create());
}
