#pragma once

#include "../../_imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

#include <unordered_map>
#include <vector>

struct WorldEditorMenu
{
	WorldEditorMenu() {};
	~WorldEditorMenu() {};
		
	void show();
	void reset();

	void createSection(const std::string& sectionName);
	void addContent(const std::string& sectionName, const std::string& articleName, const glm::vec3& value);
	void addContent(const std::string& sectionName, const std::string& articleName, const glm::vec2& value);
	void addContent(const std::string& sectionName, const std::string& articleName, const float& value);
	void addContent(const std::string& sectionName, const std::string& articleName, const int& value);

private:
	typedef std::unordered_map<std::string, std::string> _row;
	typedef std::unordered_map<std::string, _row> _table;

	_table _datas;
};